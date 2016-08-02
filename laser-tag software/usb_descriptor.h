/**HEADER********************************************************************
 * 
 * Copyright (c) 2008, 2013 Freescale Semiconductor;
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
 * $FileName: usb_descriptor.h$
 * $Version : 
 * $Date    : 
 *
 * Comments:
 *
 * @brief 
 *
 *****************************************************************************/

#ifndef _USB_DESCRIPTOR_H
#define _USB_DESCRIPTOR_H 1

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "adapter_cfg.h"
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
#include "user_config.h"
#endif
#include "usb_class.h"
#include "usb_device_config.h"
#include "usb.h"
#include "usb_class_msc.h"

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define BCD_USB_VERSION                  (0x0200)

#define HIGH_SPEED                       (0)

#if HIGH_SPEED
#define CONTROLLER_ID                    USB_CONTROLLER_EHCI_0
#else
#define CONTROLLER_ID                    USB_CONTROLLER_KHCI_0
#endif
//#define BUS_POWERED                    (0x80)   
//#define SELF_POWERED                   (0x40) 
#define HS_BULK_IN_ENDP_PACKET_SIZE      (512)
#define HS_BULK_OUT_ENDP_PACKET_SIZE     (512)
#define FS_BULK_IN_ENDP_PACKET_SIZE      (64)
#define FS_BULK_OUT_ENDP_PACKET_SIZE     (64)
/* Implementation Specific Macros */
#define MSC_DESC_ENDPOINT_COUNT          (2)

#define BULK_IN_ENDPOINT                 (1)
#if HIGH_SPEED
#define BULK_IN_ENDP_PACKET_SIZE     (HS_BULK_IN_ENDP_PACKET_SIZE)/* max supported is 64 for FS and 512 for HS*/ 
#else
#define BULK_IN_ENDP_PACKET_SIZE     (FS_BULK_IN_ENDP_PACKET_SIZE)/* max supported is 64 for FS and 512 for HS*/ 
#endif
#define BULK_OUT_ENDPOINT                (2)
/*
 * In RedStripe IP DMA Transaction fails 
 * if Packet Size is configured more than 160.
 * As per USB 2.0 spec MAX packet Size has to be configured to 512 bytes.
 * Hence compliance fails.
 */
#if HIGH_SPEED
#define BULK_OUT_ENDP_PACKET_SIZE    (HS_BULK_OUT_ENDP_PACKET_SIZE)/* max supported is 64 and 512 for HS*/
#else
#define BULK_OUT_ENDP_PACKET_SIZE    (FS_BULK_OUT_ENDP_PACKET_SIZE)/* max supported is 64 and 512 for HS*/
#endif

#if (BULK_OUT_ENDP_PACKET_SIZE < 32)
#error "BULK_OUT_ENDP_PACKET_SIZE cannot be less than 32 for MSD"
#endif

#if (!HIGH_SPEED)
    #if((BULK_OUT_ENDP_PACKET_SIZE > 64) || (BULK_IN_ENDP_PACKET_SIZE > 64))
        #error "Bulk Endpoint Packet Size greater than 64 is not allowed for NON-HIGH SPEED DEVICES"
    #endif
#else
    #if((BULK_OUT_ENDP_PACKET_SIZE > 512) || (BULK_IN_ENDP_PACKET_SIZE > 512))
        #error "Bulk Endpoint Packet Size greater than 512 is not allowed for HIGH SPEED DEVICES"
    #endif
#endif

#if HIGH_SPEED 
    /* Here Other speed Configuration is for FULL SPEED */  
    #define OTHER_SPEED_BULK_IN_ENDP_PACKET_SIZE     (64)/* max supported is 64 for FS and 512 for HS*/ 
    #define OTHER_SPEED_BULK_OUT_ENDP_PACKET_SIZE    (64)/* max supported is 64 and 512 for HS*/ 
#endif

/* Various descriptor sizes */
#define DEVICE_DESCRIPTOR_SIZE            (18)
#define CONFIG_ONLY_DESC_SIZE             (9)
#define IFACE_ONLY_DESC_SIZE              (9)
#define ENDP_ONLY_DESC_SIZE               (7)

#define CONFIG_DESC_SIZE       (CONFIG_ONLY_DESC_SIZE + \
                                IFACE_ONLY_DESC_SIZE + \
                                ENDP_ONLY_DESC_SIZE * MSC_DESC_ENDPOINT_COUNT)

