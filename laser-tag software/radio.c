#include "radio.h"
#include "fsl_gpio_driver.h"
#include "fsl_spi_master_driver.h"

static const gpio_output_pin_user_config_t pinReset = {
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 8),
    .config.outputLogic = 1,
};

static const spi_master_user_config_t spiConfig = {
    .bitsPerSec = 2000000, /* 2 MHz, max is 10 MHz */
    .polarity = kSpiClockPolarity_ActiveHigh,
    .phase = kSpiClockPhase_FirstEdge,
    .direction = kSpiMsbFirst,
    .bitCount = kSpi16BitMode,
};

static spi_master_state_t spiState;

static spi_status_t SPI_Transfer(const uint8_t *tx, uint8_t *rx, size_t count)
{
    spi_status_t rc;

    rc = SPI_DRV_MasterTransfer(0, NULL, tx, rx, count);
    if (rc != kStatus_SPI_Success) {
        return rc;
    }
    int i, timeout = (count + 127) / 128 + 1;
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

static void delay(uint32_t ms)
{
    for (; ms > 0; --ms) {
        /* XXX This is really stupid */
        volatile int i;
        for (i = 0; i < 306; ++i);
    }
}

int radio_init()
{
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 4, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 5, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 6, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 7, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 8, kPortMuxAsGpio);
    GPIO_DRV_OutputPinInit(&pinReset);
    delay(1);
    GPIO_DRV_WritePinOutput(pinReset.pinName, 0);
    delay(5);

    /* Configure DMA channel */
    SPI_DRV_MasterInit(0, &spiState);
    uint32_t calculatedBaudRate;
    SPI_DRV_MasterConfigureBus(0, &spiConfig, &calculatedBaudRate);

	write_reg(0x2f, 0xaa);
	uint32_t reg = read_reg(0x2f);
	if (reg != 0xaa) {
		return -1;
	}
	return 0;
}

/* vim: set expandtab ts=4 sw=4: */
