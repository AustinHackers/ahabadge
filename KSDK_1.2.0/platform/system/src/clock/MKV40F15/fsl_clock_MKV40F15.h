/*
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
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

#if !defined(__FSL_CLOCK_KV40F15_H__)
#define __FSL_CLOCK_KV40F15_H__

#include "fsl_mcg_hal.h"
#include "fsl_mcg_hal_modes.h"
#include "fsl_osc_hal.h"

/*! @addtogroup clock_manager*/
/*! @{*/

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FTM_EXT_CLK_COUNT  2  /* FTM external clock source count.  */

extern uint32_t g_ftmClkFreq[FTM_EXT_CLK_COUNT];          /* FTM_CLK          */

/*!@brief SIM configuration structure for dynamic clock setting. */
typedef struct SimConfig
{
    clock_er32k_src_t  er32kSrc;   /*!< ERCLK32K source selection.   */
    uint8_t outdiv1, outdiv2, outdiv3, outdiv4; /*!< OUTDIV setting. */
} sim_config_t;

/*! @brief Clock name configuration table structure*/
typedef struct ClockNameConfig {
    bool                            useOtherRefClock;     /*!< if it  uses the other ref clock*/
    clock_names_t                   otherRefClockName;    /*!< other ref clock name*/
    clock_divider_names_t           dividerName;          /*!< clock divider name*/
} clock_name_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/


/*!
 * @brief Sets the clock out divider1 setting(OUTDIV1).
 *
 * This function sets divide value OUTDIV1.
 *
 * @param outdiv1      Outdivider1 setting
 */
static inline void CLOCK_SYS_SetOutDiv1(uint8_t outdiv1)
{
    CLOCK_HAL_SetOutDiv1(SIM, outdiv1);
}

/*!
 * @brief Gets the clock out divider1 setting(OUTDIV1).
 *
 * This function gets divide value OUTDIV1.
 *
 * @return Outdivider1 setting
 */
static inline uint8_t CLOCK_SYS_GetOutDiv1(void)
{
    return CLOCK_HAL_GetOutDiv1(SIM);
}

/*!
 * @brief Sets the clock out divider2 setting(OUTDIV2).
 *
 * This function sets divide value OUTDIV2.
 *
 * @param outdiv2      Outdivider2 setting
 */
static inline void CLOCK_SYS_SetOutDiv2(uint8_t outdiv2)
{
    CLOCK_HAL_SetOutDiv2(SIM, outdiv2);
}

/*!
 * @brief Gets the clock out divider2 setting(OUTDIV2).
 *
 * This function gets divide value OUTDIV2.
 *
 * @return Outdivider2 setting
 */
static inline uint8_t CLOCK_SYS_GetOutDiv2(void)
{
    return CLOCK_HAL_GetOutDiv2(SIM);
}

/*!
 * @brief Sets the clock out divider4 setting(OUTDIV4).
 *
 * This function sets divide value OUTDIV4.
 *
 * @param outdiv4      Outdivider4 setting
 */
static inline void CLOCK_SYS_SetOutDiv4(uint8_t outdiv4)
{
    CLOCK_HAL_SetOutDiv4(SIM, outdiv4);
}

/*!
 * @brief Gets the clock out divider4 setting(OUTDIV4).
 *
 * This function gets divide value OUTDIV4.
 *
 * @return Outdivider4 setting
 */
static inline uint8_t CLOCK_SYS_GetOutDiv4(void)
{
    return CLOCK_HAL_GetOutDiv4(SIM);
}

/*!
 * @brief Sets the clock out dividers setting.
 *
 * This function sets the setting for all clock out dividers at the same time.
 *
 * @param outdiv1      Outdivider1 setting
 * @param outdiv2      Outdivider2 setting
 * @param outdiv3      Outdivider3 setting
 * @param outdiv4      Outdivider4 setting
 */
