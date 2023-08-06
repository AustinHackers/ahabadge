/*
 * Copyright (c) 2015 - 2016 David Barksdale <amatus@amat.us>
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "disk.h"
#include "epaper.h"
#include "flash.h"
#include "lptmr.h"
#include "fsl_clock_manager.h"
#include "fsl_cmp_driver.h"
#include "fsl_dac_driver.h"
#include "fsl_debug_console.h"
#include "fsl_dma_driver.h"
#include "fsl_flexio_driver.h"
#include "fsl_gpio_driver.h"
#include "fsl_lpuart_driver.h"
#include "fsl_os_abstraction.h"
#include "fsl_pit_driver.h"
#include "fsl_smc_hal.h"
#include "fsl_tpm_driver.h"
#include "fsl_rcm_hal.h"

#ifdef BADGE_V1
#warning "Building for hardware v1"
#else
#warning "Building for hardware v2"
#endif

#define USE_HIRC 1

static int current_image = 0;
static volatile int cue_next_image = 0;
static uint32_t laser_pulse_length = 32;


////////////////////////////
// A bunch of config structs

/* Configuration for enter VLPR mode. Core clock = 4MHz. */
static const clock_manager_user_config_t g_defaultClockConfigVlpr = {   
    .mcgliteConfig =
    {    
        .mcglite_mode       = kMcgliteModeLirc8M,   // Work in LIRC_8M mode.
        .irclkEnable        = true,  // MCGIRCLK enable.
        .irclkEnableInStop  = false, // MCGIRCLK disable in STOP mode.
        .ircs               = kMcgliteLircSel8M, // Select LIRC_8M.
        .fcrdiv             = kMcgliteLircDivBy1,    // FCRDIV is 0.
        .lircDiv2           = kMcgliteLircDivBy1,    // LIRC_DIV2 is 0.
        .hircEnableInNotHircMode         = false, // HIRC disable.
    },
    .simConfig =
    {    
        .er32kSrc  = kClockEr32kSrcOsc0,   // ERCLK32K selection, use OSC.
        .outdiv1   = 1U, // divide by 2, core clock = 8 MHz / 2
        .outdiv4   = 3U, // divide by 4, bus clock = 4 MHz / 4
    },
    .oscerConfig =
    {   
        .enable       = false, // OSCERCLK disable.
        .enableInStop = false, // OSCERCLK disable in STOP mode.
    }
};

/* Configuration for enter RUN mode. Core clock = 48MHz. */
static const clock_manager_user_config_t g_defaultClockConfigRun = {
    .mcgliteConfig =
    {   
        .mcglite_mode        = kMcgliteModeHirc48M,   // Work in HIRC mode.
        .irclkEnable        = false, // MCGIRCLK disable.
        .irclkEnableInStop  = false, // MCGIRCLK disable in STOP mode.
        .ircs               = kMcgliteLircSel2M, // Select LIRC_2M.
        .fcrdiv             = kMcgliteLircDivBy1,    // FCRDIV is 0.
        .lircDiv2           = kMcgliteLircDivBy1,    // LIRC_DIV2 is 0.
        .hircEnableInNotHircMode         = true,  // HIRC disable.
    },
    .simConfig =
    {   
        .er32kSrc  = kClockEr32kSrcOsc0,  // ERCLK32K selection, use OSC.
        /* 
         * 1-1 works
         * 1-3 works
         * 2-1 USB almost works
         * 2-3 USB doesn't work
         */
        .outdiv1   = 1U, // 48MHz / 2 = 24MHz
        .outdiv4   = 3U, // 24MHz / 4 = 6MHz
    },
    .oscerConfig =
    {
        .enable       = false, // OSCERCLK disable.
        .enableInStop = false, // OSCERCLK disable in STOP mode.
    }
};

/* Idle the CPU in Very Low Power Wait (VLPW) */
/* This should be the lowest power mode where the PIT still functions. */
static const smc_power_mode_config_t g_idlePowerMode = {
#if USE_HIRC
    .powerModeName = kPowerModeWait,
#else
    .powerModeName = kPowerModeVlpw,
#endif
};

/* Switch GPIO pins */
static const gpio_input_pin_user_config_t g_switch1 = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 1),
#else
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 1),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

static const gpio_input_pin_user_config_t g_switch2 = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 2),
#else
    .pinName = GPIO_MAKE_PIN(GPIOD_IDX, 6),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

