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
#include "fsl_clock_manager.h"
#include "fsl_cmp_driver.h"
#include "fsl_dac_driver.h"
#include "fsl_debug_console.h"
#include "fsl_dma_driver.h"
#include "fsl_flexio_driver.h"
#include "fsl_gpio_driver.h"
#include "fsl_lptmr_driver.h"
#include "fsl_lpuart_driver.h"
#include "fsl_os_abstraction.h"
#include "fsl_pit_driver.h"
#include "fsl_smc_hal.h"
#include "fsl_tpm_driver.h"
#include "radio.h"
#include "text.h"


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
        .outdiv1   = 0U,
        .outdiv4   = 1U,
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
    .powerModeName = kPowerModeVlpw,
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

/* LPTMR configurations */
static const lptmr_user_config_t g_lptmrConfig = {
    .timerMode = kLptmrTimerModeTimeCounter,
    .freeRunningEnable = false,
    .prescalerEnable = true,
    .prescalerClockSource = kClockLptmrSrcLpoClk,
    .prescalerValue = kLptmrPrescalerDivide2,
    .isInterruptEnabled = true,
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

/* LPUART0 config */
static lpuart_user_config_t g_lpuartConfig = {
    .clockSource = kClockLpuartSrcIrc48M,
    .baudRate = 9600,
    .parityMode = kLpuartParityEven,
    .stopBitCount = kLpuartOneStopBit,
    .bitCountPerChar = kLpuart9BitsPerChar,
};

/* FlexIO config for RGB LED */
static flexio_user_config_t g_flexioConfig = {
    .useInt = false,
    .onDozeEnable = true,
    .onDebugEnable = false,
    .fastAccessEnable = true,
};

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
    .timcmp = (32 * 2 - 1) << 8 | (10 - 1), // 32 bits at 2.4 MHz
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
static const g_buzzer_tpm_ch = 3;
#else
static const g_buzzer_tmp_ch = 1;
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
static uint32_t shift0_buf[3];
static uint32_t blank_led;
static uint8_t image0[232 * 128 / 8];
static uint8_t image1[232 * 128 / 8];

void read_sun_raster(uint8_t *in, uint8_t *out)
{
    int x, y;
    int stride = (232 + 15) / 16 * 16;

    in += 32; /* skip header, don't even check it */
    for (y = 0; y < 128; ++y) {
        for (x = 0; x < 232; x += 8) {
            uint8_t v = in[(y * stride + x) / 8];
            v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
            v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
            v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
            out[(y * 232 + x) / 8] = v;
        }
    }
}

void led(uint8_t red, uint8_t green, uint8_t blue)
{
    FLEXIO_Type *fiobase = g_flexioBase[0];
    uint32_t color = (green << 16) | (red << 8) | blue;
    int i;
    for (i = 0; i < 24; ++i) {
        shift0_buf[i / 10] <<= 3;
        shift0_buf[i / 10] |= 4 | ((color >> (22 - i)) & 2);
    }
    shift0_buf[2] <<= 3 * 6;

    DMA_DRV_ConfigTransfer(&g_fioChan, kDmaMemoryToPeripheral, 4,
            (intptr_t)&shift0_buf,
            (intptr_t)&FLEXIO_SHIFTBUFBIS_REG(fiobase, 0), sizeof(shift0_buf));
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
        LPUART_DRV_SendData(1, txBuff, laser_pulse_length);
    }

    /* countdown to turn off LED */
    if (blank_led) {
        if (blank_led == 1) {
            led(0, 0, 0);
        }
        blank_led--;
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

static void GPIO_transition_handler(const int port)
{
    if (GPIO_EXTRACT_PORT(g_switch1.pinName) == port) {
        if (GPIO_DRV_ReadPinInput(g_switch1.pinName)) {
            TPM_DRV_PwmStop(0, &param, g_buzzer_tpm_ch);
            PIT_DRV_StopTimer(0, 0);
        } else {
            position = 0;
            PIT_DRV_StartTimer(0, 0);
        }
    }

    if (GPIO_EXTRACT_PORT(g_switch2.pinName) == port) {
        if (GPIO_DRV_ReadPinInput(g_switch2.pinName)) {
            LPUART_DRV_AbortSendingData(1);
            laser_on = 0;
        } else {
            laser_on = 1;
        }
    }

    if (GPIO_EXTRACT_PORT(g_switchUp.pinName) == port) {
        if (!GPIO_DRV_ReadPinInput(g_switchUp.pinName)) {
            txBuff[0] = 'R';
            seizure_on = 0;
        }
    }

    if (GPIO_EXTRACT_PORT(g_switchLeft.pinName) == port) {
        if (!GPIO_DRV_ReadPinInput(g_switchLeft.pinName)) {
            txBuff[0] = 'G';
            seizure_on = 0;
        }
    }

    if (GPIO_EXTRACT_PORT(g_switchRight.pinName) == port) {
        if (!GPIO_DRV_ReadPinInput(g_switchRight.pinName)) {
            txBuff[0] = 'B';
            seizure_on = 0;
        }
    }

    if (GPIO_EXTRACT_PORT(g_switchDown.pinName) == port) {
        if (!GPIO_DRV_ReadPinInput(g_switchDown.pinName)) {
            txBuff[0] = 'T';
            seizure_on = 0;
        }
    }

    if (GPIO_EXTRACT_PORT(g_switchSelect.pinName) == port) {
        if (!GPIO_DRV_ReadPinInput(g_switchSelect.pinName)) {
            cue_next_image = 1;
        }
    }
}

void PORTA_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTA_BASE_PTR);

    GPIO_transition_handler(GPIOA_IDX);
}