static inline void CLOCK_SYS_SetOutDiv(uint8_t outdiv1, uint8_t outdiv2,
                                       uint8_t outdiv3, uint8_t outdiv4)
{
    CLOCK_HAL_SetOutDiv(SIM, outdiv1, outdiv2, outdiv3, outdiv4);
}

/*!
 * @brief Gets the fast peripheral clock frequency.
 *
 * This function gets the fast peripheral clock frequency.
 *
 * @return Current clock frequency.
 */
uint32_t CLOCK_SYS_GetFastPeripheralClockFreq(void);

/*!
 * @brief Get the MCGPLLCLK/MCGFLLCLK/IRC48MCLK clock frequency.
 *
 * This function gets the frequency of the MCGPLLCLK/MCGFLLCLK/IRC48MCLK.
 *
 * @return Current clock frequency.
 */
uint32_t CLOCK_SYS_GetPllFllClockFreq(void);

/*!
 * @brief Gets the MCGFFCLK clock frequency.
 *
 * This function gets the MCG fixed frequency clock (MCGFFCLK) frequency.
 *
 * @return Current clock frequency.
 */
static inline uint32_t CLOCK_SYS_GetFixedFreqClockFreq(void)
{
    return CLOCK_HAL_GetFixedFreqClk(MCG);
}

/*!
 * @brief Get internal reference clock frequency.
 *
 * This function gets the internal reference clock frequency.
 *
 * @return Current clock frequency.
 */
static inline uint32_t CLOCK_SYS_GetInternalRefClockFreq(void)
{
    return CLOCK_HAL_GetInternalRefClk(MCG);
}

/*!
 * @brief Gets the external reference 32k clock frequency.
 *
 * This function gets the external reference (ERCLK32K) clock frequency.
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetExternalRefClock32kFreq(void);

/*!
 * @brief Set the clock selecton of ERCLK32K.
 *
 * This function sets the clock selecton of ERCLK32K.
 *
 * @param src clock source.
 */
static inline void CLOCK_SYS_SetExternalRefClock32kSrc(clock_er32k_src_t src)
{
    CLOCK_HAL_SetExternalRefClock32kSrc(SIM, src);
}

/*!
 * @brief Get the clock selecton of ERCLK32K.
 *
 * This function gets the clock selecton of ERCLK32K.
 *
 * @return Current selection.
 */
static inline clock_er32k_src_t CLOCK_SYS_GetExternalRefClock32kSrc(void)
{
    return CLOCK_HAL_GetExternalRefClock32kSrc(SIM);
}

/*!
 * @brief Gets the OSC0ERCLK frequency.
 *
 * This function gets the OSC0 external reference frequency.
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetOsc0ExternalRefClockFreq(void);

/*!
 * @brief Gets the OSC0ERCLK_UNDIV frequency.
 *
 * This function gets the undivided OSC0 external reference frequency.
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetOsc0ExternalRefClockUndivFreq(void);

/*!
 * @brief Gets the watch dog clock frequency.
 *
 * This function gets the watch dog clock frequency.
 *
 * @param instance module device instance
 * @param wdogSrc  watch dog clock source selection, WDOG_STCTRLH[CLKSRC].
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetWdogFreq(uint32_t instance, clock_wdog_src_t wdogSrc);

/*!
 * @brief Gets the debug trace clock source.
 *
 * This function gets the debug trace clock source.
 * @param instance module device instance
 *
 * @return Current source.
 */
static inline clock_trace_src_t CLOCK_SYS_GetTraceSrc(uint32_t instance)
{
    return CLOCK_HAL_GetTraceClkSrc(SIM);
}

/*!
 * @brief Sets the debug trace clock source.
 *
 * This function sets the debug trace clock source.
 * @param instance module device instance.
 * @param src   debug trace clock source.
 */
static inline void CLOCK_SYS_SetTraceSrc(uint32_t instance, clock_trace_src_t src)
{
    CLOCK_HAL_SetTraceClkSrc(SIM, src);
}