static const gpio_input_pin_user_config_t g_switchUp = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 4),
#else
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 5),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

static const gpio_input_pin_user_config_t g_switchDown = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 5),
#else
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 6),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

static const gpio_input_pin_user_config_t g_switchLeft = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 18),
#else
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 3),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

static const gpio_input_pin_user_config_t g_switchRight = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 13),
#else
    .pinName = GPIO_MAKE_PIN(GPIOC_IDX, 4),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

static const gpio_input_pin_user_config_t g_switchSelect = {
#ifdef BADGE_V1
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 12),
#else
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 4),
#endif
    .config.isPullEnable = true,
    .config.pullSelect = kPortPullUp,
    .config.interrupt = kPortIntEitherEdge,
};

/* PIT config */
static const pit_user_config_t g_pitChan0 = {
    .periodUs = 193000,
    .isInterruptEnabled = true,
};

/* CMP config */
static const cmp_comparator_config_t g_cmpConf = {
    .hystersisMode = kCmpHystersisOfLevel0,
    .pinoutEnable = true,
    .pinoutUnfilteredEnable = false,
    .invertEnable = true,
    .highSpeedEnable = false,
    .dmaEnable = false,
    .risingIntEnable = false,
    .fallingIntEnable = false,
#ifdef BADGE_V1
    .plusChnMux = kCmpInputChn5,
#else
    .plusChnMux = kCmpInputChn1,
#endif
    .minusChnMux = kCmpInputChnDac,
    .triggerEnable = false,
};

/* CMP DAC config */
static cmp_dac_config_t g_cmpDacConf = {
    .dacEnable = true,
    .refVoltSrcMode = kCmpDacRefVoltSrcOf2,
    .dacValue = 32,
};

/* Laser LPUART config */
static lpuart_user_config_t g_lpuartConfig = {
#if USE_HIRC
    .clockSource = kClockLpuartSrcIrc48M,
#else
    .clockSource = kClockLpuartSrcMcgIrClk,
#endif
    .baudRate = 9600,
    .parityMode = kLpuartParityEven,
    .stopBitCount = kLpuartOneStopBit,
    .bitCountPerChar = kLpuart9BitsPerChar,
};

/* FlexIO config for RGB LED */
static flexio_user_config_t g_flexioConfig = {
    .useInt = false,
    .onDozeEnable = true,
    .onDebugEnable = true,
    .fastAccessEnable = true,
};

/*
 * Run FLEXIO at 4MHz and clock out a bit every 0.25 us.
 * We use 5 bits for each NZR symbol.
 * For an NZR zero we clock out one 1 bit (0.25 us) and four 0 bits (1 us).
 * For an NZR one we clock out four 1 bits (1 us) and one 0 bit (0.25 us).
 * This is within the WS2812B spec for "short" time of 0.2 to 0.5 us and
 * "long" time of 0.75 to 1.05 us.
 */
#if USE_HIRC
static const int flexio_clk_div = 12;
#else
static const int flexio_clk_div = 2;
#endif

static flexio_timer_config_t g_timerConfig = {
    .trgsel = FLEXIO_HAL_TIMER_TRIGGER_SEL_SHIFTnSTAT(0),
    .trgpol = kFlexioTimerTriggerPolarityActiveLow,
    .trgsrc = kFlexioTimerTriggerSourceInternal,
    .pincfg = kFlexioPinConfigOutputDisabled,
    .timod = kFlexioTimerModeDual8BitBaudBit,
    .timout = kFlexioTimerOutputOneNotAffectedByReset,
    .timdec = kFlexioTimerDecSrcOnFlexIOClockShiftTimerOutput,
    .timrst = kFlexioTimerResetNever,
    .timdis = kFlexioTimerDisableOnTimerCompare,
    .timena = kFlexioTimerEnableOnTriggerHigh,
    .tstop = kFlexioTimerStopBitDisabled,
    .tstart = kFlexioTimerStartBitDisabled,
    .timcmp = (30 * 2 - 1) << 8 | (flexio_clk_div / 2 - 1), // 30 bits at 4 Mbps
};

static flexio_shifter_config_t g_shifterConfig = {
    .timsel = 0,
    .timpol = kFlexioShifterTimerPolarityOnPositive,
    .pincfg = kFlexioPinConfigOutput,
    .pinsel = 4,
    .pinpol = kFlexioPinActiveHigh,
    .smode = kFlexioShifterModeTransmit,
    .sstop = kFlexioShifterStopBitDisable,
    .sstart = kFlexioShifterStartBitDisabledLoadDataOnEnable,
};

