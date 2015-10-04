/*
 * Copyright (c) 2015 David Barksdale <amatus@amat.us>
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

#include "fsl_clock_manager.h"
#include "fsl_dac_driver.h"
#include "fsl_dma_driver.h"
#include "fsl_gpio_driver.h"
#include "fsl_lptmr_driver.h"
#include "fsl_os_abstraction.h"
#include "fsl_pit_driver.h"
#include "fsl_smc_hal.h"


////////////////////////////
// A bunch of config structs

/* Configuration for enter VLPR mode. Core clock = 2MHz. */
static const clock_manager_user_config_t g_defaultClockConfigVlpr = {   
    .mcgliteConfig =
    {    
        .mcglite_mode       = kMcgliteModeLirc8M,   // Work in LIRC_8M mode.
        .irclkEnable        = true,  // MCGIRCLK enable.
        .irclkEnableInStop  = false, // MCGIRCLK disable in STOP mode.
        .ircs               = kMcgliteLircSel2M, // Select LIRC_2M.
        .fcrdiv             = kMcgliteLircDivBy1,    // FCRDIV is 0.
        .lircDiv2           = kMcgliteLircDivBy1,    // LIRC_DIV2 is 0.
        .hircEnableInNotHircMode         = false, // HIRC disable.
    },
    .simConfig =
    {    
        .er32kSrc  = kClockEr32kSrcOsc0,   // ERCLK32K selection, use OSC.
        .outdiv1   = 0U,
        .outdiv4   = 1U,
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

// LCD backlight GPIO pin
static const gpio_output_pin_user_config_t g_lcdBacklight = {
    .pinName = GPIO_MAKE_PIN(GPIOE_IDX, 31U),
    .config.outputLogic = 1,
    .config.slewRate = kPortSlowSlewRate,
    .config.driveStrength = kPortLowDriveStrength,
};

// LPTMR configurations
static const lptmr_user_config_t g_lptmrConfig = {
    .timerMode = kLptmrTimerModeTimeCounter,
    .freeRunningEnable = false,
    .prescalerEnable = true,
    .prescalerClockSource = kClockLptmrSrcLpoClk,
    .prescalerValue = kLptmrPrescalerDivide2,
    .isInterruptEnabled = true,
};

// PIT config
static const pit_user_config_t g_pitChan0 = {
    .periodUs = 8000,
};


///////
// Code

dma_channel_t g_chan;

/*!
 * @brief LPTMR interrupt call back function.
 * The function is used to toggle LED1.
 */
void lptmr_call_back(void)
{
    // Toggle LED1
    GPIO_DRV_TogglePinOutput(g_lcdBacklight.pinName);

    // Toggle PIT timer
    static bool enable;
    enable = !enable;
    if (enable) {
        DMA_HAL_SetTransferCount(g_dmaBase[0], g_chan.channel, 0xffff0);
        PIT_DRV_StartTimer(0, 0);
    } else {
        PIT_DRV_StopTimer(0, 0);
        DAC_DRV_Output(0, 0);
    }
}

/*!
 * @brief Main function
 */
int main (void)
{
    /* enable clock for PORTs */
    //CLOCK_SYS_EnablePortClock(PORTA_IDX);
    //CLOCK_SYS_EnablePortClock(PORTC_IDX);

    /* Set allowed power mode, allow all. */
    SMC_HAL_SetProtection(SMC, kAllowPowerModeAll);

    /* Set system clock configuration. */
    CLOCK_SYS_SetConfiguration(&g_defaultClockConfigVlpr);

    /* Initialize LPTMR */
    lptmr_state_t lptmrState;
    LPTMR_DRV_Init(LPTMR0_IDX, &lptmrState, &g_lptmrConfig);
    LPTMR_DRV_SetTimerPeriodUs(LPTMR0_IDX, 500000);
    LPTMR_DRV_InstallCallback(LPTMR0_IDX, lptmr_call_back);

    /* Initialize LCD backlight LED GPIO */
    GPIO_DRV_OutputPinInit(&g_lcdBacklight);

    /* Initialize PIT */
    PIT_DRV_Init(0, false);
    PIT_DRV_InitChannel(0, 0, &g_pitChan0);

    /* Initialize the DAC */
    dac_converter_config_t MyDacUserConfigStruct;
    DAC_DRV_StructInitUserConfigNormal(&MyDacUserConfigStruct);
    DAC_DRV_Init(0, &MyDacUserConfigStruct);

    /* Initialize DMA */
    dma_state_t dma_state;
    DMA_DRV_Init(&dma_state);
    DMA_DRV_RequestChannel(kDmaAnyChannel, kDmaRequestMux0AlwaysOn60, &g_chan);
    DMAMUX_HAL_SetPeriodTriggerCmd(g_dmamuxBase[0], g_chan.channel, true);
    const uint32_t dac_dat = (intptr_t)&DAC_DATL_REG(g_dacBase[0], 0);
    const uint16_t L_ON = 0x4ff;
    const uint16_t L_OFF = 0x2ff;
    const uint16_t laserbuf[16] __attribute__ ((aligned(32))) = {
        L_OFF, L_ON,  L_OFF, L_ON,
        L_OFF, L_OFF, L_ON,  L_ON,
        L_OFF, L_OFF, L_OFF, L_ON,
        L_ON,  L_ON,  L_OFF, L_ON,
    };
    DMA_DRV_ConfigTransfer(&g_chan, kDmaMemoryToPeripheral, 2,
            (intptr_t)laserbuf, dac_dat, 0xffff0);
    DMA_HAL_SetSourceModulo(g_dmaBase[0], g_chan.channel, kDmaModulo32Bytes);
    DMA_HAL_SetAsyncDmaRequestCmd(g_dmaBase[0], g_chan.channel, true);
    DMA_HAL_SetIntCmd(g_dmaBase[0], g_chan.channel, false);
    DMA_HAL_SetDisableRequestAfterDoneCmd(g_dmaBase[0], g_chan.channel, false);
    DMA_DRV_StartChannel(&g_chan);

    /* Start LPTMR */
    LPTMR_DRV_Start(LPTMR0_IDX);

    /* We're done, everything else is triggered through interrupts */
    for(;;) {
#ifndef DEBUG
        SMC_HAL_SetMode(SMC, &g_idlePowerMode);
#endif
    }
}

/* vim: set expandtab ts=4 sw=4: */