void PORTC_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTC_BASE_PTR);

    GPIO_transition_handler(GPIOC_IDX);
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
        led(0xff, 0x00, 0x00);
        blank_led = 30;
        return;
    }
    if (rxBuff[0] == 'G') {
        led(0x00, 0xff, 0x00);
        blank_led = 30;
        return;
    }
    if (rxBuff[0] == 'B') {
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

/*!
 * @brief Main function
 */
int main (void)
{
    /* enable clock for PORTs */
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
    CLOCK_SYS_SetConfiguration(&g_defaultClockConfigRun);

    /* Break everything */
    OSA_Init();

    /* Setup Debug console on LPUART0 on PTB17 */
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(PORTB, 17, kPortMuxAlt3);
    CLOCK_SYS_SetLpuartSrc(0, kClockLpuartSrcIrc48M);
    DbgConsole_Init(0, 9600, kDebugConsoleLPUART);
#else
    PORT_HAL_SetMuxMode(PORTD, 5, kPortMuxAlt3);
    CLOCK_SYS_SetLpuartSrc(0, kClockLpuartSrcIrc48M);
    DbgConsole_Init(2, 9600, kDebugConsoleUART);
#endif

    /* Initialize LPTMR */
    lptmr_state_t lptmrState;
    LPTMR_DRV_Init(LPTMR0_IDX, &lptmrState, &g_lptmrConfig);
    LPTMR_DRV_SetTimerPeriodUs(LPTMR0_IDX, 100000);
    LPTMR_DRV_InstallCallback(LPTMR0_IDX, lptmr_call_back);

    /* Initialize DMA */
    dma_state_t dma_state;
    DMA_DRV_Init(&dma_state);

    /* Initialize PIT */
    PIT_DRV_Init(0, false);
    PIT_DRV_InitChannel(0, 0, &g_pitChan0);

    /* Initialize CMP */
    CMP_DRV_Init(0, &g_cmpState, &g_cmpConf);
    CMP_DRV_ConfigDacChn(0, &g_cmpDacConf);
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 0, kPortMuxAlt5);
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
    LPTMR_DRV_Start(LPTMR0_IDX);

    /* Setup LPUART1 */
#ifdef BADGE_V1
    LPUART_DRV_Init(1, &g_lpuartState, &g_lpuartConfig);
    LPUART_DRV_InstallRxCallback(1, lpuartRxCallback, rxBuff, NULL, true);
    LPUART_DRV_InstallTxCallback(1, lpuartTxCallback, NULL, NULL);
    LPUART_BWR_CTRL_TXINV(g_lpuartBase[1], 1);
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 0, kPortMuxAlt3);
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 1, kPortMuxAlt3);
#else
    LPUART_DRV_Init(0, &g_lpuartState, &g_lpuartConfig);
    LPUART_DRV_InstallRxCallback(0, lpuartRxCallback, rxBuff, NULL, true);
    LPUART_DRV_InstallTxCallback(0, lpuartTxCallback, NULL, NULL);
    LPUART_BWR_CTRL_TXINV(g_lpuartBase[0], 1);
    PORT_HAL_SetMuxMode(g_portBase[GPIOA_IDX], 1, kPortMuxAlt2);
    PORT_HAL_SetMuxMode(g_portBase[GPIOA_IDX], 2, kPortMuxAlt2);