#ifdef BADGE_V1
static const int g_buzzer_tpm_ch = 3;
static const int g_laser_LPUART_UNIT = 1;
#else
static const int g_buzzer_tpm_ch = 1;
static const int g_laser_LPUART_UNIT = 0;
#endif


///////
// Code

static cmp_state_t g_cmpState;
static dma_channel_t g_fioChan;
static lpuart_state_t g_lpuartState;
static uint8_t rxBuff[1];
static uint8_t txBuff[] = { 'R' };
static uint8_t laser_on;
static uint8_t seizure_on = 1;
static uint32_t shift0_buf[4];
static uint32_t blank_led;
static uint8_t image0[EPD_W * EPD_H / 8];
static uint8_t image1[EPD_W * EPD_H / 8];

int read_sun_raster(uint8_t *in, uint8_t *out)
{
    struct {
        uint32_t magic;
        uint32_t width;
        uint32_t height;
        uint32_t depth;
    } *hdr = (void *)in;
    if (hdr->magic != __builtin_bswap32(0x59a66a95))
        return -1;
    if (hdr->depth != __builtin_bswap32(1))
        return -2;
    uint32_t w = __builtin_bswap32(hdr->width);
    uint32_t stride = (w + 15) / 16 * 16;
    if (w > EPD_W)
        w = EPD_W;
    uint32_t h = __builtin_bswap32(hdr->height);
    if (h > EPD_H)
        h = EPD_H;
    in += 32; /* skip header */
    for (uint32_t y = 0; y < h; ++y) {
        for (uint32_t x = 0; x < w; x += 8) {
            uint8_t v = in[(y * stride + x) / 8];
            v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
            v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
            v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
            out[(y * w + x) / 8] = v;
        }
    }
    return 0;
}

void led(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t color = ((uint32_t)green << 16) | ((uint32_t)red << 8) | blue;
    int i;
    for (i = 0; i < 24; ++i) {
        shift0_buf[i / 6] <<= 5;
        if ((color >> (23 - i)) & 1)
            shift0_buf[i / 6] |= 0x1e;
        else
            shift0_buf[i / 6] |= 0x10;
    }
    // the high 30 bits of each "word" are clocked out
    shift0_buf[0] <<= 2;
    shift0_buf[1] <<= 2;
    shift0_buf[2] <<= 2;
    shift0_buf[3] <<= 2;

    DMA_DRV_ConfigTransfer(&g_fioChan,
            kDmaMemoryToPeripheral,
            sizeof(shift0_buf[0]),
            (intptr_t)&shift0_buf,
            (intptr_t)&FLEXIO_SHIFTBUFBIS_REG(FLEXIO, 0),
            sizeof(shift0_buf));
    DMA_DRV_StartChannel(&g_fioChan);
}

/*!
 * @brief LPTMR interrupt call back function.
 */
static void lptmr_call_back(void)
{
    /* AGC adjust */
    if (CMP_DRV_GetOutputLogic(0) != g_cmpConf.invertEnable) {
        if (g_cmpDacConf.dacValue < 63) {
            g_cmpDacConf.dacValue++;
            CMP_DRV_ConfigDacChn(0, &g_cmpDacConf);
        }
    } else {
        if (g_cmpDacConf.dacValue > 0) {
            g_cmpDacConf.dacValue--;
            CMP_DRV_ConfigDacChn(0, &g_cmpDacConf);
        }
    }

    /* FIRE THE LASER */
    if (laser_on) {
        if (seizure_on) {
            static int foo = 0;
            static char colors[] = { 'R', 'G', 'B' };
            txBuff[0] = colors[foo];
            foo = (foo + 1) % 3;
        }
        LPUART_DRV_SendData(g_laser_LPUART_UNIT, txBuff, laser_pulse_length);
    }

    /* countdown to turn off LED */
    if (blank_led) {
        --blank_led;
        if (blank_led == 0) {
            led(0, 0, 0);
        }
    }
}

/* Play music using the PIT timer */
uint16_t notes[] = {
    0,   // rest
    494, // B4  1
    523, // C5  2
    554, // C#5 3
    587, // D5  4
    622, // D#5 5
    659, // E5  6
    698, // F5  7
    740, // F#5 8
    784, // G5  9
    830, // G#5 10
    880, // A5  11
    932, // A#5 12
    988, // B5  13
};
uint8_t beeps[] = {
     8,  8,  4,  1,
     0,  1,  0,  6,
     0,  6,  0,  6,
    10, 10, 11, 13,
    11, 11, 11,  6,
     0,  4,  0,  8,
     0,  8,  0,  8,
     6,  6,  8,  6,
};

