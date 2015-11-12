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
#include "fsl_cmp_driver.h"
#include "fsl_dac_driver.h"
#include "fsl_dma_driver.h"
#include "fsl_flexio_driver.h"
#include "fsl_gpio_driver.h"
#include "fsl_lptmr_driver.h"
#include "fsl_lpuart_driver.h"
#include "fsl_os_abstraction.h"
#include "fsl_pit_driver.h"
#include "fsl_smc_hal.h"
#include "fsl_spi_master_driver.h"


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

/* Switch1 GPIO pin */
static const gpio_input_pin_user_config_t g_switch1 = {
    .pinName = GPIO_MAKE_PIN(GPIOA_IDX, 1),
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
    .periodUs = 104, // 9615 Hz (9600 baud 0.16% error)
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
    .plusChnMux = kCmpInputChn5,
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
    .clockSource = kClockLpuartSrcMcgIrClk,
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
    .timcmp = (32 * 2 - 1) << 8 | (2 - 1), // 32 bits at 2 MHz
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

///////
// Code

static cmp_state_t g_cmpState;
static dma_channel_t g_dacChan, g_fioChan;
static lpuart_state_t g_lpuartState;
static uint8_t rxBuff[1];
static uint32_t shift0_buf[3];

/*!
 * @brief LPTMR interrupt call back function.
 * The function is used to toggle LED1.
 */
static void lptmr_call_back(void)
{
    // AGC adjust
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
}

void PORTA_IRQHandler(void)
{
    /* Clear interrupt flag.*/
    PORT_HAL_ClearPortIntFlag(PORTA_BASE_PTR);

    if (GPIO_DRV_ReadPinInput(g_switch1.pinName)) {
        PIT_DRV_StopTimer(0, 0);
        DAC_DRV_Output(0, 0);
    } else {
        DMA_HAL_SetTransferCount(g_dmaBase[0], g_dacChan.channel, 0xffff0);
        PIT_DRV_StartTimer(0, 0);
    }
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
    //CLOCK_SYS_EnablePortClock(PORTB_IDX);
    //CLOCK_SYS_EnablePortClock(PORTC_IDX);
    CLOCK_SYS_EnablePortClock(PORTD_IDX);
    CLOCK_SYS_EnablePortClock(PORTE_IDX);

    /* Set allowed power mode, allow all. */
    SMC_HAL_SetProtection(SMC, kAllowPowerModeAll);

    /* Set system clock configuration. */
    CLOCK_SYS_SetConfiguration(&g_defaultClockConfigVlpr);

    /* Initialize LPTMR */
    lptmr_state_t lptmrState;
    LPTMR_DRV_Init(LPTMR0_IDX, &lptmrState, &g_lptmrConfig);
    LPTMR_DRV_SetTimerPeriodUs(LPTMR0_IDX, 100000);
    LPTMR_DRV_InstallCallback(LPTMR0_IDX, lptmr_call_back);

    /* Initialize PIT */
    PIT_DRV_Init(0, false);
    PIT_DRV_InitChannel(0, 0, &g_pitChan0);

    /* Initialize the DAC */
    dac_converter_config_t MyDacUserConfigStruct;
    DAC_DRV_StructInitUserConfigNormal(&MyDacUserConfigStruct);
    DAC_DRV_Init(0, &MyDacUserConfigStruct);

    /* Initialize DAC DMA channel */
    dma_state_t dma_state;
    DMA_DRV_Init(&dma_state);
    DMA_DRV_RequestChannel(kDmaAnyChannel, kDmaRequestMux0AlwaysOn60,
            &g_dacChan);
    DMAMUX_HAL_SetPeriodTriggerCmd(g_dmamuxBase[0], g_dacChan.channel, true);
    const uint32_t dac_dat = (intptr_t)&DAC_DATL_REG(g_dacBase[0], 0);
    const uint16_t L_ON = 0x4ff;
    const uint16_t L_OFF = 0x2ff;
    const uint16_t laserbuf[16] __attribute__ ((aligned(32))) = {
        L_ON,  L_ON,  L_ON,  L_OFF,
        L_ON,  L_ON,  L_OFF, L_ON,
        L_ON,  L_ON,  L_OFF, L_OFF,
        L_OFF, L_OFF, L_OFF, L_OFF,
    };
    DMA_DRV_ConfigTransfer(&g_dacChan, kDmaMemoryToPeripheral, 2,
            (intptr_t)laserbuf, dac_dat, 0xffff0);
    DMA_HAL_SetSourceModulo(g_dmaBase[0], g_dacChan.channel, kDmaModulo32Bytes);
    DMA_HAL_SetAsyncDmaRequestCmd(g_dmaBase[0], g_dacChan.channel, true);
    DMA_HAL_SetIntCmd(g_dmaBase[0], g_dacChan.channel, false);
    DMA_HAL_SetDisableRequestAfterDoneCmd(g_dmaBase[0], g_dacChan.channel,
            false);
    DMA_DRV_StartChannel(&g_dacChan);
    DAC_DRV_Output(0, 0);

    /* Initialize CMP */
    CMP_DRV_Init(0, &g_cmpState, &g_cmpConf);
    CMP_DRV_ConfigDacChn(0, &g_cmpDacConf);
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 0, kPortMuxAlt5);
    CMP_DRV_Start(0);

    /* Buttons */
    GPIO_DRV_InputPinInit(&g_switch1);

    /* Start LPTMR */
    LPTMR_DRV_Start(LPTMR0_IDX);

    /* Setup LPUART1 */
    LPUART_DRV_Init(1, &g_lpuartState, &g_lpuartConfig);
    LPUART_DRV_InstallRxCallback(1, lpuartRxCallback, rxBuff, NULL, true);
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 1, kPortMuxAlt3);

    /* Setup FlexIO for the WS2812B */
    FLEXIO_Type *fiobase = g_flexioBase[0];
    CLOCK_SYS_SetFlexioSrc(0, kClockFlexioSrcMcgIrClk);
    FLEXIO_DRV_Init(0, &g_flexioConfig);
    FLEXIO_HAL_ConfigureTimer(fiobase, 0, &g_timerConfig);
    FLEXIO_HAL_ConfigureShifter(fiobase, 0, &g_shifterConfig);
    PORT_HAL_SetMuxMode(g_portBase[GPIOE_IDX], 20, kPortMuxAlt6);
    //FLEXIO_DRV_RegisterCallback(0, 0, shift0_int_handler, NULL);
    FLEXIO_DRV_Start(0);

    FLEXIO_HAL_SetShifterStatusDmaCmd(fiobase, 1, true);
    uint32_t chan;
    chan = DMA_DRV_RequestChannel(kDmaAnyChannel, kDmaRequestMux0FlexIOChannel0,
            &g_fioChan);
    if (chan == kDmaInvalidChannel) {
        while(1);
    }
    DMA_DRV_RegisterCallback(&g_fioChan, fioDmaCallback, NULL);

    uint8_t green = 32;
    uint8_t red = 0;
    uint8_t blue = 32;
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


    /* We're done, everything else is triggered through interrupts */
    for(;;) {
#ifndef DEBUG
        //SMC_HAL_SetMode(SMC, &g_idlePowerMode);
#endif
    }
}

/* vim: set expandtab ts=4 sw=4: */
