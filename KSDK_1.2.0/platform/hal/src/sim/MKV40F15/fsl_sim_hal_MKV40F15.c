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

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsl_device_registers.h"
#include "fsl_sim_hal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * APIs
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_SetOutDiv
 * Description   : Set all clock out dividers setting at the same time
 * This function will set the setting for all clock out dividers.
 *
 *END**************************************************************************/
void CLOCK_HAL_SetOutDiv(SIM_Type* base,
                         uint8_t outdiv1,
                         uint8_t outdiv2,
                         uint8_t outdiv3,
                         uint8_t outdiv4)
{
    uint32_t clkdiv1 = 0;

    clkdiv1 |= SIM_CLKDIV1_OUTDIV1(outdiv1);
    clkdiv1 |= SIM_CLKDIV1_OUTDIV2(outdiv2);
    clkdiv1 |= SIM_CLKDIV1_OUTDIV4(outdiv4);

    SIM_WR_CLKDIV1(base, clkdiv1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_GetOutDiv
 * Description   : Get all clock out dividers setting at the same time
 * This function will get the setting for all clock out dividers.
 *
 *END**************************************************************************/
void CLOCK_HAL_GetOutDiv(SIM_Type* base,
                         uint8_t *outdiv1,
                         uint8_t *outdiv2,
                         uint8_t *outdiv3,
                         uint8_t *outdiv4)
{
    *outdiv1 = SIM_BRD_CLKDIV1_OUTDIV1(base);
    *outdiv2 = SIM_BRD_CLKDIV1_OUTDIV2(base);
    *outdiv4 = SIM_BRD_CLKDIV1_OUTDIV4(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableDmaClock
 * Description   : Enable the clock for DMA module
 * This function enables the clock for DMA moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableDmaClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC7_DMA(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableDmaClock
 * Description   : Disable the clock for DMA module
 * This function disables the clock for DMA moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableDmaClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC7_DMA(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDmaGateCmd
 * Description   : Get the the clock gate state for DMA module
 * This function will get the clock gate state for DMA moudle
 *
 *END**************************************************************************/
bool SIM_HAL_GetDmaGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC7_DMA(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableDmamuxClock
 * Description   : Enable the clock for DMAMUX module
 * This function enables the clock for DMAMUX moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableDmamuxClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_DMAMUX(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableDmamuxClock
 * Description   : Disable the clock for DMAMUX module
 * This function disables the clock for DMAMUX moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableDmamuxClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_DMAMUX(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDmamuxGateCmd
 * Description   : Get the the clock gate state for DMAMUX module
 * This function will get the clock gate state for DMAMUX moudle
 *
 *END**************************************************************************/
bool SIM_HAL_GetDmamuxGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC6_DMAMUX(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePortClock
 * Description   : Enable the clock for PORT module
 * This function enables the clock for PORT moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePortClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC5_PORTA(base, 1);
        break;
    case 1:
        SIM_BWR_SCGC5_PORTB(base, 1);
        break;
    case 2:
        SIM_BWR_SCGC5_PORTC(base, 1);
        break;
    case 3:
        SIM_BWR_SCGC5_PORTD(base, 1);
        break;
    case 4:
        SIM_BWR_SCGC5_PORTE(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePortClock
 * Description   : Disable the clock for PORT module
 * This function disables the clock for PORT moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePortClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC5_PORTA(base, 0);
        break;
    case 1:
        SIM_BWR_SCGC5_PORTB(base, 0);
        break;
    case 2:
        SIM_BWR_SCGC5_PORTC(base, 0);
        break;
    case 3:
        SIM_BWR_SCGC5_PORTD(base, 0);
        break;
    case 4:
        SIM_BWR_SCGC5_PORTE(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPortGateCmd
 * Description   : Get the the clock gate state for PORT module
 * This function will get the clock gate state for PORT moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPortGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC5_PORTA(base);
        break;
    case 1:
        retValue =  SIM_BRD_SCGC5_PORTB(base);
        break;
    case 2:
        retValue =  SIM_BRD_SCGC5_PORTC(base);
        break;
    case 3:
        retValue =  SIM_BRD_SCGC5_PORTD(base);
        break;
    case 4:
        retValue =  SIM_BRD_SCGC5_PORTE(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableEwmClock
 * Description   : Enable the clock for EWM module
 * This function enables the clock for EWM moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableEwmClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC4_EWM(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableEwmClock
 * Description   : Disable the clock for EWM modul
 * This function disables the clock for EWM moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableEwmClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC4_EWM(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetEwmGateCmd
 * Description   : Get the the clock gate state for EWM module
 * This function will get the clock gate state for EWM moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetEwmGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC4_EWM(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFtfClock
 * Description   : Enable the clock for FTF module
 * This function enables the clock for FTF moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableFtfClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_FTF(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFtfClock
 * Description   : Disable the clock for FTF module
 * This function disables the clock for FTF moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableFtfClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_FTF(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtfGateCmd
 * Description   : Get the the clock gate state for FTF module
 * This function will get the clock gate state for FTF moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFtfGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC6_FTF(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableCrcClock
 * Description   : Enable the clock for CRC module
 * This function enables the clock for CRC moudle.
 *
 *END**************************************************************************/
void SIM_HAL_EnableCrcClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_CRC(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableCrcClock
 * Description   : Disable the clock for CRC module
 * This function disables the clock for CRC moudle.
 *
 *END**************************************************************************/
void SIM_HAL_DisableCrcClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_CRC(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetCrcGateCmd
 * Description   : Get the the clock gate state for CRC module
 * This function will get the clock gate state for CRC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetCrcGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC6_CRC(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableAdcClock
 * Description   : Enable the clock for ADC module
 * This function enables the clock for ADC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableAdcClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_ADC(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableAdcClock
 * Description   : Disable the clock for ADC module
 * This function disables the clock for ADC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableAdcClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_ADC(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcGateCmd
 * Description   : Get the the clock gate state for ADC module
 * This function will get the clock gate state for ADC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetAdcGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    retValue =  SIM_BRD_SCGC5_ADC(base);
    
    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableCmpClock
 * Description   : Enable the clock for CMP module
 * This function enables the clock for CMP moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableCmpClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC4_CMP(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableCmpClock
 * Description   : Disable the clock for CMP module
 * This function disables the clock for CMP moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableCmpClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC4_CMP(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetCmpGateCmd
 * Description   : Get the the clock gate state for CMP module
 * This function will get the clock gate state for CMP moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetCmpGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC4_CMP(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableDacClock
 * Description   : Enable the clock for DAC module
 * This function enables the clock for DAC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableDacClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_DAC0(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableDacClock
 * Description   : Disable the clock for DAC module
 * This function disables the clock for DAC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableDacClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_DAC0(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDacGateCmd
 * Description   : Get the the clock gate state for DAC module
 * This function will get the clock gate state for DAC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetDacGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    retValue =  SIM_BRD_SCGC6_DAC0(base);

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePdbClock
 * Description   : Enable the clock for PDB module
 * This function enables the clock for PDB moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePdbClock(SIM_Type* base, uint32_t instance)
{
    switch(instance)
    {
        case 0:
            SIM_BWR_SCGC6_PDB0(base, 1);
            break;
        case 1:
            SIM_BWR_SCGC6_PDB1(base, 1);
            break;
        default: break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePdbClock
 * Description   : Disable the clock for PDB module
 * This function disables the clock for PDB moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePdbClock(SIM_Type* base, uint32_t instance)
{
    switch(instance)
    {
        case 0:
            SIM_BWR_SCGC6_PDB0(base, 0);
            break;
        case 1:
            SIM_BWR_SCGC6_PDB1(base, 0);
            break;
        default: break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPdbGateCmd
 * Description   : Get the the clock gate state for PDB module
 * This function will get the clock gate state for PDB moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPdbGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;
    
    switch(instance)
    {
        case 0:
            retValue = SIM_BRD_SCGC6_PDB0(base);
            break;
        case 1:
            retValue = SIM_BRD_SCGC6_PDB1(base);
            break;
        default: break;
    }
    
    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFtmClock
 * Description   : Enable the clock for FTM module
 * This function enables the clock for FTM moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableFtmClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC6_FTM0(base, 1);
        break;
    case 1:
        SIM_BWR_SCGC6_FTM1(base, 1);
        break;    
    case 3:
        SIM_BWR_SCGC6_FTM3(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFtmClock
 * Description   : Disable the clock for FTM module
 * This function disables the clock for FTM moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableFtmClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC6_FTM0(base, 0);
        break;
    case 1:
        SIM_BWR_SCGC6_FTM1(base, 0);
        break;
    case 3:
        SIM_BWR_SCGC6_FTM3(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmGateCmd
 * Description   : Get the the clock gate state for FTM module
 * This function will get the clock gate state for FTM moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFtmGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC6_FTM0(base);
        break;
    case 1:
        retValue =  SIM_BRD_SCGC6_FTM1(base);
        break;
    case 2:
        retValue =  false;
        break;    
    case 3:
        retValue =  SIM_BRD_SCGC6_FTM3(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcxMuxSelChannely
 * Description   : ADC x (0-A, 1-B)channel y (6,7) Mux.
 * Refer to RM for specific channel settings        
 * Selects ADCx MUX0's channel to ADCx channel y.
 *
 *END**************************************************************************/
void SIM_HAL_SetAdcxMuxSelChannely(SIM_Type* base, uint32_t instance, 
                                   uint8_t channel, uint8_t select)
{
  assert (instance<2);
  assert (channel<8);
  assert (channel>5);
    switch(instance)
    {
    case 0:
      switch(channel)
          {
          case 6:
            SIM_BWR_ADCOPT_ADCACH6SEL(base, select);
            break;
          case 7:
            SIM_BWR_ADCOPT_ADCACH7SEL(base, select);
            break;   
          default:
            break;
          }
          break;
    case 1:
      switch(channel)
          {
          case 6:
            SIM_BWR_ADCOPT_ADCBCH6SEL(base, select);
            break;
          case 7:
            SIM_BWR_ADCOPT_ADCBCH7SEL(base, select);
            break;  
          default:
            break;
          }
          break;
    default:
      break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmChOutSrcMode
 * Description   : Get FlexTimer x channel y output source select setting
 * This function will get FlexTimer x channel y output source select
 * setting.
 *
 *END**************************************************************************/
uint8_t SIM_HAL_GetAdcxMuxSelChannely(SIM_Type* base, uint32_t instance, uint8_t channel)
{
  assert (instance<2);
  assert (channel<8);
  assert (channel>5);
  uint8_t retValue = (uint8_t) false;
  switch(instance)
    {
    case 0:
      switch(channel)
          {
          case 6:
            retValue = (uint8_t)SIM_BRD_ADCOPT_ADCACH6SEL(base);
            break;
          case 7:
            retValue = (uint8_t)SIM_BRD_ADCOPT_ADCACH7SEL(base);
            break;  
          default:
            retValue = (uint8_t) false;
            break;
          }
          break;
    case 3:
      switch(channel)
          {
          case 6:
            retValue = (uint8_t)SIM_BRD_ADCOPT_ADCBCH6SEL(base);
            break;
          case 7:
            retValue = (uint8_t)SIM_BRD_ADCOPT_ADCBCH7SEL(base);
            break;  
          default:
            retValue = (uint8_t) false;
            break;
          }
          break;
    default:
      retValue = (uint8_t) false;
      break;
    }
  return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetNanoedgeRegulator12SupStdbyControl
 * Description   : Nanoedge Regulator 1.2 V Supply Standby Control
 * This function controls the standby mode of the 1.2 V supply from the nanoedge voltage regulator.
 * 00 Nanoedge regulator 1.2 V supply placed in normal mode
 * 01 Nanoedge regulator 1.2 V supply placed in standby mode.
 * 10 Nanoedge regulator 1.2 V supply placed in normal mode and SR12STDBY is write protected until
 * chip reset.
 * 11 Nanoedge regulator 1.2 V supply placed in standby mode and SR12STDBY is write protected until
 * chip reset.
 *
 *END**************************************************************************/
void SIM_HAL_SetNanoedgeRegulator12SupStdbyControl(SIM_Type* base, uint8_t select)
{
   assert (select < 4);
   SIM_BWR_PWRC_SR12STDBY(base, select);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetNanoedgeRegulator12SupStdbyControl
 * Description   : Get Nanoedge Regulator 1.2 V Supply Standby Control
 * This function will get the standby mode of the 1.2 V supply from the nanoedge voltage regulator
 *
 *END**************************************************************************/
uint8_t SIM_HAL_GetNanoedgeRegulator12SupStdbyControl(SIM_Type* base)
{
    return (uint8_t)SIM_BRD_PWRC_SR12STDBY(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetNanoedgeRegulator27SupStdbyControl
 * Description   : Nanoedge Regulator 2.7 V Supply Standby Control
 * This function controls the standby mode of the 2.7 V supply from the nanoedge voltage regulator.
 * 00 Nanoedge regulator 2.7 V supply placed in normal mode
 * 01 Nanoedge regulator 2.7 V supply placed in standby mode.
 * 10 Nanoedge regulator 2.7 V supply placed in normal mode and SR27STDBY is write protected until
 * chip reset.
 * 11 Nanoedge regulator 2.7 V supply placed in standby mode and SR27STDBY is write protected until
 * chip reset.
 *
 *END**************************************************************************/
void SIM_HAL_SetNanoedgeRegulator27SupStdbyControl(SIM_Type* base, uint8_t select)
{
   assert (select < 4);
   SIM_BWR_PWRC_SR12STDBY(base, select);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetNanoedgeRegulator27SupStdbyControl
 * Description   : Get Nanoedge Regulator 2.7 V Supply Standby Control
 * This function will get the standby mode of the 2.7 V supply from the nanoedge voltage regulator
 *
 *END**************************************************************************/
uint8_t SIM_HAL_GetNanoedgeRegulator27SupStdbyControl(SIM_Type* base)
{
    return (uint8_t)SIM_BRD_PWRC_SR12STDBY(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetNanoedgeReg27n12SupPwrdwnControl
 * Description   : Nanoedge Regulator 2.7V and 1.2V Supply Powerdown Control
 * This function controls the powerdown mode of the 2.7V and 1.2V supply from the nanoedge voltage regulator
 * 00 Nanoedge regulator placed in normal mode.
 * 01 Nanoedge regulator placed in powerdown mode.
 * 10 Nanoedge regulator placed in normal mode and SRPDN is write protected until chip reset.
 * 11 Nanoedge regulator placed in powerdown mode and SRPDN is write protected until chip reset.
 *
 *END**************************************************************************/
void SIM_HAL_SetNanoedgeReg27n12SupPwrdwnControl(SIM_Type* base, uint8_t select)
{
   assert (select < 4);
   SIM_BWR_PWRC_SRPDN(base, select);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetNanoedgeReg27n12SupPwrdwnControl
 * Description   : Get Nanoedge Regulator 2.7 V Supply Standby Control
 * This function will get the standby mode of the 2.7 V supply from the nanoedge voltage regulator
 *
 *END**************************************************************************/
uint8_t SIM_HAL_GetNanoedgeReg27n12SupPwrdwnControl(SIM_Type* base)
{
    return (uint8_t)SIM_BRD_PWRC_SRPDN(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetWdogClkSrc
 * Description   : WDOG Clock Select
 * This function selects the clock source of the WDOG2008 watchdog.
 * 0 Internal 1 kHz clock is source to WDOG2008
 * 1 MCGIRCLK is source to WDOG2008
 *
 *END**************************************************************************/
void SIM_HAL_SetWdogClkSrc(SIM_Type* base, bool select)
{
        SIM_BWR_WDOGC_WDOGCLKS(base, select);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetWdogClkSrc
 * Description   : Get WDOG Clock setting
 * This function will get the clock source of the WDOG2008 watchdog
 *
 *END**************************************************************************/
bool SIM_HAL_GetWdogClkSrc(SIM_Type* base)
{
    return (bool)SIM_BRD_WDOGC_WDOGCLKS(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSyncXbarAPittrigX
 * Description   : Synchronize XBARA's Input PIT Trigger X with fast clock
 * This function controls the synchronizer between PIT trigger X and XBARA's input.
 *
 *END**************************************************************************/
void SIM_HAL_EnableSyncXbarAPittrigX(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG0(base, 1);
        break;
    case 1:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG1(base, 1);
        break;
    case 2:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG2(base, 1);
        break;
    case 3:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG3(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSyncXbarAPittrigX
 * Description   : Disable the synchronizer between PIT trigger X and XBARA's input
 * This function Disables the synchronizer between PIT trigger X and XBARA's input
 *
 *END**************************************************************************/
void SIM_HAL_DisableSyncXbarAPittrigX(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG0(base, 0);
        break;
    case 1:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG1(base, 0);
        break;
    case 2:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG2(base, 0);
        break;
    case 3:
        SIM_BWR_MISCTRL2_SYNCXBARAPITTRIG3(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSyncXbarAPittrigXcmd
 * Description   : Get the synchronizer cmd between PIT trigger X and XBARA's input
 *
 *END**************************************************************************/
bool SIM_HAL_GetSyncXbarAPittrigXcmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_MISCTRL2_SYNCXBARAPITTRIG0(base);
        break;
    case 1:
        retValue =  SIM_BRD_MISCTRL2_SYNCXBARAPITTRIG1(base);
        break;
    case 2:
        retValue =  SIM_BRD_MISCTRL2_SYNCXBARAPITTRIG2(base);
        break;
    case 3:
        retValue =  SIM_BRD_MISCTRL2_SYNCXBARAPITTRIG3(base);
        break;
    default:
        retValue =  false;
        break;
    }
    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSyncXbarBPittrigX
 * Description   : Synchronize XBARB's Input PIT Trigger X with fast clock
 * This function controls the synchronizer between PIT trigger X and XBARB's input.
 *
 *END**************************************************************************/
void SIM_HAL_EnableSyncXbarBPittrigX(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL2_SYNCXBARBPITTRIG0(base, 1);
        break;
    case 1:
        SIM_BWR_MISCTRL2_SYNCXBARBPITTRIG1(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSyncXbarBPittrigX
 * Description   : Disable the synchronizer between PIT trigger X and XBARB's input
 * This function Disables the synchronizer between PIT trigger X and XBARB's input
 *
 *END**************************************************************************/
void SIM_HAL_DisableSyncXbarBPittrigX(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL2_SYNCXBARBPITTRIG0(base, 0);
        break;
    case 1:
        SIM_BWR_MISCTRL2_SYNCXBARBPITTRIG1(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSyncXbarBPittrigXcmd
 * Description   : Get the synchronizer cmd between PIT trigger X and XBARB's input
 *
 *END**************************************************************************/
bool SIM_HAL_GetSyncXbarBPittrigXcmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_MISCTRL2_SYNCXBARBPITTRIG0(base);
        break;
    case 1:
        retValue =  SIM_BRD_MISCTRL2_SYNCXBARBPITTRIG1(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSyncXbarDac
 * Description   : Synchronize XBARA's output for DAC Hardware Trigger with flash/slow clock
 * This function controls the synchronizer between XBARA's output and DAC hardware trigger.
 *
 *END**************************************************************************/
void SIM_HAL_EnableSyncXbarDac(SIM_Type* base)
{
    SIM_BWR_MISCTRL2_SYNCDACHWTRIG(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSyncXbarDac
 * Description   : Disable  the synchronizer  between XBARA's output and DAC hardware trigger
 * This function Disables  the synchronizer  between XBARA's output and DAC hardware trigger
 *
 *END**************************************************************************/
void SIM_HAL_DisableSyncXbarDac(SIM_Type* base)
{
    SIM_BWR_MISCTRL2_SYNCDACHWTRIG(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSyncXbarDaccmd
 * Description   : Get the synchronizer cmd between XBARA's output  and DAC hardware trigger
 *
 *END**************************************************************************/
bool SIM_HAL_GetSyncXbarDaccmd(SIM_Type* base)
{
    return  (bool)SIM_BRD_MISCTRL2_SYNCDACHWTRIG(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSyncXbarEwmin
 * Description   : Enable the synchronizer  between XBARA's output and EWM's ewm_in
 * This function enables the synchronizer between XBARA's output and EWM's ewm_in
 *
 *END**************************************************************************/
void SIM_HAL_EnableSyncXbarEwmin(SIM_Type* base)
{
        SIM_BWR_MISCTRL2_SYNCEWMIN(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSyncXbarEwmin
 * Description   : Disable  the synchronizer  between XBARA's output and EWM's ewm_in
 * This function Disables  the synchronizer  between XBARA's output and EWM's ewm_in
 *
 *END**************************************************************************/
void SIM_HAL_DisableSyncXbarEwmin(SIM_Type* base)
{
  SIM_BWR_MISCTRL2_SYNCEWMIN(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSyncXbarEwmincmd
 * Description   : Get the synchronizer cmd between XBARA's output  and EWM's ewm_in
 *
 *END**************************************************************************/
bool SIM_HAL_GetSyncXbarEwmincmd(SIM_Type* base)
{
    return  (bool)SIM_BRD_MISCTRL2_SYNCEWMIN(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSyncXbarCmpX
 * Description   : Enable the the synchronizer between XBARA's output and CMP3's sample/window input
 * This function enables the the synchronizer between XBARA's output and CMP3's sample/window input
 *
 *END**************************************************************************/
void SIM_HAL_EnableSyncXbarCmpX(SIM_Type* base, uint32_t instance)
{
  assert (instance < CMP_INSTANCE_COUNT);
    switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL2_SYNCCMP0SAMPLEWIN(base, 1);
        break;
    case 1:
        SIM_BWR_MISCTRL2_SYNCCMP1SAMPLEWIN(base, 1);
        break;
    case 2:
        SIM_BWR_MISCTRL2_SYNCCMP2SAMPLEWIN(base, 1);
        break;
    case 3:
        SIM_BWR_MISCTRL2_SYNCCMP3SAMPLEWIN(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSyncXbarCmpX
 * Description   : Disable the synchronizer between XBARA's output and CMP3's sample/window input
 * This function Disables the synchronizer between XBARA's output and CMP3's sample/window input
 *
 *END**************************************************************************/
void SIM_HAL_DisableSyncXbarCmpX(SIM_Type* base, uint32_t instance)
{
  assert (instance < CMP_INSTANCE_COUNT);
    switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL2_SYNCCMP0SAMPLEWIN(base, 0);
        break;
    case 1:
        SIM_BWR_MISCTRL2_SYNCCMP1SAMPLEWIN(base, 0);
        break;
    case 2:
        SIM_BWR_MISCTRL2_SYNCCMP2SAMPLEWIN(base, 0);
        break;
    case 3:
        SIM_BWR_MISCTRL2_SYNCCMP3SAMPLEWIN(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSyncXbarCmpXcmd
 * Description   : Get the synchronizer cmd between XBARA's output and CMP3's sample/window input
 *
 *END**************************************************************************/
bool SIM_HAL_GetSyncXbarCmpXcmd(SIM_Type* base, uint32_t instance)
{
  assert (instance < CMP_INSTANCE_COUNT);
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_MISCTRL2_SYNCCMP0SAMPLEWIN(base);
        break;
    case 1:
        retValue =  SIM_BRD_MISCTRL2_SYNCCMP1SAMPLEWIN(base);
        break;
    case 2:
        retValue =  SIM_BRD_MISCTRL2_SYNCCMP2SAMPLEWIN(base);
        break;
    case 3:
        retValue =  SIM_BRD_MISCTRL2_SYNCCMP3SAMPLEWIN(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetCmpWinxSrc
 * Description   : Set CMP Sample/Window Input X Source
 * This function controls the sample/window source of CMP module
 *
 *END**************************************************************************/
void SIM_HAL_SetCmpWinxSrc(SIM_Type* base, uint32_t instance,
                                      sim_cmp_win_in_src select)
{
  assert (instance < CMP_INSTANCE_COUNT);
  switch (instance)
    {
    case 0:
        SIM_BWR_MISCTRL_CMPWIN0SRC(base, select);
        break;
    case 1:
        SIM_BWR_MISCTRL_CMPWIN1SRC(base, select);
        break;
    case 2:
        SIM_BWR_MISCTRL_CMPWIN2SRC(base, select);
        break;
    case 3:
        SIM_BWR_MISCTRL_CMPWIN3SRC(base, select);
        break;    
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetCmpWinxSrc
 * Description   : Get CMP Sample/Window Input X Source
 * This function will get the sample/window source of CMP module
 *
 *END**************************************************************************/
sim_cmp_win_in_src SIM_HAL_GetCmpWinxSrc(SIM_Type* base, uint32_t instance)
{
  assert (instance < CMP_INSTANCE_COUNT);
    sim_cmp_win_in_src retValue = (sim_cmp_win_in_src)0;
    switch (instance)
    {
    case 0:
        retValue = (sim_cmp_win_in_src)SIM_BRD_MISCTRL_CMPWIN0SRC(base);
        break;
    case 1:
        retValue = (sim_cmp_win_in_src)SIM_BRD_MISCTRL_CMPWIN1SRC(base);
        break;
    case 2:
        retValue = (sim_cmp_win_in_src)SIM_BRD_MISCTRL_CMPWIN2SRC(base);
        break;
    case 3:
        retValue = (sim_cmp_win_in_src)SIM_BRD_MISCTRL_CMPWIN3SRC(base);
        break;
    default:
        break;
    }

    return retValue;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetEwmInSrc
 * Description   : Set EWM_IN source setting
 * This function controls the ewm_in source of EWM module
 *
 *END**************************************************************************/
void SIM_HAL_SetEwmInSrc(SIM_Type* base, sim_ewm_in_src select)
{
        SIM_BWR_MISCTRL_EWMINSRC(base, select);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetEwmInSrc
 * Description   : Get EWM_IN source setting
 * This function will get the ewm_in source of EWM module
 *
 *END**************************************************************************/
sim_ewm_in_src SIM_HAL_GetEwmInSrc(SIM_Type* base)
{
   return (sim_ewm_in_src)SIM_BRD_MISCTRL_EWMINSRC(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetDacHwTrigSrc
 * Description   : Set DAC x Hardware trigger source setting
 * This function will select the DAC0 Hardware Trigger Input Source
 *
 *END**************************************************************************/
void SIM_HAL_SetDacHwTrigSrc(SIM_Type* base,sim_dac_hw_trg_sel select)
{
        SIM_BWR_MISCTRL_DACTRIGSRC(base, select);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetDacHwTrigSrc
 * Description   : Get DAC x Hardware trigger source setting
 * This function will get the DAC0 Hardware Trigger Input Source
 *
 *END**************************************************************************/
sim_dac_hw_trg_sel SIM_HAL_GetDacHwTrigSrc(SIM_Type* base, uint32_t instance)
{
        return (sim_dac_hw_trg_sel)SIM_BRD_MISCTRL_DACTRIGSRC(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmExternalClkPinMode
 * Description   : Set FlexTimer x external clock pin select setting
 * This function will select the source of FTMx external clock pin select
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmExternalClkPinMode(SIM_Type* base,
                                      uint32_t instance,
                                      sim_ftm_clk_sel_t select)
{
    assert (instance < FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        SIM_BWR_SOPT9_FTM0CLKSEL(base, select);
        break;
    case 1:
        SIM_BWR_SOPT9_FTM1CLKSEL(base, select);
        break;
    case 3:
        SIM_BWR_SOPT9_FTM3CLKSEL(base, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmExternalClkPinMode
 * Description   : Get FlexTimer x external clock pin select setting
 * This function will get FlexTimer x external clock pin select setting.
 *
 *END**************************************************************************/
sim_ftm_clk_sel_t SIM_HAL_GetFtmExternalClkPinMode(SIM_Type* base,
                                                   uint32_t instance)
{
    sim_ftm_clk_sel_t retValue = (sim_ftm_clk_sel_t)0;

    assert (instance < FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_ftm_clk_sel_t)SIM_BRD_SOPT9_FTM0CLKSEL(base);
        break;
    case 1:
        retValue = (sim_ftm_clk_sel_t)SIM_BRD_SOPT9_FTM1CLKSEL(base);
        break;
    case 3:
        retValue = (sim_ftm_clk_sel_t)SIM_BRD_SOPT9_FTM3CLKSEL(base);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmChSrcMode
 * Description   : FlexTimer x channel y input capture source select setting
 * This function will select FlexTimer x channel y input capture source
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmChSrcMode(SIM_Type* base,
                             uint32_t instance,
                             uint8_t  channel,
                             sim_ftm_ch_src_t select)
{
    assert (instance < FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 1:
        switch (channel)
        {
        case 0:
            SIM_BWR_SOPT9_FTM1ICH0SRC(base, select);
            break;
        case 1:
            SIM_BWR_SOPT9_FTM1ICH1SRC(base, select);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmChSrcMode
 * Description   : Get FlexTimer x channel y input capture source select setting
 * This function will get FlexTimer x channel y input capture source select
 * setting.
 *
 *END**************************************************************************/
sim_ftm_ch_src_t SIM_HAL_GetFtmChSrcMode(SIM_Type* base,
                                         uint32_t instance,
                                         uint8_t channel)
{
    sim_ftm_ch_src_t retValue = (sim_ftm_ch_src_t)false;

    assert (instance < FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 1:
        switch (channel)
        {
        case 0:
            retValue = (sim_ftm_ch_src_t)SIM_BRD_SOPT9_FTM1ICH0SRC(base);
            break;
        case 1:
            retValue = (sim_ftm_ch_src_t)SIM_BRD_SOPT9_FTM1ICH1SRC(base);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmChOutSrcMode
 * Description   : FlexTimer x channel y output source select setting.
 * This function will select FlexTimer x channel y output source
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmChOutSrcMode(SIM_Type* base,
                                uint32_t instance,
                                uint8_t channel,
                                sim_ftm_ch_out_src_t select)
{
    switch(instance)
    {
    case 0:
      switch(channel)
          {
          case 0:
            SIM_BWR_SOPT8_FTM0OCH0SRC(base, select);
            break;
          case 1:
            SIM_BWR_SOPT8_FTM0OCH1SRC(base, select);
            break;  
          case 2:
            SIM_BWR_SOPT8_FTM0OCH2SRC(base, select);
            break;
          case 3:
            SIM_BWR_SOPT8_FTM0OCH3SRC(base, select);
            break;
          case 4:
            SIM_BWR_SOPT8_FTM0OCH4SRC(base, select);
            break;  
          case 5:
            SIM_BWR_SOPT8_FTM0OCH5SRC(base, select);
            break;
          case 6:
            SIM_BWR_SOPT8_FTM0OCH6SRC(base, select);
            break;
          case 7:
            SIM_BWR_SOPT8_FTM0OCH7SRC(base, select);
            break;  
          default:
            break;
          }
          break;
    case 3:
      switch(channel)
          {
          case 0:
            SIM_BWR_SOPT8_FTM3OCH0SRC(base, select);
            break;
          case 1:
            SIM_BWR_SOPT8_FTM3OCH1SRC(base, select);
            break;  
          case 2:
            SIM_BWR_SOPT8_FTM3OCH2SRC(base, select);
            break;
          case 3:
            SIM_BWR_SOPT8_FTM3OCH3SRC(base, select);
            break;
          case 4:
            SIM_BWR_SOPT8_FTM3OCH4SRC(base, select);
            break;  
          case 5:
            SIM_BWR_SOPT8_FTM3OCH5SRC(base, select);
            break;
          case 6:
            SIM_BWR_SOPT8_FTM3OCH6SRC(base, select);
            break;
          case 7:
            SIM_BWR_SOPT8_FTM3OCH7SRC(base, select);
            break;
          default:
            break;
          }
          break;
    default:
      break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmChOutSrcMode
 * Description   : Get FlexTimer x channel y output source select setting
 * This function will get FlexTimer x channel y output source select
 * setting.
 *
 *END**************************************************************************/
sim_ftm_ch_out_src_t SIM_HAL_GetFtmChOutSrcMode(SIM_Type* base,
                                                uint32_t instance,
                                                uint8_t channel)
{
  sim_ftm_ch_out_src_t retValue = (sim_ftm_ch_out_src_t) false;
  switch(instance)
    {
    case 0:
      switch(channel)
          {
          case 0:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH0SRC(base);
            break;
          case 1:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH1SRC(base);
            break;  
          case 2:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH2SRC(base);
            break;
          case 3:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH3SRC(base);
            break;
          case 4:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH4SRC(base);
            break;  
          case 5:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH5SRC(base);
            break;
          case 6:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH6SRC(base);
            break;
          case 7:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM0OCH7SRC(base);
            break;  
          default:
            retValue = (sim_ftm_ch_out_src_t) false;
            break;
          }
          break;
    case 3:
      switch(channel)
          {
          case 0:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH0SRC(base);
            break;
          case 1:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH1SRC(base);
            break;  
          case 2:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH2SRC(base);
            break;
          case 3:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH3SRC(base);
            break;
          case 4:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH4SRC(base);
            break;  
          case 5:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH5SRC(base);
            break;
          case 6:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH6SRC(base);
            break;
          case 7:
            retValue = (sim_ftm_ch_out_src_t)SIM_BRD_SOPT8_FTM3OCH7SRC(base);
            break;  
          default:
            retValue = (sim_ftm_ch_out_src_t) false;
            break;
          }
          break;
    default:
      retValue = (sim_ftm_ch_out_src_t) false;
      break;
    }
  return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmCarrierFreqCmd
 * Description   : Set FTMxCFSELBIT
 * This function sets FlexTimer x carrier frequency selection software configuration setting.
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmCarrierFreqCmd(SIM_Type* base, uint32_t instance, bool ftmcarrierfreqsrc)
{
    assert (instance < FTM_INSTANCE_COUNT);
    switch(instance)
    {
    case 0:
      SIM_BWR_SOPT8_FTM0CFSEL(base,ftmcarrierfreqsrc);
      break;
    case 3:
      SIM_BWR_SOPT8_FTM3CFSEL(base,ftmcarrierfreqsrc);
      break;
    default:
      break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmCarrierFreqCmd
 * Description   : Get FlexTimer x carrier frequency selection software configuration setting.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFtmCarrierFreqCmd(SIM_Type* base, uint32_t instance)
{
  bool retValue = false;
   assert (instance < FTM_INSTANCE_COUNT);
    switch(instance)
    {
    case 0:
      retValue = SIM_BRD_SOPT8_FTM0CFSEL(base);
      break;
    case 3:
      retValue = SIM_BRD_SOPT8_FTM3CFSEL(base);
      break;
    default:
      retValue = false;
      break;
    }
    return retValue;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmSyncCmd
 * Description   : Set FTMxSYNCBIT
 * This function sets FlexTimer x hardware trigger 0 software synchronization.
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmSyncCmd(SIM_Type* base, uint32_t instance, bool sync)
{
    assert (instance < FTM_INSTANCE_COUNT);
    if (sync)
    {
        SIM_SET_SOPT8(base, (1U<<instance));
    }
    else
    {
        SIM_CLR_SOPT8(base, (1U<<instance));
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcAlternativeTriggerCmd
 * Description   : Set ADCx alternate trigger enable setting
 * This function will enable/disable alternative conversion triggers for ADCx.
 *
 *END**************************************************************************/
void SIM_HAL_SetAdcAlternativeTriggerCmd(SIM_Type* base,
                                         sim_cadc_conv_id_t convId,
                                         sim_adc_alt_trg_en enable)
{
    switch (convId)
    {
    case kSimCAdcConvA:
        SIM_BWR_SOPT7_ADCAALTTRGEN(base, enable);
        break;
    case kSimCAdcConvB:
        SIM_BWR_SOPT7_ADCBALTTRGEN(base, enable);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcAlternativeTriggerCmd
 * Description   : Get ADCx alternate trigger enable setting
 * This function will get ADCx alternate trigger enable setting.
 *
 *END**************************************************************************/
sim_adc_alt_trg_en SIM_HAL_GetAdcAlternativeTriggerCmd(SIM_Type* base, sim_cadc_conv_id_t convId)
{
    sim_adc_alt_trg_en retValue = (sim_adc_alt_trg_en)false;

    switch (convId)
    {
    case kSimCAdcConvA:
        retValue = (sim_adc_alt_trg_en)SIM_BRD_SOPT7_ADCAALTTRGEN(base);
        break;
    case kSimCAdcConvB:
        retValue = (sim_adc_alt_trg_en)SIM_BRD_SOPT7_ADCBALTTRGEN(base);
        break;
    default:
        retValue = (sim_adc_alt_trg_en)false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcTriggerMode
 * Description   : Set ADCx trigger select setting
 * This function will select the ADCx trigger source when alternative triggers
 * are enabled through ADCxALTTRGEN
 *
 *END**************************************************************************/
void SIM_HAL_SetAdcTriggerMode(SIM_Type* base,
                               sim_cadc_conv_id_t convId,
                               sim_adc_trg_sel_t select)
{
    switch (convId)
    {
    case kSimCAdcConvA:
        SIM_BWR_SOPT7_ADCATRGSEL(base, select);
        break;
    case kSimCAdcConvB:
        SIM_BWR_SOPT7_ADCBTRGSEL(base, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAdcTriggerMode
 * Description   : Get ADCx trigger select setting
 * This function will get ADCx trigger select setting.
 *
 *END**************************************************************************/
sim_adc_trg_sel_t SIM_HAL_GetAdcTriggerMode(SIM_Type* base, sim_cadc_conv_id_t convId)
{
    sim_adc_trg_sel_t retValue = (sim_adc_trg_sel_t)0;
    
    switch (convId)
    {
    case kSimCAdcConvA:
        retValue = (sim_adc_trg_sel_t)SIM_BRD_SOPT7_ADCATRGSEL(base);
        break;
    case kSimCAdcConvB:
        retValue = (sim_adc_trg_sel_t)SIM_BRD_SOPT7_ADCBTRGSEL(base);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetAdcTriggerModeOneStep
 * Description   : Set ADCx trigger setting.
 * This function sets ADC alternate trigger and trigger mode.
 *
 *END**************************************************************************/
void SIM_HAL_SetAdcTriggerModeOneStep(SIM_Type* base,
                                      sim_cadc_conv_id_t convId,
                                      bool    altTrigEn,
                                      sim_adc_trg_sel_t trigSel)
{
    switch (convId)
    {
        case kSimCAdcConvA:
            SIM_BWR_SOPT7_ADCAALTTRGEN(base, altTrigEn ? 1 : 0);
            break;
        case kSimCAdcConvB:
            SIM_BWR_SOPT7_ADCBALTTRGEN(base, altTrigEn ? 1 : 0);
            break;
    default:
        break;
    }

    if (altTrigEn)
    {
        switch (convId)
        {
            case kSimCAdcConvA:
                SIM_BWR_SOPT7_ADCATRGSEL(base, trigSel);
                break;
            case kSimCAdcConvB:
                SIM_BWR_SOPT7_ADCBTRGSEL(base, trigSel);
                break;
            default:
                break;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetUartRxSrcMode
 * Description   : Set UARTx receive data source select setting
 * This function will select the source for the UART1 receive data.
 *
 *END**************************************************************************/
void SIM_HAL_SetUartRxSrcMode(SIM_Type* base,
                              uint32_t instance,
                              sim_uart_rxsrc_t select)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        SIM_BWR_SOPT5_UART0RXSRC(base, select);
        break;
    case 1:
        SIM_BWR_SOPT5_UART1RXSRC(base, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUartRxSrcMode
 * Description   : Get UARTx receive data source select setting
 * This function will get UARTx receive data source select setting.
 *
 *END**************************************************************************/
sim_uart_rxsrc_t SIM_HAL_GetUartRxSrcMode(SIM_Type* base, uint32_t instance)
{
    sim_uart_rxsrc_t retValue = (sim_uart_rxsrc_t)0;

    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_uart_rxsrc_t)SIM_BRD_SOPT5_UART0RXSRC(base);
        break;
    case 1:
        retValue = (sim_uart_rxsrc_t)SIM_BRD_SOPT5_UART1RXSRC(base);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetUartTxSrcMode
 * Description   : Set UARTx transmit data source select setting
 * This function will select the source for the UARTx transmit data.
 *
 *END**************************************************************************/
void SIM_HAL_SetUartTxSrcMode(SIM_Type* base,
                              uint32_t instance,
                              sim_uart_txsrc_t select)
{
    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        SIM_BWR_SOPT5_UART0TXSRC(base, select);
        break;
    case 1:
        SIM_BWR_SOPT5_UART1TXSRC(base, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUartTxSrcMode
 * Description   : Get UARTx transmit data source select setting
 * This function will get UARTx transmit data source select setting.
 *
 *END**************************************************************************/
sim_uart_txsrc_t SIM_HAL_GetUartTxSrcMode(SIM_Type* base, uint32_t instance)
{
    sim_uart_txsrc_t retValue =(sim_uart_txsrc_t)0;

    assert(instance < FSL_FEATURE_SIM_OPT_UART_COUNT);

    switch (instance)
    {
    case 0:
        retValue = (sim_uart_txsrc_t)SIM_BRD_SOPT5_UART0TXSRC(base);
        break;
    case 1:
        retValue = (sim_uart_txsrc_t)SIM_BRD_SOPT5_UART1TXSRC(base);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmTriggerSrcMode
 * Description   : Set FlexTimer x hardware trigger y source select setting
 * This function will select the source of FTMx hardware trigger y.
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmTriggerSrcMode(SIM_Type* base,
                                  uint32_t instance,
                                  uint8_t  trigger,
                                  sim_ftm_trg_src_t select)
{
    assert (instance < FTM_INSTANCE_COUNT);
    assert (trigger < FSL_FEATURE_SIM_OPT_FTM_TRIGGER_COUNT);

    switch (instance)
    {
    case 0:
        switch (trigger)
        {
        case 0:
            SIM_BWR_SOPT4_FTM0TRG0SRC(base, select);
            break;
        case 1:
            SIM_BWR_SOPT4_FTM0TRG1SRC(base, select);
            break;
        case 2:
            SIM_BWR_SOPT4_FTM0TRG2SRC(base, select);
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (trigger)
        {
        case 0:
            SIM_BWR_SOPT4_FTM1TRG0SRC(base, select);
            break;
        case 2:
            SIM_BWR_SOPT4_FTM1TRG2SRC(base, select);
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (trigger)
        {
        case 0:
            SIM_BWR_SOPT4_FTM3TRG0SRC(base, select);
            break;
        case 1:
            SIM_BWR_SOPT4_FTM3TRG1SRC(base, select);
            break;
        case 2:
            SIM_BWR_SOPT4_FTM3TRG2SRC(base, select);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmTriggerSrcMode
 * Description   : Get FlexTimer x hardware trigger y source select setting
 * This function will get FlexTimer x hardware trigger y source select setting.
 *
 *END**************************************************************************/
sim_ftm_trg_src_t SIM_HAL_GetFtmTriggerSrcMode(SIM_Type* base,
                                               uint32_t instance,
                                               uint8_t trigger)
{
    sim_ftm_trg_src_t retValue = (sim_ftm_trg_src_t)0;

    assert (instance < FTM_INSTANCE_COUNT);
    assert (trigger < FSL_FEATURE_SIM_OPT_FTM_TRIGGER_COUNT);

    switch (instance)
    {
    case 0:
        switch (trigger)
        {
        case 0:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM0TRG0SRC(base);
            break;
        case 1:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM0TRG1SRC(base);
            break;
        case 2:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM0TRG2SRC(base);
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (trigger)
        {
        case 0:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM1TRG0SRC(base);
            break;
        case 2:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM1TRG2SRC(base);
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (trigger)
        {
        case 0:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM3TRG0SRC(base);
            break;
        case 1:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM3TRG1SRC(base);
            break;
        case 2:
            retValue = (sim_ftm_trg_src_t)SIM_BRD_SOPT4_FTM3TRG2SRC(base);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_SetFtmFaultSelMode
 * Description   : Set FlexTimer x fault y select setting
 * This function will set the FlexTimer x fault y select setting.
 *
 *END**************************************************************************/
void SIM_HAL_SetFtmFaultSelMode(SIM_Type* base,
                                uint8_t instance,
                                uint8_t  fault,
                                sim_ftm_flt_sel_t select)
{
    assert (instance < FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        switch (fault)
        {
        case 0:
            SIM_BWR_SOPT4_FTM0FLT0(base, select);
            break;
        case 1:
            SIM_BWR_SOPT4_FTM0FLT1(base, select);
            break;
        case 2:
            SIM_BWR_SOPT4_FTM0FLT2(base, select);
            break;
        case 3:
            SIM_BWR_SOPT4_FTM0FLT3(base, select);
            break;
        default:
            break;
        }
        break;
    case 1:
        SIM_BWR_SOPT4_FTM1FLT0(base, select);
        break;
    case 3:
        SIM_BWR_SOPT4_FTM3FLT0(base, select);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFtmFaultSelMode
 * Description   : Get FlexTimer x fault y select setting
 * This function will get FlexTimer x fault y select setting.
 *
 *END**************************************************************************/
sim_ftm_flt_sel_t SIM_HAL_GetFtmFaultSelMode(SIM_Type* base,
                                             uint8_t instance,
                                             uint8_t fault)
{
    sim_ftm_flt_sel_t retValue = (sim_ftm_flt_sel_t)0;

    assert (instance < FTM_INSTANCE_COUNT);

    switch (instance)
    {
    case 0:
        switch (fault)
        {
        case 0:
            retValue = (sim_ftm_flt_sel_t)SIM_BRD_SOPT4_FTM0FLT0(base);
            break;
        case 1:
            retValue = (sim_ftm_flt_sel_t)SIM_BRD_SOPT4_FTM0FLT1(base);
            break;
        case 2:
            retValue = (sim_ftm_flt_sel_t)SIM_BRD_SOPT4_FTM0FLT2(base);
            break;
        case 3:
            retValue = (sim_ftm_flt_sel_t)SIM_BRD_SOPT4_FTM0FLT3(base);
            break;    
        default:
            break;
        }
        break;
    case 1:
        retValue = (sim_ftm_flt_sel_t)SIM_BRD_SOPT4_FTM1FLT0(base);
        break;
    case 3:
        retValue = (sim_ftm_flt_sel_t)SIM_BRD_SOPT4_FTM3FLT0(base);
        break;
    default:
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePitClock
 * Description   : Enable the clock for PIT module
 * This function enables the clock for PIT moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePitClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_PIT(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePitClock
 * Description   : Disable the clock for PIT module
 * This function disables the clock for PIT moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePitClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC6_PIT(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPitGateCmd
 * Description   : Get the the clock gate state for PIT module
 * This function will get the clock gate state for PIT moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPitGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC6_PIT(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableLptmrClock
 * Description   : Enable the clock for LPTIMER module
 * This function enables the clock for LPTIMER moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableLptmrClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_LPTMR(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableLptmrClock
 * Description   : Disable the clock for LPTIMER module
 * This function disables the clock for LPTIMER moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableLptmrClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_LPTMR(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetLptmrGateCmd
 * Description   : Get the the clock gate state for LPTIMER module
 * This function will get the clock gate state for LPTIMER moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetLptmrGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC5_LPTMR(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableFlexcanClock
 * Description   : Enable the clock for FLEXCAN module
 * This function enables the clock for FLEXCAN moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableFlexcanClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC6_FLEXCAN0(base, 1);
        break;
    case 1:
        SIM_BWR_SCGC6_FLEXCAN1(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableFlexcanClock
 * Description   : Disable the clock for FLEXCAN module
 * This function disables the clock for FLEXCAN moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableFlexcanClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC6_FLEXCAN0(base, 0);
        break;
    case 1:
        SIM_BWR_SCGC6_FLEXCAN1(base, 0);
        break;
    default:
        break;
    }
    
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetFlexcanGateCmd
 * Description   : Get the the clock gate state for FLEXCAN module
 * This function will get the clock gate state for FLEXCAN moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetFlexcanGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC6_FLEXCAN0(base);
        break;
    case 1:
        retValue =  SIM_BRD_SCGC6_FLEXCAN1(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableSpiClock
 * Description   : Enable the clock for SPI module
 * This function enables the clock for SPI moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableSpiClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC6_SPI0(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableSpiClock
 * Description   : Disable the clock for SPI module
 * This function disables the clock for SPI moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableSpiClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC6_SPI0(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetSpiGateCmd
 * Description   : Get the the clock gate state for SPI module
 * This function will get the clock gate state for SPI moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetSpiGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC6_SPI0(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableI2cClock
 * Description   : Enable the clock for I2C module
 * This function enables the clock for I2C moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableI2cClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC4_I2C0(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableI2cClock
 * Description   : Disable the clock for I2C module
 * This function disables the clock for I2C moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableI2cClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC4_I2C0(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetI2cGateCmd
 * Description   : Get the the clock gate state for I2C module
 * This function will get the clock gate state for I2C moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetI2cGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC4_I2C0(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableUartClock
 * Description   : Enable the clock for UART module
 * This function enables the clock for UART moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableUartClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC4_UART0(base, 1);
        break;
    case 1:
        SIM_BWR_SCGC4_UART1(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableUartClock
 * Description   : Disable the clock for UART module
 * This function disables the clock for UART moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableUartClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC4_UART0(base, 0);
        break;
    case 1:
        SIM_BWR_SCGC4_UART1(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetUartGateCmd
 * Description   : Get the the clock gate state for UART module
 * This function will get the clock gate state for UART moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetUartGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC4_UART0(base);
        break;
    case 1:
        retValue =  SIM_BRD_SCGC4_UART1(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnablePwmClock
 * Description   : Enable the clock for eFlexPWM module
 * This function enables the clock for eFlexPWM moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnablePwmClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC4_eFlexPWM0(base, 1);
        break;
    case 1:
        SIM_BWR_SCGC4_eFlexPWM1(base, 1);
        break;
    case 2:
        SIM_BWR_SCGC4_eFlexPWM2(base, 1);
        break;
    case 3:
        SIM_BWR_SCGC4_eFlexPWM3(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisablePwmClock
 * Description   : Disable the clock for eFlexPWM module
 * This function disables the clock for eFlexPWM moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisablePwmClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC4_eFlexPWM0(base, 0);
        break;
    case 1:
        SIM_BWR_SCGC4_eFlexPWM1(base, 0);
        break;
    case 2:
        SIM_BWR_SCGC4_eFlexPWM2(base, 0);
        break;
    case 3:
        SIM_BWR_SCGC4_eFlexPWM3(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetPwmGateCmd
 * Description   : Get the the clock gate state for eFlexPWM module
 * This function will get the clock gate state for eFlexPWM moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetPwmGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC4_eFlexPWM0(base);
        break;
    case 1:
        retValue =  SIM_BRD_SCGC4_eFlexPWM1(base);
        break;
    case 2:
        retValue =  SIM_BRD_SCGC4_eFlexPWM2(base);
        break;
    case 3:
        retValue =  SIM_BRD_SCGC4_eFlexPWM3(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableAoiClock
 * Description   : Enable the clock for AOI module
 * This function enables the clock for AOI moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableAoiClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_AOI(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableAoiClock
 * Description   : Disable the clock for AOI module
 * This function disables the clock for AOI moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableAoiClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_AOI(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetAoiGateCmd
 * Description   : Get the the clock gate state for AOI module
 * This function will get the clock gate state for AOI moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetAoiGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC5_AOI(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableXbarClock
 * Description   : Enable the clock for XBAR module
 * This function enables the clock for XBAR moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableXbarClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC5_XBARA(base, 1);
        break;
    case 1:
        SIM_BWR_SCGC5_XBARB(base, 1);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableXbarClock
 * Description   : Disable the clock for XBAR module
 * This function disables the clock for XBAR moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableXbarClock(SIM_Type* base, uint32_t instance)
{
    switch (instance)
    {
    case 0:
        SIM_BWR_SCGC5_XBARA(base, 0);
        break;
    case 1:
        SIM_BWR_SCGC5_XBARB(base, 0);
        break;
    default:
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetXbarGateCmd
 * Description   : Get the the clock gate state for XBAR module
 * This function will get the clock gate state for XBAR moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetXbarGateCmd(SIM_Type* base, uint32_t instance)
{
    bool retValue = false;

    switch (instance)
    {
    case 0:
        retValue =  SIM_BRD_SCGC5_XBARA(base);
        break;
    case 1:
        retValue =  SIM_BRD_SCGC5_XBARB(base);
        break;
    default:
        retValue =  false;
        break;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_EnableEncClock
 * Description   : Enable the clock for ENC module
 * This function enables the clock for ENC moudle
 *
 *END**************************************************************************/
void SIM_HAL_EnableEncClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_ENC(base, 1);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_DisableEncClock
 * Description   : Disable the clock for ENC module
 * This function disables the clock for ENC moudle
 *
 *END**************************************************************************/
void SIM_HAL_DisableEncClock(SIM_Type* base, uint32_t instance)
{
    SIM_BWR_SCGC5_ENC(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SIM_HAL_GetEncGateCmd
 * Description   : Get the the clock gate state for ENC module
 * This function will get the clock gate state for ENC moudle.
 *
 *END**************************************************************************/
bool SIM_HAL_GetEncGateCmd(SIM_Type* base, uint32_t instance)
{
    return SIM_BRD_SCGC5_ENC(base);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