tpm_pwm_param_t param = {
    .mode = kTpmEdgeAlignedPWM,
    .edgeMode = kTpmHighTrue,
    .uDutyCyclePercent = 50,
};

int position;

void PIT_IRQHandler(void)
{
    if (PIT_HAL_IsIntPending(g_pitBase[0], 0))
    {
        PIT_HAL_ClearIntFlag(g_pitBase[0], 0);
        param.uFrequencyHZ = notes[beeps[position]];
        position = (position + 1) % sizeof(beeps);
        if (param.uFrequencyHZ)
            TPM_DRV_PwmStart(0, &param, g_buzzer_tpm_ch);
        else
            TPM_DRV_PwmStop(0, &param, g_buzzer_tpm_ch);
    }
    if (PIT_HAL_IsIntPending(g_pitBase[0], 1))
    {
        PIT_HAL_ClearIntFlag(g_pitBase[0], 1);
    }
}

static void GPIO_transition_handler()
{
#ifdef DEBUG
    debug_printf("Switches: %c%c%c%c%c%c%c\r\n",
            GPIO_DRV_ReadPinInput(g_switch1.pinName) ? ' ' : '1',
            GPIO_DRV_ReadPinInput(g_switch2.pinName) ? ' ' : '2',
            GPIO_DRV_ReadPinInput(g_switchUp.pinName) ? ' ' : 'U',
            GPIO_DRV_ReadPinInput(g_switchDown.pinName) ? ' ' : 'D',
            GPIO_DRV_ReadPinInput(g_switchLeft.pinName) ? ' ' : 'L',
            GPIO_DRV_ReadPinInput(g_switchRight.pinName) ? ' ' : 'R',
            GPIO_DRV_ReadPinInput(g_switchSelect.pinName) ? ' ' : 'S');
#endif
    if (GPIO_DRV_ReadPinInput(g_switch1.pinName)) {
        TPM_DRV_PwmStop(0, &param, g_buzzer_tpm_ch);
        PIT_DRV_StopTimer(0, 0);
    } else {
        position = 0;
        PIT_DRV_StartTimer(0, 0);
    }

    if (GPIO_DRV_ReadPinInput(g_switch2.pinName)) {
        LPUART_DRV_AbortSendingData(g_laser_LPUART_UNIT);
        laser_on = 0;
    } else {
        laser_on = 1;
    }

    if (!GPIO_DRV_ReadPinInput(g_switchUp.pinName)) {
        txBuff[0] = 'R';
        seizure_on = 0;
    }

    if (!GPIO_DRV_ReadPinInput(g_switchLeft.pinName)) {
        txBuff[0] = 'G';
        seizure_on = 0;
    }

    if (!GPIO_DRV_ReadPinInput(g_switchRight.pinName)) {
        txBuff[0] = 'B';
        seizure_on = 0;
    }

    if (!GPIO_DRV_ReadPinInput(g_switchDown.pinName)) {
        txBuff[0] = 'T';
        seizure_on = 0;
    }

    if (!GPIO_DRV_ReadPinInput(g_switchSelect.pinName)) {
        cue_next_image = 1;
    }
}

void PORTA_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTA_BASE_PTR);

    GPIO_transition_handler();
}

void PORTCD_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTC_BASE_PTR);
    PORT_HAL_ClearPortIntFlag(PORTD_BASE_PTR);

    GPIO_transition_handler();
}

static void lpuartTxCallback(uint32_t instance, void *state)
{
    lpuart_state_t *lpuartState = state;

    /* Decrement txSize so the transfer eventually stops */
    --lpuartState->txSize;
}

