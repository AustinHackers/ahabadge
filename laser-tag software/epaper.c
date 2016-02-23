#include "fsl_gpio_driver.h"
#include "fsl_spi_master_driver.h"

static const gpio_output_pin_user_config_t pinDischarge = {
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 19),
    .config.outputLogic = 0,
};

static const gpio_output_pin_user_config_t pinCS = {
    .pinName = GPIO_MAKE_PIN(GPIOD_IDX, 4),
    .config.outputLogic = 1,
};

static const spi_master_user_config_t spiConfig = {
    .bitsPerSec = 2000000, /* 2 MHz, max is 20 MHz */
    .polarity = kSpiClockPolarity_ActiveHigh,
    .phase = kSpiClockPhase_FirstEdge,
    .direction = kSpiMsbFirst,
    .bitCount = kSpi8BitMode,
};

static const uint8_t channel_select[] = {
    0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0xff
};

static spi_master_state_t spiState;

void EPD_Init()
{
    PORT_HAL_SetMuxMode(g_portBase[GPIOA_IDX], 19, kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(g_portBase[GPIOA_IDX], 20, kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 4, kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 5, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 6, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 7, kPortMuxAlt2);
    GPIO_DRV_OutputPinInit(&pinDischarge);
    GPIO_DRV_OutputPinInit(&pinCS);
    SPI_DRV_MasterInit(1, &spiState);
    uint32_t calculatedBaudRate;
    SPI_DRV_MasterConfigureBus(1, &spiConfig, &calculatedBaudRate);
}

void EPD_WriteCommandBuffer(uint8_t index, const uint8_t *data, size_t length)
{
    uint8_t tx[3] = { 0x70, index, 0x72 };

    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_DRV_MasterTransferBlocking(1, NULL, tx, NULL, 2, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_DRV_MasterTransferBlocking(1, NULL, &tx[2], NULL, 1, 1);
    SPI_DRV_MasterTransferBlocking(1, NULL, data, NULL, length, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
}

void EPD_WriteCommandByte(uint8_t index, uint8_t data)
{
    EPD_WriteCommandBuffer(index, &data, 1);
}

uint8_t EPD_ReadCommand(uint8_t index)
{
    uint8_t tx[4] = { 0x70, index, 0x73, 0x00 };
    uint8_t rx[2];

    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_DRV_MasterTransferBlocking(1, NULL, tx, NULL, 2, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_DRV_MasterTransferBlocking(1, NULL, &tx[2], rx, 2, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    return rx[1];
}

uint8_t EPD_ReadCogID()
{
    uint8_t tx[2] = { 0x71, 0x00 };
    uint8_t rx[2];

    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_DRV_MasterTransferBlocking(1, NULL, tx, rx, sizeof tx, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    return rx[1];
}

void EPD_Delay(uint32_t ms)
{
    for (; ms > 0; --ms) {
        /* XXX This is really stupid */
        volatile int i;
        for (i = 0; i < 306; ++i);
    }
}

int EPD_Draw()
{
    /* read the COG ID */
    uint8_t id = EPD_ReadCogID();
    if ((id & 0x0f) != 0x02) {
        return -1;
    }

    /* disable OE */
    EPD_WriteCommandByte(0x02, 0x40);

    /* check breakage */
    uint8_t broken_panel = EPD_ReadCommand(0x0f);
    if (0x00 == (0x80 & broken_panel)) {
        return -2;
    }

    /* power saving mode */
    EPD_WriteCommandByte(0x0b, 0x02);

    /* channel select */
    EPD_WriteCommandBuffer(0x01, channel_select, sizeof channel_select);

    /* high power mode osc */
    EPD_WriteCommandByte(0x07, 0xd1);

    /* power setting */
    EPD_WriteCommandByte(0x08, 0x02);

    /* Vcom level */
    EPD_WriteCommandByte(0x09, 0xc2);

    /* power setting */
    EPD_WriteCommandByte(0x04, 0x03);

    /* driver latch on */
    EPD_WriteCommandByte(0x03, 0x01);

    /* driver latch off */
    EPD_WriteCommandByte(0x03, 0x00);
    EPD_Delay(5);

    int i;
    for (i = 0; i < 4; ++i) {
        /* charge pump positive voltage on - VGH/VDL on */
        EPD_WriteCommandByte(0x05, 0x01);
        EPD_Delay(240);

        /* charge pump negative voltage on - VGL/VDL on */
        EPD_WriteCommandByte(0x05, 0x03);
        EPD_Delay(40);

        /* charge pump Vcom on - Vcom driver on */
        EPD_WriteCommandByte(0x05, 0x0f);
        EPD_Delay(40);

        /* check DC/DC */
        uint8_t dc_state = EPD_ReadCommand(0x0f);
        if (0x40 & dc_state)
            break;
    }

    if (4 == i)
        return -3;

    /* output enable to disable */
    EPD_WriteCommandByte(0x02, 0x40);

    /* TODO Draw something */

#if 0
    /* Shutdown */
    GPIO_DRV_WritePinOutput(pinDischarge.pinName, 1);
    GPIO_DRV_WritePinOutput(pinDischarge.pinName, 0);
#endif
    return 0;
}

void EPD_Deinit()
{
    SPI_DRV_MasterDeinit(1);
}

/* vim: set expandtab ts=4 sw=4: */
