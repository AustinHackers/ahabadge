#include "radio.h"
#include "fsl_debug_console.h"
#include "fsl_gpio_driver.h"
#include "fsl_spi_master_driver.h"
#include "RFM69registers.h"

static const gpio_output_pin_user_config_t pinReset = {
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 8),
    .config.outputLogic = 1,
};

static const gpio_input_pin_user_config_t pinDIO0 = {
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 9),
};

static const gpio_output_pin_user_config_t pinDIO2 = {
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 10),
    .config.outputLogic = 0,
};

static const spi_master_user_config_t spiConfig = {
    .bitsPerSec = 10000000, /* max is 10 MHz */
    .polarity = kSpiClockPolarity_ActiveHigh,
    .phase = kSpiClockPhase_FirstEdge,
    .direction = kSpiMsbFirst,
    .bitCount = kSpi16BitMode,
};

static const struct {
    uint8_t reg;
    uint8_t val;
} config[] = {
    { REG_OPMODE, RF_OPMODE_SEQUENCER_ON
                | RF_OPMODE_LISTEN_OFF
                | RF_OPMODE_STANDBY },
    { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET
                   | RF_DATAMODUL_MODULATIONTYPE_FSK
                   | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
    { REG_BITRATEMSB, RF_BITRATEMSB_55555 }, // 55,555 bps
    { REG_BITRATELSB, RF_BITRATELSB_55555 },
    { REG_FDEVMSB, RF_FDEVMSB_50000 }, // default: 5KHz,
    { REG_FDEVLSB, RF_FDEVLSB_50000 }, // (FDEV + BitRate / 2 <= 500KHz)
    { REG_FRFMSB, RF_FRFMSB_915 },
    { REG_FRFMID, RF_FRFMID_915 },
    { REG_FRFLSB, RF_FRFLSB_915 },
    // looks like PA1 and PA2 are not implemented on RFM69W,
    // hence the max output power is 13dBm
    // +17dBm and +20dBm are possible on RFM69HW
    // +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1**)
    // +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
    // +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high
    // power PA settings (section 3.3.7 in datasheet)
    { REG_PALEVEL, RF_PALEVEL_PA0_OFF
                 | RF_PALEVEL_PA1_ON
                 | RF_PALEVEL_PA2_ON
                 | RF_PALEVEL_OUTPUTPOWER_11111 },
    { REG_TESTPA1, 0x5D },
    { REG_TESTPA2, 0x7C },
    { REG_OCP, RF_OCP_OFF }, // over current protection
    // RXBW defaults are RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5
    // (RxBw: 10.4KHz)
    //for BR-19200: RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3
    // (BitRate < 2 * RxBw)
    { REG_RXBW, RF_RXBW_DCCFREQ_010
              | RF_RXBW_MANT_16
              | RF_RXBW_EXP_2 },
    // DIO0 is the only IRQ we're using
    { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 },
    // DIO5 ClkOut disable for power saving
    { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF },
    // writing to this bit ensures that the FIFO & status flags are reset
    { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN },
    // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
    { REG_RSSITHRESH, 220 },
    // default 3 preamble bytes 0xAAAAAA
    //{ REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE }
    { REG_SYNCCONFIG, RF_SYNC_ON
                    | RF_SYNC_FIFOFILL_AUTO
                    | RF_SYNC_SIZE_2
                    | RF_SYNC_TOL_0 },
    // attempt to make this compatible with sync1 byte of RFM12B lib
    { REG_SYNCVALUE1, 0x2D },
    { REG_SYNCVALUE2, 0x01 }, // NETWORK ID (1 = users, 2 = uber)
    { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE
                       | RF_PACKET1_DCFREE_OFF
                       | RF_PACKET1_CRC_ON
                       | RF_PACKET1_CRCAUTOCLEAR_ON
                       | RF_PACKET1_ADRSFILTERING_OFF },
    // in variable length mode: the max frame size, not used in TX
    { REG_PAYLOADLENGTH, 66 },
    // turned off because we're not using address filtering
    //{ REG_NODEADRS, nodeID },
    { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY
                    | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
    // RXRESTARTDELAY must match transmitter PA ramp-down time
    // (bitrate dependent)
    //for BR-19200: RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON
    //| RF_PACKET2_AES_OFF
    { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS
                       | RF_PACKET2_AUTORXRESTART_ON
                       | RF_PACKET2_AES_OFF },
    // run DAGC continuously in RX mode for Fading Margin Improvement,
    // recommended default for AfcLowBetaOn=0
    { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 },
};

static spi_master_state_t spiState;

static void delay(uint32_t ms)
{
    for (; ms > 0; --ms) {
        /* XXX This is really stupid */
        volatile int i;
        for (i = 0; i < 306 * 12; ++i);
    }
}

static spi_status_t SPI_Transfer(const uint8_t *tx, uint8_t *rx, size_t count)
{
    spi_status_t rc;

    rc = SPI_DRV_MasterTransfer(0, NULL, tx, rx, count);
    if (rc != kStatus_SPI_Success) {
        return rc;
    }
    int i, timeout = count * 2;
    for (i = 0; i < timeout; ++i) {
        rc = SPI_DRV_MasterGetTransferStatus(0, NULL);
        if (rc == kStatus_SPI_Success) {
            return rc;
        }
    }
    return rc;
}

static uint8_t read_reg(uint8_t reg)
{
	uint8_t tx[2] = { 0, reg & 0x7f };
	uint8_t rx[2] = { 0, 0 };

    SPI_Transfer(tx, rx, 2);
	return rx[0];
}

static void write_reg(uint8_t reg, uint8_t val)
{
	uint8_t tx[2] = { val, reg | 0x80 };

    SPI_Transfer(tx, NULL, 2);
}

int radio_init()
{
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 4, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 5, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 6, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 7, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 8, kPortMuxAsGpio);
    //PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 10, kPortMuxAsGpio);
    GPIO_DRV_OutputPinInit(&pinReset);
    //GPIO_DRV_OutputPinInit(&pinDIO2);
    delay(1);
    GPIO_DRV_WritePinOutput(pinReset.pinName, 0);
    delay(5);

    /* Configure SPI0 */
    SPI_DRV_MasterInit(0, &spiState);
    uint32_t calculatedBaudRate;
    SPI_DRV_MasterConfigureBus(0, &spiConfig, &calculatedBaudRate);
    debug_printf("radio baud rate %u Hz\r\n", calculatedBaudRate);

    /* Use REG_SYNCVALUE1 to test that the radio is there */
	write_reg(REG_SYNCVALUE1, 0x55);
	uint8_t reg = read_reg(REG_SYNCVALUE1);
	if (reg != 0x55) {
		return -1;
	}

    /* Setup a bunch of config registers */
    uint32_t i;
    for (i = 0; i < sizeof config / sizeof *config; ++i) {
        write_reg(config[i].reg, config[i].val);
    }
	return 0;
}

void set_mode(uint8_t mode)
{
    uint8_t reg = read_reg(REG_OPMODE);

    write_reg(REG_OPMODE, (reg & 0xE3) | mode);
}

void radio_test(void)
{
    //write_reg(REG_DATAMODUL, RF_DATAMODUL_DATAMODE_CONTINUOUSNOBSYNC);
    set_mode(RF_OPMODE_TRANSMITTER);
    /*
    bool a = false;
    while (true) {
        GPIO_DRV_WritePinOutput(pinDIO2.pinName, a);
        a = !a;
        delay(100);
    }*/
}

/* vim: set expandtab ts=4 sw=4: */