static void lpuartRxCallback(uint32_t instance, void *lpuartState)
{
    LPUART_Type *base = g_lpuartBase[instance];
    uint32_t stat = LPUART_RD_STAT(base);
    bool noise = stat & LPUART_STAT_NF_MASK;
    bool frame_error = stat & LPUART_STAT_FE_MASK;
    bool parity_error = stat & LPUART_STAT_PF_MASK;
    LPUART_WR_STAT(base, (stat & 0x3e000000) |
            LPUART_STAT_NF_MASK | LPUART_STAT_FE_MASK | LPUART_STAT_PF_MASK);
    if (rxBuff[0] == 'R') {
        seizure_on = 0;
        txBuff[0] = rxBuff[0];
        led(0xff, 0x00, 0x00);
        blank_led = 30;
        return;
    }
    if (rxBuff[0] == 'G') {
        seizure_on = 0;
        txBuff[0] = rxBuff[0];
        led(0x00, 0xff, 0x00);
        blank_led = 30;
        return;
    }
    if (rxBuff[0] == 'B') {
        seizure_on = 0;
        txBuff[0] = rxBuff[0];
        led(0x00, 0x00, 0xff);
        blank_led = 30;
        return;
    }
    if (rxBuff[0] == 'T') {
        position = 0;
        PIT_DRV_StartTimer(0, 0);
        return;
    }
    return;
    if (noise) {
        led(0xff, 0xff, 0x00);
        return;
    }
    if (frame_error) {
        led(0xff, 0x00, 0xff);
        return;
    }
    if (parity_error) {
        led(0x00, 0xff, 0xff);
        return;
    }
    led(0x00, 0x00, 0x00);
}

static void fioDmaCallback(void *param, dma_channel_status_t status)
{
    DMA_DRV_StopChannel(&g_fioChan);
}

void HardFault_Handler(void)
{
    //debug_printf("Hard Fault!\r\n");
    for(;;);
}

/*!
 * @brief Main function
 */