#endif

    /* Setup FlexIO for the WS2812B */
    FLEXIO_Type *fiobase = g_flexioBase[0];
    CLOCK_SYS_SetFlexioSrc(0, kClockFlexioSrcIrc48M);
    FLEXIO_DRV_Init(0, &g_flexioConfig);
    FLEXIO_HAL_ConfigureTimer(fiobase, 0, &g_timerConfig);
    FLEXIO_HAL_ConfigureShifter(fiobase, 0, &g_shifterConfig);
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 20, kPortMuxAlt6);
#else
    PORT_HAL_SetMuxMode(g_portBase[GPIOD_IDX], 4, kPortMuxAlt6);
#endif
    FLEXIO_DRV_Start(0);

    FLEXIO_HAL_SetShifterStatusDmaCmd(fiobase, 1, true);
    DMA_DRV_RequestChannel(kDmaAnyChannel, kDmaRequestMux0FlexIOChannel0,
            &g_fioChan);
    DMA_DRV_RegisterCallback(&g_fioChan, fioDmaCallback, NULL);

    /* Config buzzer on TPM0 */
#ifdef BADGE_V1
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 30, kPortMuxAlt3);
#else
    PORT_HAL_SetMuxMode(g_portBase[GPIOC_IDX], 2, kPortMuxAlt4);
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
    TPM_DRV_SetClock(0, kTpmClockSourceModuleHighFreq, kTpmDividedBy8);

    /* Blank LED just in case, saves power */
    led(0x00, 0x00, 0x00);

    /* Initialize flash stuff */
    flash_init();

    /* Init e-paper display */
    EPD_Init();

    /* Throw up first image */
    read_sun_raster(images[current_image], image0);
    int ret = EPD_Draw(NULL, image0);
    debug_printf("EPD_Draw returned %d\r\n", ret);
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

#if 0
    ret = radio_init();
    debug_printf("radio_init returned %d\r\n", ret);
    if (0 == ret) {
        led(0x22, 0x00, 0x22);
    }
#endif

    /* No good can come of this */
    disk_init();

    /* We're done, everything else is triggered through interrupts */
    for(;;) {
        if (cue_next_image) {
            int old_image = current_image;
            current_image = (current_image + 1) % IMAGE_COUNT;
            debug_printf("drawing %d -> %d\r\n", old_image, current_image);
            read_sun_raster(images[old_image], image0);
            read_sun_raster(images[current_image], image1);
            EPD_Draw(image0, image1);
            cue_next_image = 0;
        }
#ifndef DEBUG
        //SMC_HAL_SetMode(SMC, &g_idlePowerMode);
#endif
    }
}

/* vim: set expandtab ts=4 sw=4: */
