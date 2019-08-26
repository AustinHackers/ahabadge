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
#include "fsl_lpuart_hal.h"

#if FSL_FEATURE_SOC_LPUART_COUNT

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_Init
 * Description   : Initializes the LPUART controller to known state.
 *
 *END**************************************************************************/
void LPUART_HAL_Init(LPUART_Type * base)
{
    LPUART_WR_BAUD(base, 0x0F000004);
    LPUART_WR_STAT(base, 0xC01FC000);
    LPUART_WR_CTRL(base, 0x00000000);
    LPUART_WR_MATCH(base, 0x00000000);
#if FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
    LPUART_WR_MODIR(base, 0x00000000);
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetBaudRate
 * Description   : Configures the LPUART baud rate.
 * In some LPUART instances the user must disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_HAL_SetBaudRate(LPUART_Type * base,
                                       uint32_t sourceClockInHz,
                                       uint32_t desiredBaudRate)
{
    uint16_t sbr, sbrTemp, i;
    uint32_t osr, tempDiff, calculatedBaud, baudDiff;

    /* This lpuart instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, osr is typically hard-set to 16 in other lpuart instantiations
     * First calculate the baud rate using the minimum OSR possible (4) */
    osr = 4;
    sbr = (sourceClockInHz/(desiredBaudRate * osr));
    calculatedBaud = (sourceClockInHz / (osr * sbr));

    if (calculatedBaud > desiredBaudRate)
    {
        baudDiff = calculatedBaud - desiredBaudRate;
    }
    else
    {
        baudDiff = desiredBaudRate - calculatedBaud;
    }

    /* loop to find the best osr value possible, one that generates minimum baudDiff
     * iterate through the rest of the supported values of osr */
    for (i = 5; i <= 32; i++)
    {
        /* calculate the temporary sbr value   */
        sbrTemp = (sourceClockInHz/(desiredBaudRate * i));
        /* calculate the baud rate based on the temporary osr and sbr values */
        calculatedBaud = (sourceClockInHz / (i * sbrTemp));

        if (calculatedBaud > desiredBaudRate)
        {
            tempDiff = calculatedBaud - desiredBaudRate;
        }
        else
        {
            tempDiff = desiredBaudRate - calculatedBaud;
        }

        if (tempDiff <= baudDiff)
        {
            baudDiff = tempDiff;
            osr = i;  /* update and store the best osr value calculated */
            sbr = sbrTemp;  /* update store the best sbr value calculated */
        }
    }

    /* Check to see if actual baud rate is within 3% of desired baud rate
     * based on the best calculate osr value */
    if (baudDiff < ((desiredBaudRate / 100) * 3))
    {
        /* Acceptable baud rate, check if osr is between 4x and 7x oversampling.
         * If so, then "BOTHEDGE" sampling must be turned on */
        if ((osr > 3) && (osr < 8))
        {
            LPUART_BWR_BAUD_BOTHEDGE(base, 1);
        }

        /* program the osr value (bit value is one less than actual value) */
        LPUART_BWR_BAUD_OSR(base, (osr-1));

        /* write the sbr value to the BAUD registers */
        LPUART_BWR_BAUD_SBR(base, sbr);
    }
    else
    {
        /* Unacceptable baud rate difference of more than 3% */
        return kStatus_LPUART_BaudRateCalculationError;
    }

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetBitCountPerChar
 * Description   : Configures the number of bits per char in LPUART controller.
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 *END**************************************************************************/
void LPUART_HAL_SetBitCountPerChar(LPUART_Type * base,
                                   lpuart_bit_count_per_char_t bitCountPerChar)
{
    if (bitCountPerChar == kLpuart10BitsPerChar)
    {
        LPUART_BWR_BAUD_M10(base, 0x1U);
    }
    else
    {
        /* config 8-bit (M=0) or 9-bits (M=1) */
        LPUART_BWR_CTRL_M(base, bitCountPerChar);
        /* clear M10 to make sure not 10-bit mode */
        LPUART_BWR_BAUD_M10(base, 0x0U);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetParityMode
 * Description   : Configures parity mode in the LPUART controller.
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 *END**************************************************************************/
void LPUART_HAL_SetParityMode(LPUART_Type * base, lpuart_parity_mode_t parityModeType)
{
    LPUART_BWR_CTRL_PE(base, parityModeType >> 1U);
    LPUART_BWR_CTRL_PT(base, parityModeType & 1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_Putchar9
 * Description   : Sends the LPUART 9-bit character.
 *
 *END**************************************************************************/
void LPUART_HAL_Putchar9(LPUART_Type * base, uint16_t data)
{
    uint8_t ninthDataBit;

    ninthDataBit = (data >> 8U) & 0x1U;

    /* write to ninth data bit T8(where T[0:7]=8-bits, T8=9th bit) */
    LPUART_BWR_CTRL_R9T8(base, ninthDataBit);

    /* write 8-bits to the data register*/
    LPUART_WR_DATA(base, (uint8_t)data);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_Putchar10
 * Description   : Sends the LPUART 10-bit character.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_HAL_Putchar10(LPUART_Type * base, uint16_t data)
{
    uint8_t ninthDataBit, tenthDataBit;

    ninthDataBit = (data >> 8U) & 0x1U;
    tenthDataBit = (data >> 9U) & 0x1U;

    /* write to ninth/tenth data bit (T[0:7]=8-bits, T8=9th bit, T9=10th bit) */
    LPUART_BWR_CTRL_R9T8(base, ninthDataBit);
    LPUART_BWR_CTRL_R8T9(base, tenthDataBit);

    /* write to 8-bits to the data register */
    LPUART_WR_DATA(base, (uint8_t)data);

    return kStatus_LPUART_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_Getchar9
 * Description   : Gets the LPUART 9-bit character.
 *
 *END**************************************************************************/
void LPUART_HAL_Getchar9(LPUART_Type * base, uint16_t *readData)
{
    /* get ninth bit from lpuart data register */
    *readData = (uint16_t)LPUART_BRD_CTRL_R8T9(base) << 8;

    /* get 8-bit data from the lpuart data register */
    *readData |= (uint8_t)LPUART_RD_DATA(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_Getchar10
 * Description   : Gets the LPUART 10-bit character, available only on
 *                 supported lpuarts
 *
 *END**************************************************************************/
void LPUART_HAL_Getchar10(LPUART_Type * base, uint16_t *readData)
{
    /* read tenth data bit */
    *readData = (uint16_t)((uint32_t)(LPUART_BRD_CTRL_R9T8(base)) << 9U);

    /* read ninth data bit */
    *readData |= (uint16_t)((uint32_t)(LPUART_BRD_CTRL_R8T9(base)) << 8U);

    /* get 8-bit data */
    *readData |= LPUART_RD_DATA(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SendDataPolling
 * Description   : Send out multiple bytes of data using polling method.
 * This function only supports 8-bit transaction.
 *
 *END**************************************************************************/
void LPUART_HAL_SendDataPolling(LPUART_Type * base,
                                const uint8_t *txBuff,
                                uint32_t txSize)
{
    while (txSize--)
    {
        while (!LPUART_BRD_STAT_TDRE(base))
        {}

        LPUART_HAL_Putchar(base, *txBuff++);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_ReceiveDataPolling
 * Description   : Receive multiple bytes of data using polling method.
 * This function only supports 8-bit transaction.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_HAL_ReceiveDataPolling(LPUART_Type * base,
                                              uint8_t *rxBuff,
                                              uint32_t rxSize)
{
    lpuart_status_t retVal = kStatus_LPUART_Success;

    while (rxSize--)
    {
        while (!LPUART_BRD_STAT_RDRF(base))
        {}

        LPUART_HAL_Getchar(base, rxBuff++);

        /* Clear the Overrun flag since it will block receiving */
        if (LPUART_BRD_STAT_OR(base))
        {
            LPUART_BWR_STAT_OR(base, 1U);
            retVal = kStatus_LPUART_RxOverRun;
        }
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetIntMode
 * Description   : Configures the LPUART module interrupts to enable/disable
 * various interrupt sources.
 *
 *END**************************************************************************/
void LPUART_HAL_SetIntMode(LPUART_Type * base, lpuart_interrupt_t interrupt, bool enable)
{
    uint32_t reg = (uint32_t)(interrupt) >> LPUART_SHIFT;
    uint32_t temp = 1U << (uint32_t)interrupt;

    switch ( reg )
    {
        case LPUART_BAUD_REG_ID:
            enable ? LPUART_SET_BAUD(base, temp) : LPUART_CLR_BAUD(base, temp);
            break;
        case LPUART_STAT_REG_ID:
            enable ? LPUART_SET_STAT(base, temp) : LPUART_CLR_STAT(base, temp);
            break;
        case LPUART_CTRL_REG_ID:
            enable ? LPUART_SET_CTRL(base, temp) : LPUART_CLR_CTRL(base, temp);
            break;
        case LPUART_DATA_REG_ID:
            enable ? LPUART_SET_DATA(base, temp) : LPUART_CLR_DATA(base, temp);
            break;
        case LPUART_MATCH_REG_ID:
            enable ? LPUART_SET_MATCH(base, temp) : LPUART_CLR_MATCH(base, temp);
            break;
#if FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
        case LPUART_MODIR_REG_ID:
            enable ? LPUART_SET_MODIR(base, temp) : LPUART_CLR_MODIR(base, temp);
            break;
#endif
        default :
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_GetIntMode
 * Description   : Returns whether LPUART module interrupts is enabled/disabled.
 *
 *END**************************************************************************/
bool LPUART_HAL_GetIntMode(LPUART_Type * base, lpuart_interrupt_t interrupt)
{
    uint32_t reg = (uint32_t)(interrupt) >> LPUART_SHIFT;
	  bool retVal = false;

    switch ( reg )
    {
        case LPUART_BAUD_REG_ID:
            retVal = LPUART_RD_BAUD(base) >> (uint32_t)(interrupt) & 1U;
            break;
        case LPUART_STAT_REG_ID:
            retVal = LPUART_RD_STAT(base) >> (uint32_t)(interrupt) & 1U;
            break;
        case LPUART_CTRL_REG_ID:
            retVal = LPUART_RD_CTRL(base) >> (uint32_t)(interrupt) & 1U;
            break;
        case LPUART_DATA_REG_ID:
            retVal = LPUART_RD_DATA(base) >> (uint32_t)(interrupt) & 1U;
            break;
        case LPUART_MATCH_REG_ID:
            retVal = LPUART_RD_MATCH(base) >> (uint32_t)(interrupt) & 1U;
            break;
#if FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
        case LPUART_MODIR_REG_ID:
            retVal = LPUART_RD_MODIR(base) >> (uint32_t)(interrupt) & 1U;
            break;
#endif
        default :
            break;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetLoopbackCmd
 * Description   : Configures the LPUART loopback operation (enable/disable
 *                 loopback operation)
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 *END**************************************************************************/
void LPUART_HAL_SetLoopbackCmd(LPUART_Type * base, bool enable)
{
    /* configure LOOPS bit to enable(1)/disable(0) loopback mode, but also need
     * to clear RSRC */
    LPUART_BWR_CTRL_LOOPS(base, enable);

    /* clear RSRC for loopback mode, and if loopback disabled, */
    /* this bit has no meaning but clear anyway */
    /* to set it back to default value */
    LPUART_BWR_CTRL_RSRC(base, 0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetSingleWireCmd
 * Description   : Configures the LPUART single-wire operation (enable/disable
 *                 single-wire mode)
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 *END**************************************************************************/
void LPUART_HAL_SetSingleWireCmd(LPUART_Type * base, bool enable)
{
    /* to enable single-wire mode, need both LOOPS and RSRC set,
     * to enable or clear both */
    LPUART_BWR_CTRL_LOOPS(base, enable);
    LPUART_BWR_CTRL_RSRC(base, enable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetReceiverInStandbyMode
 * Description   : Places the LPUART receiver in standby mode.
 * In some LPUART instances, before placing LPUART in standby mode, first
 * determine whether the receiver is set to wake on idle or whether it is
 * already in idle state.
 *
 *END**************************************************************************/
lpuart_status_t LPUART_HAL_SetReceiverInStandbyMode(LPUART_Type * base)
{
    lpuart_wakeup_method_t rxWakeMethod;
    bool lpuart_current_rx_state;

    rxWakeMethod = LPUART_HAL_GetReceiverWakeupMode(base);
    lpuart_current_rx_state = LPUART_HAL_GetStatusFlag(base, kLpuartRxActive);

    /* if both rxWakeMethod is set for idle and current rx state is idle,
     * don't put in standby */
    if ((rxWakeMethod == kLpuartIdleLineWake) && (lpuart_current_rx_state == 0))
    {
        return kStatus_LPUART_RxStandbyModeError;
    }
    else
    {
        /* set the RWU bit to place receiver into standby mode */
        LPUART_BWR_CTRL_RWU(base, 1);
        return kStatus_LPUART_Success;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetIdleLineDetect
 * Description   : LPUART idle-line detect operation configuration (idle line
 * bit-count start and wake up affect on IDLE status bit).
 * In some LPUART instances, the user should disable the transmitter/receiver
 * before calling this function.
 * Generally, this may be applied to all LPUARTs to ensure safe operation.
 *
 *END**************************************************************************/
void LPUART_HAL_SetIdleLineDetect(LPUART_Type * base,
                                  const lpuart_idle_line_config_t *config)
{
    /* Configure the idle line detection configuration as follows:
     * configure the ILT to bit count after start bit or stop bit
     * configure RWUID to set or not set IDLE status bit upon detection of
     * an idle character when receiver in standby */
    LPUART_BWR_CTRL_ILT(base, config->idleLineType);
    LPUART_BWR_STAT_RWUID(base, config->rxWakeIdleDetect);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetMatchAddressReg1
 * Description   : Configures match address register 1
 *
 *END**************************************************************************/
void LPUART_HAL_SetMatchAddressReg1(LPUART_Type * base, bool enable, uint8_t value)
{
    /* The MAEN bit must be cleared before configuring MA value */
    LPUART_BWR_BAUD_MAEN1(base, 0x0U);
    if (enable)
    {
        LPUART_BWR_MATCH_MA1(base, value);
        LPUART_BWR_BAUD_MAEN1(base, 0x1U);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetMatchAddressReg2
 * Description   : Configures match address register 2
 *
 *END**************************************************************************/
void LPUART_HAL_SetMatchAddressReg2(LPUART_Type * base, bool enable, uint8_t value)
{
    /* The MAEN bit must be cleared before configuring MA value */
    LPUART_BWR_BAUD_MAEN2(base, 0x0U);
    if (enable)
    {
        LPUART_BWR_MATCH_MA2(base, value);
        LPUART_BWR_BAUD_MAEN2(base, 0x1U);
    }
}

#if FSL_FEATURE_LPUART_HAS_IR_SUPPORT
/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_SetInfrared
 * Description   : Configures the LPUART infrared operation.
 *
 *END**************************************************************************/
void LPUART_HAL_SetInfrared(LPUART_Type * base, bool enable,
                            lpuart_ir_tx_pulsewidth_t pulseWidth)
{
    /* enable or disable infrared */
    LPUART_BWR_MODIR_IREN(base, enable);

    /* configure the narrow pulse width of the IR pulse */
    LPUART_BWR_MODIR_TNP(base, pulseWidth);
}
#endif  /* FSL_FEATURE_LPUART_HAS_IR_SUPPORT */

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_GetStatusFlag
 * Description   : LPUART get status flag by passing flag enum.
 *
 *END**************************************************************************/
bool LPUART_HAL_GetStatusFlag(LPUART_Type * base, lpuart_status_flag_t statusFlag)
{
    uint32_t reg = (uint32_t)(statusFlag) >> LPUART_SHIFT;
	bool retVal = false;

    switch ( reg )
    {
        case LPUART_BAUD_REG_ID:
            retVal = LPUART_RD_BAUD(base) >> (uint32_t)(statusFlag) & 1U;
            break;
        case LPUART_STAT_REG_ID:
            retVal = LPUART_RD_STAT(base) >> (uint32_t)(statusFlag) & 1U;
            break;
        case LPUART_CTRL_REG_ID:
            retVal = LPUART_RD_CTRL(base) >> (uint32_t)(statusFlag) & 1U;
            break;
        case LPUART_DATA_REG_ID:
            retVal = LPUART_RD_DATA(base) >> (uint32_t)(statusFlag) & 1U;
            break;
        case LPUART_MATCH_REG_ID:
            retVal = LPUART_RD_MATCH(base) >> (uint32_t)(statusFlag) & 1U;
            break;
#if FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
        case LPUART_MODIR_REG_ID:
            retVal = LPUART_RD_MODIR(base) >> (uint32_t)(statusFlag) & 1U;
            break;
#endif
        default:
            break;
    }

    return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_HAL_ClearStatusFlag
 * Description   : LPUART clears an individual status flag
 * (see lpuart_status_flag_t for list of status bits).
 *
 *END**************************************************************************/
lpuart_status_t LPUART_HAL_ClearStatusFlag(LPUART_Type * base,
                                           lpuart_status_flag_t statusFlag)
{
    lpuart_status_t returnCode = kStatus_LPUART_Success;

    switch(statusFlag)
    {
        /* These flags are cleared automatically by other lpuart operations
         * and cannot be manually cleared, return error code */
        case kLpuartTxDataRegEmpty:
        case kLpuartTxComplete:
        case kLpuartRxDataRegFull:
        case kLpuartRxActive:
#if FSL_FEATURE_LPUART_HAS_EXTENDED_DATA_REGISTER_FLAGS
        case kLpuartNoiseInCurrentWord:
        case kLpuartParityErrInCurrentWord:
#endif
            returnCode = kStatus_LPUART_ClearStatusFlagError;
            break;

        case kLpuartIdleLineDetect:
            LPUART_WR_STAT(base, LPUART_STAT_IDLE_MASK);
            break;

        case kLpuartRxOverrun:
            LPUART_WR_STAT(base, LPUART_STAT_OR_MASK);
            break;

        case kLpuartNoiseDetect:
            LPUART_WR_STAT(base, LPUART_STAT_NF_MASK);
            break;

        case kLpuartFrameErr:
            LPUART_WR_STAT(base, LPUART_STAT_FE_MASK);
            break;

        case kLpuartParityErr:
            LPUART_WR_STAT(base, LPUART_STAT_PF_MASK);
            break;

        case kLpuartLineBreakDetect:
            LPUART_WR_STAT(base, LPUART_STAT_LBKDIF_MASK);
            break;

        case kLpuartRxActiveEdgeDetect:
            LPUART_WR_STAT(base, LPUART_STAT_RXEDGIF_MASK);
            break;

#if FSL_FEATURE_LPUART_HAS_ADDRESS_MATCHING
        case kLpuartMatchAddrOne:
            LPUART_WR_STAT(base, LPUART_STAT_MA1F_MASK);
            break;
        case kLpuartMatchAddrTwo:
            LPUART_WR_STAT(base, LPUART_STAT_MA2F_MASK);
            break;
#endif
        default:
            returnCode = kStatus_LPUART_ClearStatusFlagError;
            break;
    }

    return (returnCode);
}

#endif /* FSL_FEATURE_SOC_LPUART_COUNT */
/*******************************************************************************
 * EOF
 ******************************************************************************/