int main (void)
{
    /* enable clock for PORTs */
    // XXX GPIO_DRV_InputPinInit() already does this!
    CLOCK_SYS_EnablePortClock(PORTA_IDX);
    CLOCK_SYS_EnablePortClock(PORTC_IDX);
    CLOCK_SYS_EnablePortClock(PORTD_IDX);
#ifdef BADGE_V1
    CLOCK_SYS_EnablePortClock(PORTB_IDX);
    CLOCK_SYS_EnablePortClock(PORTE_IDX);
#endif

    /* Set allowed power mode, allow all. */
    SMC_HAL_SetProtection(SMC, kAllowPowerModeAll);

    /* Set system clock configuration. */
#if USE_HIRC
    CLOCK_SYS_SetConfiguration(&g_defaultClockConfigRun);
#else
    CLOCK_SYS_SetConfiguration(&g_defaultClockConfigVlpr);
#endif

    /* Setup Debug console */
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(PORTB, 17, kPortMuxAlt3);
#if USE_HIRC
    CLOCK_SYS_SetLpuartSrc(0, kClockLpuartSrcIrc48M);
#else
    CLOCK_SYS_SetLpuartSrc(0, kClockLpuartSrcMcgIrClk);
#endif
    DbgConsole_Init(0, 9600, kDebugConsoleLPUART);
#else
    PORT_HAL_SetMuxMode(PORTD, 5, kPortMuxAlt3);
    DbgConsole_Init(2, 9600, kDebugConsoleUART);
#endif
#ifdef DEBUG
    debug_printf("Debug console initialized\r\n");
    debug_printf("Reset src = 0x%x\r\n", RCM_HAL_GetSrcStatus(RCM, kRcmSrcAll));
#endif

    /* Initialize LPTMR */
    lptmr_init(lptmr_call_back);

    /* Initialize DMA */
    dma_state_t dma_state;
    DMA_DRV_Init(&dma_state);

    /* Initialize PIT */
    PIT_DRV_Init(0, false);
    PIT_DRV_InitChannel(0, 0, &g_pitChan0);

    /* Initialize CMP */
    CMP_DRV_Init(0, &g_cmpState, &g_cmpConf);
    CMP_DRV_ConfigDacChn(0, &g_cmpDacConf);
    CMP_DRV_Start(0);

    /* Buttons */
    GPIO_DRV_InputPinInit(&g_switch1);
    GPIO_DRV_InputPinInit(&g_switch2);
    GPIO_DRV_InputPinInit(&g_switchUp);
    GPIO_DRV_InputPinInit(&g_switchDown);
    GPIO_DRV_InputPinInit(&g_switchLeft);
    GPIO_DRV_InputPinInit(&g_switchRight);
    GPIO_DRV_InputPinInit(&g_switchSelect);

    /* Start LPTMR */
    lptmr_start();

    /* Setup Laser UART */
    LPUART_DRV_Init(g_laser_LPUART_UNIT, &g_lpuartState, &g_lpuartConfig);
    LPUART_DRV_InstallRxCallback(g_laser_LPUART_UNIT, lpuartRxCallback, rxBuff, NULL, true);
    LPUART_DRV_InstallTxCallback(g_laser_LPUART_UNIT, lpuartTxCallback, NULL, NULL);
    LPUART_BWR_CTRL_TXINV(g_lpuartBase[g_laser_LPUART_UNIT], 1);
    SIM_HAL_SetLpuartRxSrcMode(SIM, g_laser_LPUART_UNIT, kSimLpuartRxsrcCmp0);
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(PORTE, 0, kPortMuxAlt3);
#else
    PORT_HAL_SetMuxMode(PORTA, 2, kPortMuxAlt2);
#endif

    /* Setup FlexIO for the WS2812B */
#if USE_HIRC
    CLOCK_SYS_SetFlexioSrc(0, kClockFlexioSrcIrc48M);
#else
    CLOCK_SYS_SetFlexioSrc(0, kClockFlexioSrcMcgIrClk);
#endif
    FLEXIO_DRV_Init(0, &g_flexioConfig);
    FLEXIO_HAL_ConfigureTimer(FLEXIO, 0, &g_timerConfig);
    FLEXIO_HAL_ConfigureShifter(FLEXIO, 0, &g_shifterConfig);
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(PORTE, 20, kPortMuxAlt6);
#else
    PORT_HAL_SetMuxMode(PORTD, 4, kPortMuxAlt6);
#endif
    FLEXIO_DRV_Start(0);

    FLEXIO_HAL_SetShifterStatusDmaCmd(FLEXIO, 1, true);
    DMA_DRV_RequestChannel(kDmaAnyChannel, kDmaRequestMux0FlexIOChannel0,
            &g_fioChan);
    DMA_DRV_RegisterCallback(&g_fioChan, fioDmaCallback, NULL);

    /* Config buzzer on TPM0 */
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(PORTE, 30, kPortMuxAlt3);
#else
    PORT_HAL_SetMuxMode(PORTC, 2, kPortMuxAlt4);
#endif
    tpm_general_config_t tmpConfig = {
        .isDBGMode = false,
        .isGlobalTimeBase = false,
        .isTriggerMode = false,
        .isStopCountOnOveflow = false,
        .isCountReloadOnTrig = false,
        .triggerSource = kTpmTrigSel0,
    };
    TPM_DRV_Init(0, &tmpConfig);
#if USE_HIRC
    TPM_DRV_SetClock(0, kTpmClockSourceModuleHighFreq, kTpmDividedBy8);
#else
    TPM_DRV_SetClock(0, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);
#endif

    /* Blank LED just in case, saves power */
    led(0x00, 0x00, 0x00);

    /* Initialize flash stuff */
    flash_init();

    /* Init e-paper display */
    EPD_Init();

    /* Throw up first image */
    int ret = read_sun_raster(images[current_image], image0);
    debug_printf("read_sun_raster returned %d\r\n", ret);
    if (0 == ret) {
        ret = EPD_Draw(NULL, image0);
        debug_printf("EPD_Draw returned %d\r\n", ret);
    }
    if (-1 == ret) {
        led(0xff, 0x00, 0x00);
    } else if (-2 == ret) {
        led(0xff, 0xff, 0x00);
    } else if (-3 == ret) {
        led(0x00, 0x00, 0xff);
    } else {
        led(0x00, 0xff, 0x00);
    }
    blank_led = 30;

#if USE_HIRC
    /* No good can come of this */
    disk_init();
#endif

    /* We're done, everything else is triggered through interrupts */
    for(;;) {
        if (cue_next_image) {
            cue_next_image = 0;
            int old_image = current_image;
            ret = read_sun_raster(images[old_image], image0);
            if (0 == ret) {
                do {
                    current_image = (current_image + 1) % IMAGE_COUNT;
                    ret = read_sun_raster(images[current_image], image1);
                } while (ret != 0 && current_image != old_image);
                debug_printf("drawing %d -> %d\r\n", old_image, current_image);
                EPD_Draw(image0, image1);
            }
        }
#if !USE_HIRC
        SMC_HAL_SetMode(SMC, &g_idlePowerMode);
#else
        /* This doesn't seem to save any power in HIRC and instead messes up
         * FLEXIO */
        //SMC_HAL_SetMode(SMC, &g_idlePowerMode);
#endif
    }
}

/* vim: set expandtab ts=4 sw=4: */
