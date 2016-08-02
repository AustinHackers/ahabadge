#include "epaper.h"
#include "fsl_debug_console.h"
#include "fsl_gpio_driver.h"
#include "fsl_spi_master_driver.h"

typedef enum {       /* Image pixel -> Display pixel */
    EPD_compensate,  /* B -> W, W -> B (Current Image) */
    EPD_white,       /* B -> N, W -> W (Current Image) */
    EPD_inverse,     /* B -> N, W -> B (New Image) */
    EPD_normal       /* B -> B, W -> W (New Image) */
} EPD_stage;

static const gpio_output_pin_user_config_t pinDischarge = {
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 19),
    .config.outputLogic = 0,
};

static const gpio_output_pin_user_config_t pinCS = {
    .pinName = GPIO_MAKE_PIN(GPIOD_IDX, 4),
    .config.outputLogic = 1,
};

static const spi_master_user_config_t spiConfig = {
    .bitsPerSec = 20000000, /* 20 MHz, max is 20 MHz */
    .polarity = kSpiClockPolarity_ActiveHigh,
    .phase = kSpiClockPhase_FirstEdge,
    .direction = kSpiMsbFirst,
    .bitCount = kSpi8BitMode,
};

static const uint8_t channel_select[] = {
    0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0xff
};

static const int LINES_PER_DISPLAY = 128;
static const int BYTES_PER_SCAN = 128 / 4 / 2;
static const int BYTES_PER_LINE = 232 / 8;

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
}

static spi_status_t SPI_Transfer(const uint8_t *tx, uint8_t *rx, size_t count)
{
    spi_status_t rc;

    rc = SPI_DRV_MasterTransfer(1, NULL, tx, rx, count);
    if (rc != kStatus_SPI_Success) {
        return rc;
    }
    int i, timeout = (count + 127) / 128 + 1;
    for (i = 0; i < timeout; ++i) {
        rc = SPI_DRV_MasterGetTransferStatus(1, NULL);
        if (rc == kStatus_SPI_Success) {
            return rc;
        }
    }
    return rc;
}

static void EPD_WriteCommandBuffer(uint8_t index, const uint8_t *data,
        size_t length)
{
    uint8_t tx[3] = { 0x70, index, 0x72 };

    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_Transfer(tx, NULL, 2);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_Transfer(&tx[2], NULL, 1);
    SPI_Transfer(data, NULL, length);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
}

static void EPD_WriteCommandByte(uint8_t index, uint8_t data)
{
    EPD_WriteCommandBuffer(index, &data, 1);
}

static uint8_t EPD_ReadCommand(uint8_t index)
{
    uint8_t tx[4] = { 0x70, index, 0x73, 0x00 };
    uint8_t rx[2];

    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_Transfer(tx, NULL, 2);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_Transfer(&tx[2], rx, 2);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    return rx[1];
}

static uint8_t EPD_ReadCogID()
{
    uint8_t tx[2] = { 0x71, 0x00 };
    uint8_t rx[2];

    GPIO_DRV_WritePinOutput(pinCS.pinName, 0);
    SPI_Transfer(tx, rx, 2);
    GPIO_DRV_WritePinOutput(pinCS.pinName, 1);
    return rx[1];
}

static void EPD_Delay(uint32_t ms)
{
    for (; ms > 0; --ms) {
        /* XXX This is really stupid */
        volatile int i;
        for (i = 0; i < 306 * 48; ++i);
    }
}