/*!
 * @brief Gets the debug trace clock frequency.
 *
 * This function gets the debug trace clock frequency.
 * @param instance module device instance
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetTraceFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for DMA module.
 *
 * This function gets the clock frequence for DMA moudle.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetDmaFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for DMAMUX module.
 *
 * This function gets the clock frequence for DMAMUX moudle.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetDmamuxFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for EWM module.
 *
 * This function gets the clock frequence for EWM moudle.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetEwmFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for FTF module. (Flash Memory)
 *
 * This function gets the clock frequence for FTF module. (Flash Memory)
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetFtfFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for CRC module.
 *
 * This function gets the clock frequence for CRC module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetCrcFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for ADC module.
 *
 * This function gets the clock frequence for ADC module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetAdcFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for CMP module.
 *
 * This function gets the clock frequence for CMP module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetCmpFreq(uint32_t instance);

/*!
 * @brief Gets the  frequency.
 *
 * This function gets the OSC0 external reference frequency.
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetOsc0ExternalRefClockFreq(void);


/*!
 * @brief Gets the clock frequency for PDB module.
 *
 * This function gets the clock frequence for PDB module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetPdbFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for eFlexPWM module.
 *
 * This function gets the clock frequence for eFlexPWM module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetPwmFreq(uint32_t instance);

/*!
 * @brief Gets FTM external clock frequency.
 *
 * This function gets the FTM external clock frequency.
 *
 * @param instance module device instance
 * @return freq    Current frequency.
 */
uint32_t CLOCK_SYS_GetFtmExternalFreq(uint32_t instance);

/*!
 * @brief Gets FTM external clock source.
 *
 * This function gets the FTM external clock source.
 *
 * @param instance module device instance.
 * @return Ftm external clock source.
 */
static inline sim_ftm_clk_sel_t CLOCK_SYS_GetFtmExternalSrc(uint32_t instance)
{
    return SIM_HAL_GetFtmExternalClkPinMode(SIM, instance);
}

/*!
 * @brief Sets FTM external clock source.
 *
 * This function sets the FTM external clock source.
 *
 * @param instance module device instance.
 * @param ftmSrc FTM clock source setting.
 */
static inline void CLOCK_SYS_SetFtmExternalSrc(uint32_t instance,
                                               sim_ftm_clk_sel_t ftmSrc)
{
    SIM_HAL_SetFtmExternalClkPinMode(SIM, instance, ftmSrc);
}

/*!
 * @brief Gets FTM fixed frequency clock frequency.
 *
 * This function gets the FTM fixed frequency clock frequency.
 *
 * @param instance module device instance
 * @return freq    Current frequency.
 */
static inline uint32_t CLOCK_SYS_GetFtmFixedFreq(uint32_t instance)
{
    return CLOCK_SYS_GetFixedFreqClockFreq();
}

/*!
 * @brief Gets FTM's system clock frequency.
 *
 * This function gets the FTM's system clock frequency.
 *
 * @param instance module device instance
 * @return Current frequency.
 */
static inline uint32_t CLOCK_SYS_GetFtmSystemClockFreq(uint32_t instance)
{
    return CLOCK_SYS_GetFastPeripheralClockFreq();
}

/*!
 * @brief Enable the clock for PORT module.
 *
 * This function enables the clock for PORT module.
 * @param instance module device instance
 */
void CLOCK_SYS_EnablePortClock(uint32_t instance);

/*!
 * @brief Disable the clock for PORT module.
 *
 * This function disables the clock for PORT module.
 * @param instance module device instance
 */
void CLOCK_SYS_DisablePortClock(uint32_t instance);

