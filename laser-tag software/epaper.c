#include "fsl_gpio_driver.h"
#include "fsl_spi_master_driver.h"

static const gpio_output_pin_user_config_t pinReset = {
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 20),
    .config.outputLogic = 0, // XXX
};

static const gpio_output_pin_user_config_t pinDischarge = {
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 19),
    .config.outputLogic = 0,
};

static const spi_master_user_config_t spiConfig = {
    .bitsPerSec = 4000000, /* 4 MHz, max is 20 MHz */
    .polarity = kSpiClockPolarity_ActiveHigh,
    .phase = kSpiClockPhase_FirstEdge,
    .direction = kSpiMsbFirst,
    .bitCount = kSpi8BitMode,
};

static spi_master_state_t spiState;

extern void led(uint8_t red, uint8_t green, uint8_t blue);

void EPD_Init()
{
    PORT_HAL_SetMuxMode(g_portBase[GPIOA_IDX], 19, kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(g_portBase[GPIOA_IDX], 20, kPortMuxAsGpio);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 4, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 5, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 6, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 7, kPortMuxAlt2);
    GPIO_DRV_OutputPinInit(&pinReset);
    GPIO_DRV_OutputPinInit(&pinDischarge);
    SPI_DRV_MasterInit(1, &spiState);
    uint32_t calculatedBaudRate;
    SPI_DRV_MasterConfigureBus(1, &spiConfig, &calculatedBaudRate);
}

void EPD_Draw()
{
    /* Reset */
    GPIO_DRV_WritePinOutput(pinReset.pinName, 0);
    int i;
    for(i = 0; i < 0xffff; ++i);
    //GPIO_DRV_WritePinOutput(pinReset.pinName, 1);

    uint8_t tx[2], rx[2];

    tx[0] = 0x71;
    tx[1] = 0x00;
    SPI_DRV_MasterTransferBlocking(1, NULL, tx, rx, 2, 1);
    SPI_DRV_MasterTransferBlocking(1, NULL, tx, rx, 2, 1);
    if ((rx[1] & 0x0f) != 0x02) {
        led(0xff, 0x00, 0x00);
    } else {
        led(0x00, 0xff, 0x00);
    }

    /* Shutdown */
    GPIO_DRV_WritePinOutput(pinDischarge.pinName, 1);
    GPIO_DRV_WritePinOutput(pinDischarge.pinName, 0);
}

/* vim: set expandtab ts=4 sw=4: */