static void EPD_line(int line, const uint8_t *data, uint8_t fixed_value,
        EPD_stage stage)
{
    size_t len = BYTES_PER_SCAN * 2 + BYTES_PER_LINE * 2 + 1;
    uint8_t buf[len], *p = buf;
    int i;
    for (i = 0; i < BYTES_PER_SCAN; ++i) {
        if (0 != (line & 0x01) && line / 8 == i) {
            *p++ = 0xc0 >> (line & 0x06);
        } else {
            *p++ = 0x00;
        }
    }

    for (i = BYTES_PER_LINE; i > 0; --i) {
        if (data) {
            uint16_t pixels = data[i - 1];
            /* interleave bits with zeros */
            pixels = (pixels | (pixels << 4)) & 0x0f0f;
            pixels = (pixels | (pixels << 2)) & 0x3333;
            pixels = (pixels | (pixels << 1)) & 0x5555;
            switch (stage) {
            case EPD_compensate:
                pixels = 0xaaaa | (pixels ^ 0x5555);
                break;
            case EPD_white:
                pixels = 0x5555 + (pixels ^ 0x5555);
                break;
            case EPD_inverse:
                pixels = 0x5555 | ((pixels ^ 0x5555) << 1);
                break;
            case EPD_normal:
                pixels = 0xaaaa | pixels;
                break;
            }
            *p++ = pixels >> 8;
            *p++ = pixels;
        } else {
            *p++ = fixed_value;
            *p++ = fixed_value;
        }
    }

    for (i = BYTES_PER_SCAN; i > 0; --i) {
        if (0 == (line & 0x01) && line / 8 == i - 1) {
            *p++ = 0x03 << (line & 0x06);
        } else {
            *p++ = 0x00;
        }
    }

    /* border byte */
    if (EPD_normal == stage) {
        *p++ = 0xaa;
    } else {
        *p++ = 0x00;
    }

    EPD_WriteCommandBuffer(0x0a, buf, len);

    /* turn on OE */
    EPD_WriteCommandByte(0x02, 0x07);
}

static void EPD_frame(const uint8_t *data, uint8_t fixed_value, EPD_stage stage)
{
    int i;
    if (data) {
        for (i = 0; i < LINES_PER_DISPLAY; ++i) {
            EPD_line(i, &data[i * BYTES_PER_LINE], 0, stage);
        }
    } else {
        for (i = 0; i < LINES_PER_DISPLAY; ++i) {
            EPD_line(i, NULL, fixed_value, stage);
        }
    }
}

static void EPD_frame_repeat(const uint8_t *data, uint8_t fixed_value,
        EPD_stage stage)
{
    /* TODO this needs to repeat for about 630ms */
    int i;
    for (i = 0; i < 4; ++i) {
        EPD_frame(data, fixed_value, stage);
    }
}

int EPD_Draw(const uint8_t *old_image, const uint8_t *new_image)
{
    int rc = 0;

    /* Configure DMA channel */
    SPI_DRV_MasterInit(1, &spiState);
    uint32_t calculatedBaudRate;
    SPI_DRV_MasterConfigureBus(1, &spiConfig, &calculatedBaudRate);

    /* read the COG ID */
    uint8_t id = EPD_ReadCogID();
    if ((id & 0x0f) != 0x02) {
        rc = -1;
        goto out;
    }

    /* disable OE */
    EPD_WriteCommandByte(0x02, 0x40);

    /* check breakage */
    uint8_t broken_panel = EPD_ReadCommand(0x0f);
    if (0x00 == (0x80 & broken_panel)) {
        rc = -2;
        goto out;
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

    if (4 == i) {
        rc = -3;
        goto out;
    }

    /* output enable to disable */
    EPD_WriteCommandByte(0x02, 0x40);

    /* Draw something */
    /* TODO I'd like to call a callback at this point with a "graphics context"
     * that contains a buffer and the screen coordinates that correspond to
     * the part of the screen to be drawn. That way we can have as little or
     * as much of the image in RAM as we want while drawing. The callback would
     * be called for each portion of the screen we're "caching" in memory while
     * pushing bits to the display.
     */
    EPD_frame_repeat(old_image, 0xff, EPD_compensate);
    EPD_frame_repeat(old_image, 0xff, EPD_white);
    EPD_frame_repeat(new_image, 0xaa, EPD_inverse);
    EPD_frame_repeat(new_image, 0xaa, EPD_normal);

    /* ??? */
    EPD_WriteCommandByte(0x0b, 0x00);

    /* latch reset turn on */
    EPD_WriteCommandByte(0x03, 0x01);

    /* power off charge pump Vcom */
    EPD_WriteCommandByte(0x05, 0x03);

    /* power off charge pump neg voltage */
    EPD_WriteCommandByte(0x05, 0x01);
    EPD_Delay(120);

    /* discharge internal */
    EPD_WriteCommandByte(0x04, 0x80);

    /* power off all charge pumps */
    EPD_WriteCommandByte(0x05, 0x00);

    /* turn off osc */
    EPD_WriteCommandByte(0x07, 0x01);
    EPD_Delay(50);

    /* discharge external */
    GPIO_DRV_WritePinOutput(pinDischarge.pinName, 1);
    EPD_Delay(150);
    GPIO_DRV_WritePinOutput(pinDischarge.pinName, 0);

out:
    SPI_DRV_MasterDeinit(1);
    return rc;
}

/* vim: set expandtab ts=4 sw=4: */