#if HIGH_SPEED
    #define DEVICE_QUALIFIER_DESCRIPTOR_SIZE    (10)
    #define OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE  (CONFIG_DESC_SIZE)  
#endif

/* Max descriptors provided by the Application */
#define USB_MAX_STD_DESCRIPTORS           (7) 

/* Max configuration supported by the Application */
#define USB_MAX_CONFIG_SUPPORTED          (1)

/* Max string descriptors supported by the Application */
#define USB_MAX_STRING_DESCRIPTORS        (4)

/* Max language codes supported by the USB */
#define USB_MAX_LANGUAGES_SUPPORTED       (1)

/* string descriptors sizes */
#define USB_STR_DESC_SIZE                 (2)
#define USB_STR_0_SIZE                    (2)
#define USB_STR_1_SIZE                    (12)
#define USB_STR_2_SIZE                    (20)
#define USB_STR_3_SIZE                    (32)
#define USB_STR_n_SIZE                    (32)

/* descriptors codes */
#define USB_DEVICE_DESCRIPTOR             (1)
#define USB_CONFIG_DESCRIPTOR             (2)
#define USB_STRING_DESCRIPTOR             (3)
#define USB_IFACE_DESCRIPTOR              (4)
#define USB_ENDPOINT_DESCRIPTOR           (5)
#define USB_DEVQUAL_DESCRIPTOR            (6)

#define USB_MAX_SUPPORTED_INTERFACES      (1)

#if HIGH_SPEED
    #define USB_DEVQUAL_DESCRIPTOR        (6)
    #define USB_OTHER_SPEED_DESCRIPTOR    (7)
#endif

#define DEVICE_DESC_DEVICE_CLASS                (0x00)
#define DEVICE_DESC_DEVICE_SUBCLASS             (0x00)
#define DEVICE_DESC_DEVICE_PROTOCOL             (0x00)
#define DEVICE_DESC_NUM_CONFIG_SUPPOTED         (0x01)
/* Keep the following macro Zero if you don't Support Other Speed Configuration
 If you support Other Speeds make it 0x01 */
#define DEVICE_OTHER_DESC_NUM_CONFIG_SUPPOTED   (0x00) 
#define CONFIG_DESC_NUM_INTERFACES_SUPPOTED     (0x01)
#define CONFIG_DESC_CURRENT_DRAWN               (0x32)

/* mass storage specific macros */
#define MASS_STORAGE_CLASS               (0x08)
#define SCSI_TRANSPARENT_COMMAND_SET     (0x06)
#define BULK_ONLY_PROTOCOL               (0x50)
#define PERIPHERAL_DEVICE_TYPE           (0x00)
/* Direct Access Block Device : Magnetic and Flash Drives*/
#define PERIPHERAL_QUALIFIER_SHIFT       (5)
#define PERIPHERAL_QUALIFIER             (0)
#define SPC_VERSION                      (4)/*SPC3 is 5; SPC2 is 4*/
#define REMOVABLE_MEDIUM_BIT             (1) 
#define REMOVABLE_MEDIUM_BIT_SHIFT       (7)
#define ADDITIONAL_LENGTH                (0x20)
/*set to 0x20H if returning 36 bytes of inquiry data*/
/******************************************************************************
 * Types
 *****************************************************************************/
/******************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint8_t USB_Desc_Get_Descriptor(
    uint32_t handle,
    uint8_t type,
    uint8_t str_num,
    uint16_t index,
    uint8_t * *descriptor,
    uint32_t *size);

extern uint8_t USB_Desc_Get_Interface(
    uint32_t handle,
    uint8_t interface,
    uint8_t * alt_interface);

extern uint8_t USB_Desc_Set_Interface(
    uint32_t handle,
    uint8_t interface,
    uint8_t alt_interface);

extern bool USB_Desc_Valid_Configation(uint32_t handle,
    uint16_t config_val);

extern bool USB_Desc_Remote_Wakeup(uint32_t handle);

extern usb_endpoints_t *USB_Desc_Get_Endpoints(uint32_t handle);

uint8_t USB_Desc_Set_Speed
(
    uint32_t handle,
    uint16_t speed
    );
#endif

/* vim: set expandtab ts=4 sw=4: */