/*!
 * @brief Get the the clock gate state for PORT module.
 *
 * This function will get the clock gate state for PORT module.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
bool CLOCK_SYS_GetPortGateCmd(uint32_t instance);

/*!
 * @brief Gets the clock frequency for PIT module.
 *
 * This function gets the clock frequence for PIT module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetPitFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for SPI module
 *
 * This function gets the clock frequence for SPI module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetSpiFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for I2C module
 *
 * This function gets the clock frequence for I2C module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetI2cFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for UART module
 *
 * This function gets the clock frequence for UART module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetUartFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for GPIO module
 *
 * This function gets the clock frequence for GPIO module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetGpioFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for ENC module
 *
 * This function gets the clock frequence for ENC module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetEncFreq(uint32_t instance);

/*!
 * @brief Gets the clock frequency for XBAR module
 *
 * This function gets the clock frequence for XBAR module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetXbarFreq(uint32_t instance);

/*!
 * @brief Gets LPTMRx pre-scaler/glitch filter clock frequency.
 *
 * This function gets the LPTMRx pre-scaler/glitch filter clock frequency.
 *
 * @param instance module device instance
 * @param lptmrSrc LPTMRx clock source selection.
 *
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetLptmrFreq(uint32_t instance, clock_lptmr_src_t lptmrSrc);

/*!
 * @brief Gets the clock frequency for AOI module
 *
 * This function gets the clock frequence for AOI module.
 * @param instance module device instance
 * @return freq    clock frequence for this module
 */
uint32_t    CLOCK_SYS_GetAoiFreq(uint32_t instance);

