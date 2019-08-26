/**HEADER********************************************************************
* 
* Copyright (c) 2013 - 2015 Freescale Semiconductor;
* All Rights Reserved
*
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* Comments:  
*
*END************************************************************************/
#include "adapter.h"
#include "usb.h"
#include "usb_device_config.h"
#include "usb_misc.h"
#include "fsl_usb_khci_hal.h"
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
    #if (defined(CPU_MKL27Z64VLH4))
#include "MKL27Z644/MKL27Z644_sim.h"
#include "MKL27Z644/MKL27Z644_usb.h"
    #endif
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
#include "MKL27Z644.h"
#endif
#include "usb_device_config.h"
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX) || (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM) || (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#define BSP_USB_INT_LEVEL                (4)
#define USB_CLK_RECOVER_IRC_EN (*(volatile unsigned char *)0x40072144)
#define BSPCFG_USB_USE_IRC48M            (1)

extern uint8_t soc_get_usb_vector_number(uint8_t controller_id);
extern uint32_t soc_get_usb_base_address(uint8_t controller_id);
#ifdef __cplusplus
extern "C" {
#endif
extern _WEAK_FUNCTION(usb_status bsp_usb_dev_board_init(uint8_t controller_id));
#ifdef __cplusplus
    }
#endif

static usb_status bsp_usb_dev_soc_init
(
    uint8_t controller_id
)
{
    int32_t ret = USB_OK;
    uint8_t usb_instance = 0;
    uint32_t base_addres = 0;
    if (USB_CONTROLLER_KHCI_0==controller_id  )
    {    
         usb_instance = controller_id - USB_CONTROLLER_KHCI_0;
         base_addres = soc_get_usb_base_address(controller_id);
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#if BSPCFG_USB_USE_IRC48M
        /* IRC48MHz selected as CLK source */
        CLOCK_SYS_SetUsbfsSrc(usb_instance, kClockUsbfsSrcIrc48M);
        /* USB Clock Gating */
        CLOCK_SYS_EnableUsbfsClock(usb_instance);
        /* Enable IRC 48MHz for USB module, the regulator is always enabled on KL27 */
        USB_CLK_RECOVER_IRC_EN = USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK;
#else
        /* USB_CLKIN selected as CLK source */
        CLOCK_SYS_SetUsbfsSrc(usb_instance, kClockUsbfsSrcExt);
        
        /* USB Clock Gating */
        CLOCK_SYS_EnableUsbfsClock(usb_instance);
#endif
        
        /* reset USB CTRL register */
        usb_hal_khci_reset_control_register(base_addres);
#if USBCFG_DEV_KEEP_ALIVE_MODE
        USB0_CLK_RECOVER_CTRL |= USB_CLK_RECOVER_CTRL_CLOCK_RECOVER_EN_MASK;
        USB0_KEEP_ALIVE_CTRL = USB_KEEP_ALIVE_CTRL_KEEP_ALIVE_EN_MASK|USB_KEEP_ALIVE_CTRL_OWN_OVERRD_EN_MASK
                                    |USB_KEEP_ALIVE_CTRL_STOP_ACK_DLY_EN_MASK|USB_KEEP_ALIVE_CTRL_AHB_DLY_EN_MASK
                                    |USB_KEEP_ALIVE_CTRL_WAKE_INT_EN_MASK;
        /* wake on out and setup transaction */
        USB0_KEEP_ALIVE_WKCTRL = 0x1;
        MCG_MC |= MCG_MC_HIRCLPEN_MASK;
        PMC_REGSC |= PMC_REGSC_BGEN_MASK|PMC_REGSC_VLPO_MASK;
#endif

        /* Enable internal pull-up resistor */
        usb_hal_khci_set_internal_pullup(base_addres);
        usb_hal_khci_set_trc0(base_addres); /* Software must set this bit to 1 */

        /* setup interrupt */
        OS_intr_init((IRQn_Type)soc_get_usb_vector_number(controller_id), BSP_USB_INT_LEVEL, 0, TRUE);

#endif
    }
    else
    {
        ret = USBERR_BAD_STATUS; //unknow controller
    }

    return ret;
}

_WEAK_FUNCTION (usb_status bsp_usb_dev_board_init(uint8_t controller_id))
{
    usb_status ret = USB_OK;

    controller_id = bsp_usb_dev_soc_init(controller_id);
    if ( USB_CONTROLLER_KHCI_0==controller_id)
    {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)

#else

#endif
    }
    else
    {
       ret = USBERR_BAD_STATUS;
    }

    return ret;
}
usb_status bsp_usb_dev_init(uint8_t controller_id)
{
    usb_status ret = USB_OK;
    
    ret = bsp_usb_dev_soc_init(controller_id);
    if (ret == USB_OK)
    {
        ret = bsp_usb_dev_board_init(controller_id);
    }

    return ret;
}

#endif
/* EOF */
