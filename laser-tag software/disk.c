/**HEADER********************************************************************
 * 
 * Copyright (c) 2008, 2013 - 2014 Freescale Semiconductor;
 * All Rights Reserved
 *
 * Copyright (c) 1989-2008 ARC International;
 * All Rights Reserved
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
 * $FileName: disk.c$
 * $Version : 
 * $Date    : 
 *
 * Comments:
 *
 * @brief  RAM Disk has been emulated via this Mass Storage Demo
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device_stack_interface.h"
#include "disk.h"
#include "adapter_cfg.h"
#include "flash.h"
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
#include "fsl_debug_console.h"
#include "fsl_port_hal.h"

#if USBCFG_DEV_COMPOSITE
#error This application requires USBCFG_DEV_COMPOSITE defined zero in usb_device_config.h. Please recompile usbd with this option.
#endif

#if !USBCFG_DEV_MSC
#error This application requires USBCFG_DEV_MSC defined none zero in usb_device_config.h. Please recompile usbd with this option.
#endif

#if HIGH_SPEED

#if !USBCFG_DEV_EHCI
#error This application requires USBCFG_DEV_EHCI defined none zero in usb_device_config.h. Please recompile usbd with this option.
#endif

#endif

/****************************************************************************
 * Global Variables
 ****************************************************************************/
/* Add all the variables needed for disk.c to this structure */
extern usb_desc_request_notify_struct_t desc_callback;
usb_application_callback_struct_t msc_application_callback;
usb_vendor_req_callback_struct_t vend_req_callback;
usb_class_specific_callback_struct_t class_specific_callback;

msc_config_struct_t g_msd_config;
disk_struct_t g_disk;
uint8_t sector_buffer[MSD_RECV_BUFFER_SIZE];

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
void USB_App_Device_Callback(uint8_t event_type, void* val, void* arg);
uint8_t USB_App_Class_Callback
(uint8_t event_type,
    uint16_t value,
    uint8_t ** data,
    uint32_t* size,
    void* arg
    );

/*****************************************************************************
 * Local Variables 
 *****************************************************************************/

/*****************************************************************************
 * Local Functions
 *****************************************************************************/

/******************************************************************************
 * 
 *    @name        USB_App_Device_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       pointer : 
 *    @param       event_type : value of the event
 *    @param       val : gives the configuration value 
 * 
 *    @return      None
 *
 *****************************************************************************/
void USB_App_Device_Callback(uint8_t event_type, void* val, void* arg)
{
    if (event_type == USB_DEV_EVENT_BUS_RESET)
    {
        g_disk.start_app = FALSE;
        if (USB_OK == USB_Class_MSC_Get_Speed(g_disk.app_handle, &g_disk.speed))
        {
            USB_Desc_Set_Speed(g_disk.app_handle, g_disk.speed);
        }
    }
    else if (event_type == USB_DEV_EVENT_ENUM_COMPLETE)
    {
        g_disk.start_app = TRUE;
    }
    else if (event_type == USB_DEV_EVENT_ERROR)
    {
        /* add user code for error handling */
    }
    else if (event_type == USB_MSC_DEVICE_GET_SEND_BUFF_INFO)
    {
        if (NULL != val)
        {
            *((uint32_t *)val) = (uint32_t)DISK_SIZE_NORMAL;
        }
    }
    else if (event_type == USB_MSC_DEVICE_GET_RECV_BUFF_INFO)
    {
        if (NULL != val)
        {
            *((uint32_t *)val) = (uint32_t)DISK_SIZE_NORMAL;
        }
    }

    return;
}

/******************************************************************************
 * 
 *    @name        USB_App_Class_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       pointer : 
 *    @param       event_type : value of the event
 *    @param       val : gives the configuration value 
 * 
 *    @return      None
 *
 *****************************************************************************/
uint8_t USB_App_Class_Callback
(uint8_t event_type,
    uint16_t value,
    uint8_t ** data,
    uint32_t* size,
    void* arg
) 
{
    lba_app_struct_t* lba_data_ptr;
    uint8_t * prevent_removal_ptr;
    device_lba_info_struct_t* device_lba_info_ptr;
    uint8_t error = USB_OK;

    if (g_disk.read_write_error)
    {
        return USBERR_ERROR;
    }

    switch(event_type)
    {
    case USB_DEV_EVENT_DATA_RECEIVED:
        lba_data_ptr = (lba_app_struct_t*) size;
        flash_write((intptr_t)images[lba_data_ptr->lun] + lba_data_ptr->offset,
                sector_buffer,
                MSD_RECV_BUFFER_SIZE);
        break;
    case USB_DEV_EVENT_SEND_COMPLETE:
        lba_data_ptr = (lba_app_struct_t*) size;
        break;
    case USB_MSC_START_STOP_EJECT_MEDIA:
        break;
    case USB_MSC_DEVICE_READ_REQUEST:
        lba_data_ptr = (lba_app_struct_t*) size;

        if(data != NULL)
        {
            *data = images[lba_data_ptr->lun] + lba_data_ptr->offset;
        }
        break;
    case USB_MSC_DEVICE_WRITE_REQUEST:
        lba_data_ptr = (lba_app_struct_t*) size;
        if(data != NULL)
        {
            *data = sector_buffer;
        }
        break;
    case USB_MSC_DEVICE_FORMAT_COMPLETE:
        break;
    case USB_MSC_DEVICE_REMOVAL_REQUEST:
        prevent_removal_ptr = (uint8_t *) size;
        if (SUPPORT_DISK_LOCKING_MECHANISM)
        {
            g_disk.disk_lock = *prevent_removal_ptr;
        }
        else if ((!SUPPORT_DISK_LOCKING_MECHANISM) && (!(*prevent_removal_ptr)))
        {
            /*there is no support for disk locking and removal of medium is enabled*/
            /* code to be added here for this condition, if required */
        }
        break;
    case USB_MSC_DEVICE_GET_INFO:
        device_lba_info_ptr = (device_lba_info_struct_t*) size;
        device_lba_info_ptr->total_lba_device_supports = TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL;
        device_lba_info_ptr->length_of_each_lab_of_device = LENGTH_OF_EACH_LAB;
        device_lba_info_ptr->num_lun_supported = LOGICAL_UNIT_SUPPORTED;
        break;
    default:
        break;
    }

    return error;
}

/******************************************************************************
 *  
 *   @name        disk_init
 * 
 *   @brief       This function is the entry for mouse (or other usage)
 * 
 *   @param       None
 * 
 *   @return      None
 **                
 *****************************************************************************/

void disk_init()
{
    OS_Mem_zero(&g_disk, sizeof(disk_struct_t));

    OS_Mem_zero(&g_msd_config, sizeof(msc_config_struct_t));

    msc_application_callback.callback = USB_App_Device_Callback;
    msc_application_callback.arg = &g_disk.app_handle;

    /* Register the callbacks to lower layers */
    g_msd_config.msc_application_callback = msc_application_callback;
    g_msd_config.vendor_req_callback = vend_req_callback;
    g_msd_config.class_specific_callback.callback = USB_App_Class_Callback;
    g_msd_config.class_specific_callback.arg = &g_disk.app_handle;
    g_msd_config.desc_callback_ptr = &desc_callback;

    g_disk.speed = USB_SPEED_FULL;
    /* Finally, Initialize the device and USB Stack layers*/
    USB_Class_MSC_Init(CONTROLLER_ID, &g_msd_config, &g_disk.app_handle);
}

/* vim: set expandtab ts=4 sw=4: */