/*!
 * @brief Enable the clock for DMA module.
 *
 * This function enables the clock for DMA moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableDmaClock(uint32_t instance)
{
    SIM_HAL_EnableDmaClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for DMA module.
 *
 * This function disables the clock for DMA moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableDmaClock(uint32_t instance)
{
    SIM_HAL_DisableDmaClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for DMA module.
 *
 * This function will get the clock gate state for DMA moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetDmaGateCmd(uint32_t instance)
{
    return SIM_HAL_GetDmaGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for DMAMUX module.
 *
 * This function enables the clock for DMAMUX moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableDmamuxClock(uint32_t instance)
{
    SIM_HAL_EnableDmamuxClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for DMAMUX module.
 *
 * This function disables the clock for DMAMUX moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableDmamuxClock(uint32_t instance)
{
    SIM_HAL_DisableDmamuxClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for DMAMUX module.
 *
 * This function will get the clock gate state for DMAMUX moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetDmamuxGateCmd(uint32_t instance)
{
    return SIM_HAL_GetDmamuxGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for EWM module.
 *
 * This function enables the clock for EWM moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableEwmClock(uint32_t instance)
{
    SIM_HAL_EnableEwmClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for EWM module.
 *
 * This function disables the clock for EWM moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableEwmClock(uint32_t instance)
{
    SIM_HAL_DisableEwmClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for EWM module.
 *
 * This function will get the clock gate state for EWM moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetEwmGateCmd(uint32_t instance)
{
    return SIM_HAL_GetEwmGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for FTF module.
 *
 * This function enables the clock for FTF moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFtfClock(uint32_t instance)
{
    SIM_HAL_EnableFtfClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for FTF module.
 *
 * This function disables the clock for FTF moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFtfClock(uint32_t instance)
{
    SIM_HAL_DisableFtfClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for FTF module.
 *
 * This function will get the clock gate state for FTF moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFtfGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFtfGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for CRC module.
 *
 * This function enables the clock for CRC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableCrcClock(uint32_t instance)
{
    SIM_HAL_EnableCrcClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for CRC module.
 *
 * This function disables the clock for CRC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableCrcClock(uint32_t instance)
{
    SIM_HAL_DisableCrcClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for CRC module.
 *
 * This function will get the clock gate state for CRC moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetCrcGateCmd(uint32_t instance)
{
    return SIM_HAL_GetCrcGateCmd(g_simBase[0], instance);
}



/*!
 * @brief Enable the clock for ADC module.
 *
 * This function enables the clock for ADC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableAdcClock(uint32_t instance)
{
    SIM_HAL_EnableAdcClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for ADC module.
 *
 * This function disables the clock for ADC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableAdcClock(uint32_t instance)
{
    SIM_HAL_DisableAdcClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for ADC module.
 *
 * This function will get the clock gate state for ADC moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetAdcGateCmd(uint32_t instance)
{
    return SIM_HAL_GetAdcGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for CMP module.
 *
 * This function enables the clock for CMP moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableCmpClock(uint32_t instance)
{
    SIM_HAL_EnableCmpClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for CMP module.
 *
 * This function disables the clock for CMP moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableCmpClock(uint32_t instance)
{
    SIM_HAL_DisableCmpClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for CMP module.
 *
 * This function will get the clock gate state for CMP moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetCmpGateCmd(uint32_t instance)
{
    return SIM_HAL_GetCmpGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for DAC module.
 *
 * This function enables the clock for DAC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableDacClock(uint32_t instance)
{
    SIM_HAL_EnableDacClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for DAC module.
 *
 * This function disables the clock for DAC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableDacClock(uint32_t instance)
{
    SIM_HAL_DisableDacClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for DAC module.
 *
 * This function will get the clock gate state for DAC moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetDacGateCmd(uint32_t instance)
{
    return SIM_HAL_GetDacGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for PDB module.
 *
 * This function enables the clock for PDB moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnablePdbClock(uint32_t instance)
{
    SIM_HAL_EnablePdbClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for PDB module.
 *
 * This function disables the clock for PDB moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisablePdbClock(uint32_t instance)
{
    SIM_HAL_DisablePdbClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for PDB module.
 *
 * This function will get the clock gate state for PDB moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetPdbGateCmd(uint32_t instance)
{
    return SIM_HAL_GetPdbGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for FTM module.
 *
 * This function enables the clock for FTM moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFtmClock(uint32_t instance)
{
    SIM_HAL_EnableFtmClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for FTM module.
 *
 * This function disables the clock for FTM moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFtmClock(uint32_t instance)
{
    SIM_HAL_DisableFtmClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for FTM module.
 *
 * This function will get the clock gate state for FTM moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFtmGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFtmGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for PIT module.
 *
 * This function enables the clock for PIT moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnablePitClock(uint32_t instance)
{
    SIM_HAL_EnablePitClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for PIT module.
 *
 * This function disables the clock for PIT moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisablePitClock(uint32_t instance)
{
    SIM_HAL_DisablePitClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for PIT module.
 *
 * This function will get the clock gate state for PIT moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetPitGateCmd(uint32_t instance)
{
    return SIM_HAL_GetPitGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for LPTIMER module.
 *
 * This function enables the clock for LPTIMER moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableLptmrClock(uint32_t instance)
{
    SIM_HAL_EnableLptmrClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for LPTIMER module.
 *
 * This function disables the clock for LPTIMER moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableLptmrClock(uint32_t instance)
{
    SIM_HAL_DisableLptmrClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for LPTIMER module.
 *
 * This function will get the clock gate state for LPTIMER moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetLptmrGateCmd(uint32_t instance)
{
    return SIM_HAL_GetLptmrGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for FLEXCAN module.
 *
 * This function enables the clock for FLEXCAN moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableFlexcanClock(uint32_t instance)
{
    SIM_HAL_EnableFlexcanClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for FLEXCAN module.
 *
 * This function disables the clock for FLEXCAN moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableFlexcanClock(uint32_t instance)
{
    SIM_HAL_DisableFlexcanClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for FLEXCAN module.
 *
 * This function will get the clock gate state for FLEXCAN moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetFlexcanGateCmd(uint32_t instance)
{
    return SIM_HAL_GetFlexcanGateCmd(g_simBase[0], instance);
}

 /*!
 * @brief Gets FLEXCAN clock frequency.
 *
 * This function gets the FLEXCAN clock frequency.
 *
 * @param instance   module device instance
 * @param flexcanSrc clock source
 * @return Current frequency.
 */
uint32_t CLOCK_SYS_GetFlexcanFreq(uint8_t instance, clock_flexcan_src_t flexcanSrc);

