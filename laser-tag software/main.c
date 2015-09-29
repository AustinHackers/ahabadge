/*
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

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
// SDK Included Files
#include "board.h"
#include "fsl_lptmr_driver.h"
#include "fsl_debug_console.h"
#include "fsl_dac_driver.h"
#include "fsl_os_abstraction.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
// Timer period: 500000uS
#define TMR_PERIOD         500000U
#if defined(TWR_KV46F150M)
#define LPTMR0_IDX LPTMR_IDX
#endif


const gpio_output_pin_user_config_t lcdBacklight = {
    .pinName = GPIO_MAKE_PIN(GPIOE_IDX, 31U),
    .config.outputLogic = 1,
    .config.slewRate = kPortSlowSlewRate,
    .config.driveStrength = kPortLowDriveStrength,
};

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief LPTMR interrupt call back function.
 * The function is used to toggle LED1.
 */
void lptmr_call_back(void)
{
    // Toggle LED1
    GPIO_DRV_TogglePinOutput(lcdBacklight.pinName);
}

/*!
 * @brief Main function
 */
int main (void)
{
    // LPTMR configurations
    lptmr_user_config_t lptmrConfig =
    {
        .timerMode = kLptmrTimerModeTimeCounter,
        .freeRunningEnable = false,
        .prescalerEnable = true,
        .prescalerClockSource = kClockLptmrSrcLpoClk,
        .prescalerValue = kLptmrPrescalerDivide2,
        .isInterruptEnabled = true,
    };
    // LPTMR driver state information
    lptmr_state_t lptmrState;

    // Initialize standard SDK demo application pins
    hardware_init();

    // Initialize LPTMR
    LPTMR_DRV_Init(LPTMR0_IDX, &lptmrState, &lptmrConfig);
    // Set timer period for TMR_PERIOD seconds
    LPTMR_DRV_SetTimerPeriodUs(LPTMR0_IDX, TMR_PERIOD);
    // Install interrupt call back function for LPTMR
    LPTMR_DRV_InstallCallback(LPTMR0_IDX, lptmr_call_back);
    // Start LPTMR
    LPTMR_DRV_Start(LPTMR0_IDX);

    // Initialize LED1
    GPIO_DRV_OutputPinInit(&lcdBacklight);

    // Print the initial banner
    PRINTF("\r\nHello World!\n\n\r");

    dac_converter_config_t MyDacUserConfigStruct;
    // Fill the structure with configuration of software trigger. //
    DAC_DRV_StructInitUserConfigNormal(&MyDacUserConfigStruct);
    // Initialize the DAC Converter. //
    DAC_DRV_Init(0, &MyDacUserConfigStruct);
    // Output the DAC value. //
    uint16_t p = 0x1000;
    uint16_t i = 0x2ff;
    int8_t d = 1;
    while(1) {
        if (i == 0x2ff) {
            d = 1;
        } else if (i == 0x4ff) {
            d = -1;
            p = p * 7 / 8;
            if (p == 0) p = 0x1000;
        }
        i += d;
        DAC_DRV_Output(0, i);
        //OSA_TimeDelay(1);
        uint16_t c;
        for(c = 0; c < p; ++c);
    }
}

/* vim: set expandtab ts=4 sw=4: */