/*!
 * @brief Enable the clock for SPI module.
 *
 * This function enables the clock for SPI moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableSpiClock(uint32_t instance)
{
    SIM_HAL_EnableSpiClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for SPI module.
 *
 * This function disables the clock for SPI moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableSpiClock(uint32_t instance)
{
    SIM_HAL_DisableSpiClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for SPI module.
 *
 * This function will get the clock gate state for SPI moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetSpiGateCmd(uint32_t instance)
{
    return SIM_HAL_GetSpiGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for I2C module.
 *
 * This function enables the clock for I2C moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableI2cClock(uint32_t instance)
{
    SIM_HAL_EnableI2cClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for I2C module.
 *
 * This function disables the clock for I2C moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableI2cClock(uint32_t instance)
{
    SIM_HAL_DisableI2cClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for I2C module.
 *
 * This function will get the clock gate state for I2C moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetI2cGateCmd(uint32_t instance)
{
    return SIM_HAL_GetI2cGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for UART module.
 *
 * This function enables the clock for UART moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableUartClock(uint32_t instance)
{
    SIM_HAL_EnableUartClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for UART module.
 *
 * This function disables the clock for UART moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableUartClock(uint32_t instance)
{
    SIM_HAL_DisableUartClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for UART module.
 *
 * This function will get the clock gate state for UART moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetUartGateCmd(uint32_t instance)
{
    return SIM_HAL_GetUartGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for eFlexPWM module.
 *
 * This function enables the clock for eFlexPWM moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnablePwmClock(uint32_t instance)
{
    SIM_HAL_EnablePwmClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for eFlexPWM module.
 *
 * This function disables the clock for eFlexPWM moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisablePwmClock(uint32_t instance)
{
    SIM_HAL_DisablePwmClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for eFlexPWM module.
 *
 * This function will get the clock gate state for eFlexPWM moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetPwmGateCmd(uint32_t instance)
{
    return SIM_HAL_GetPwmGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for AOI module.
 *
 * This function enables the clock for AOI moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableAoiClock(uint32_t instance)
{
    SIM_HAL_EnableAoiClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for AOI module.
 *
 * This function disables the clock for AOI moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableAoiClock(uint32_t instance)
{
    SIM_HAL_DisableAoiClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for AOI module.
 *
 * This function will get the clock gate state for AOI moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetAoiGateCmd(uint32_t instance)
{
    return SIM_HAL_GetAoiGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for XBAR module.
 *
 * This function enables the clock for XBAR moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableXbarClock(uint32_t instance)
{
    SIM_HAL_EnableXbarClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for XBAR module.
 *
 * This function disables the clock for XBAR moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableXbarClock(uint32_t instance)
{
    SIM_HAL_DisableXbarClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for XBAR module.
 *
 * This function will get the clock gate state for XBAR moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetXbarGateCmd(uint32_t instance)
{
    return SIM_HAL_GetXbarGateCmd(g_simBase[0], instance);
}

/*!
 * @brief Enable the clock for ENC module.
 *
 * This function enables the clock for ENC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_EnableEncClock(uint32_t instance)
{
    SIM_HAL_EnableEncClock(g_simBase[0], instance);
}

/*!
 * @brief Disable the clock for ENC module.
 *
 * This function disables the clock for ENC moudle.
 * @param instance module device instance
 */
static inline void CLOCK_SYS_DisableEncClock(uint32_t instance)
{
    SIM_HAL_DisableEncClock(g_simBase[0], instance);
}

/*!
 * @brief Get the the clock gate state for ENC module.
 *
 * This function will get the clock gate state for ENC moudle.
 * @param instance module device instance
 * @return state true - ungated(Enabled), false - gated (Disabled)
 */
static inline bool CLOCK_SYS_GetEncGateCmd(uint32_t instance)
{
    return SIM_HAL_GetEncGateCmd(g_simBase[0], instance);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* __FSL_CLOCK_KV40F15_H__ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

