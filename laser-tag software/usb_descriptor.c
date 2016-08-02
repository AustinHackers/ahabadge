/**HEADER********************************************************************
 * 
 * Copyright (c) 2008, 2013 - 2015 Freescale Semiconductor;
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
 * $FileName: usb_descriptor.c$
 * $Version : 
 * $Date    : 
 *
 * Comments:
 *
 * @brief The file contains USB descriptors and functions
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_device_config.h"
#include "usb.h"
#include "usb_device_stack_interface.h"
#include "usb_descriptor.h"

extern void USB_App_Class_Callback
(uint8_t event_type,
    uint16_t value,
    uint8_t ** data,
    uint32_t* size,
    void* arg
) ;
/*****************************************************************************
 * Constant and Macro's
 *****************************************************************************/
usb_ep_struct_t ep[MSC_DESC_ENDPOINT_COUNT] =
{
    {
        BULK_IN_ENDPOINT,
        USB_BULK_PIPE,
        USB_SEND,
        BULK_IN_ENDP_PACKET_SIZE
    },
    {
        BULK_OUT_ENDPOINT,
        USB_BULK_PIPE,
        USB_RECV,
        BULK_OUT_ENDP_PACKET_SIZE
    }
};

/* structure containing details of all the endpoints used by this device */
usb_endpoints_t usb_desc_ep =
{
    MSC_DESC_ENDPOINT_COUNT,
    ep
};

static usb_if_struct_t usb_if[1];
static device_lba_info_struct_t usb_msc_lba_info_struct;
static const usb_class_struct_t usb_dec_class =
{
    USB_CLASS_MSC,
    {
        1, /* Count */
        usb_if
    }
};

uint8_t g_device_descriptor[DEVICE_DESCRIPTOR_SIZE] =
{
    /* "Device Descriptor Size */
    DEVICE_DESCRIPTOR_SIZE,
    /* "Device" Type of descriptor */
    USB_DEVICE_DESCRIPTOR,
    /*  BCD USB version  */
    USB_uint_16_low(BCD_USB_VERSION), USB_uint_16_high(BCD_USB_VERSION),
    /* Device Class is indicated in the interface descriptors */
    DEVICE_DESC_DEVICE_CLASS,
    /*  Device Subclass is indicated in the interface descriptors  */
    DEVICE_DESC_DEVICE_SUBCLASS,
    /*  Device Protocol  */
    DEVICE_DESC_DEVICE_PROTOCOL,
    /* Max Packet size */
    CONTROL_MAX_PACKET_SIZE,
    /* Vendor ID */
    0xa2, 0x15,
    /* Product ID */
    0x00, 0x02,
    /* BCD Device version */
    0x02, 0x00,
    /* Manufacturer string index */
    0x01,
    /* Product string index */
    0x02,
    /*  Serial number string index */
    0x03,
    /*  Number of configurations */
    DEVICE_DESC_NUM_CONFIG_SUPPOTED
};

uint8_t g_config_descriptor[CONFIG_DESC_SIZE] =
{
    /* Configuration Descriptor Size - always 9 bytes*/
    CONFIG_ONLY_DESC_SIZE,
    /* "Configuration" type of descriptor */
    USB_CONFIG_DESCRIPTOR,
    /*  Total length of the Configuration descriptor */
    USB_uint_16_low(CONFIG_DESC_SIZE), USB_uint_16_high(CONFIG_DESC_SIZE),
    /*  NumInterfaces */
    CONFIG_DESC_NUM_INTERFACES_SUPPOTED,
    /*  Configuration Value */
    1,
    /* Configuration Description String Index */
    0,
    /*  Attributes.support RemoteWakeup and self power */
    (USB_DESC_CFG_ATTRIBUTES_D7_POS) | (USBCFG_DEV_SELF_POWER << USB_DESC_CFG_ATTRIBUTES_SELF_POWERED_SHIFT) | (USBCFG_DEV_REMOTE_WAKEUP << USB_DESC_CFG_ATTRIBUTES_REMOTE_WAKEUP_SHIFT),
    /*  Current draw from bus */
    CONFIG_DESC_CURRENT_DRAWN,

    /* INTERFACE DESCRIPTOR */
    IFACE_ONLY_DESC_SIZE,
    USB_IFACE_DESCRIPTOR,
    /* bInterfaceNumber */
    0x00,
    /* bAlternateSetting */
    0x00,
    (uint8_t) MSC_DESC_ENDPOINT_COUNT,
    /* Interface Class */
    MASS_STORAGE_CLASS,
    /* Interface Subclass*/
    SCSI_TRANSPARENT_COMMAND_SET,
    /* Interface Protocol*/
    BULK_ONLY_PROTOCOL,
    /* Interface Description String Index*/
    0x00,

    /*Endpoint descriptor */
    ENDP_ONLY_DESC_SIZE,
    USB_ENDPOINT_DESCRIPTOR,
    BULK_IN_ENDPOINT | (USB_SEND << 7),
    USB_BULK_PIPE,
    USB_uint_16_low(BULK_IN_ENDP_PACKET_SIZE),
    USB_uint_16_high(BULK_IN_ENDP_PACKET_SIZE),
    0x00,/* This value is ignored for Bulk ENDPOINT */

    /*Endpoint descriptor */
    ENDP_ONLY_DESC_SIZE,
    USB_ENDPOINT_DESCRIPTOR,
    BULK_OUT_ENDPOINT | (USB_RECV << 7),
    USB_BULK_PIPE,
    USB_uint_16_low(BULK_OUT_ENDP_PACKET_SIZE),
    USB_uint_16_high(BULK_OUT_ENDP_PACKET_SIZE),
    0x00 /* This value is ignored for Bulk ENDPOINT */
};

#if HIGH_SPEED
uint8_t g_device_qualifier_descriptor[DEVICE_QUALIFIER_DESCRIPTOR_SIZE] =
{
    /* Device Qualifier Descriptor Size */
    DEVICE_QUALIFIER_DESCRIPTOR_SIZE,
    /* Type of Descriptor */
    USB_DEVQUAL_DESCRIPTOR,
    /*  BCD USB version  */
    USB_uint_16_low(BCD_USB_VERSION), USB_uint_16_high(BCD_USB_VERSION),
    /* bDeviceClass */
    DEVICE_DESC_DEVICE_CLASS,
    /* bDeviceSubClass */
    DEVICE_DESC_DEVICE_SUBCLASS,
    /* bDeviceProtocol */
    DEVICE_DESC_DEVICE_PROTOCOL,
    /* bMaxPacketSize0 */
    CONTROL_MAX_PACKET_SIZE,
    /* bNumConfigurations */
    DEVICE_OTHER_DESC_NUM_CONFIG_SUPPOTED,
    /* Reserved : must be zero */
    0x00
};

uint8_t g_other_speed_config_descriptor[OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE] =
{
    CONFIG_ONLY_DESC_SIZE,
    /* This is a Other speed config descr */
    USB_OTHER_SPEED_DESCRIPTOR,
    /*  Total length of the Configuration descriptor */
    USB_uint_16_low(CONFIG_DESC_SIZE), USB_uint_16_high(CONFIG_DESC_SIZE),
    CONFIG_DESC_NUM_INTERFACES_SUPPOTED,
    /*value used to select this configuration : Configuration Value */
    1,
    /*  Configuration Description String Index*/
    0,
    /*Attributes.support RemoteWakeup and self power*/
    (USB_DESC_CFG_ATTRIBUTES_D7_POS) | (USBCFG_DEV_SELF_POWER << USB_DESC_CFG_ATTRIBUTES_SELF_POWERED_SHIFT) | (USBCFG_DEV_REMOTE_WAKEUP << USB_DESC_CFG_ATTRIBUTES_REMOTE_WAKEUP_SHIFT),
    /*  Current draw from bus */
    CONFIG_DESC_CURRENT_DRAWN,

    /* INTERFACE DESCRIPTOR */
    IFACE_ONLY_DESC_SIZE,
    USB_IFACE_DESCRIPTOR,
    /* bInterfaceNumber */
    0x00,
    /* bAlternateSetting */
    0x00,
    (uint8_t)MSC_DESC_ENDPOINT_COUNT,
    /* Interface Class */
    MASS_STORAGE_CLASS,
    /* Interface Subclass*/
    SCSI_TRANSPARENT_COMMAND_SET,
    /* Interface Protocol*/
    BULK_ONLY_PROTOCOL,
    /* Interface Description String Index*/
    0x00,

    /*Endpoint descriptor */
    ENDP_ONLY_DESC_SIZE,
    USB_ENDPOINT_DESCRIPTOR,
    BULK_IN_ENDPOINT|(USB_SEND << 7),
    USB_BULK_PIPE,
    USB_uint_16_low(OTHER_SPEED_BULK_IN_ENDP_PACKET_SIZE),
    USB_uint_16_high(OTHER_SPEED_BULK_IN_ENDP_PACKET_SIZE),
    0x00,/* This value is ignored for Bulk ENDPOINT */

    /*Endpoint descriptor */
    ENDP_ONLY_DESC_SIZE,
    USB_ENDPOINT_DESCRIPTOR,
    BULK_OUT_ENDPOINT|(USB_RECV << 7),
    USB_BULK_PIPE,
    USB_uint_16_low(OTHER_SPEED_BULK_OUT_ENDP_PACKET_SIZE),
    USB_uint_16_high(OTHER_SPEED_BULK_OUT_ENDP_PACKET_SIZE),
    0x00 /* This value is ignored for Bulk ENDPOINT */
};
#endif

uint8_t USB_STR_0[USB_STR_0_SIZE + USB_STR_DESC_SIZE] =
{ sizeof(USB_STR_0),
    USB_STRING_DESCRIPTOR,
    0x09,
    0x04 /*equivalent to 0x0409*/
};

/*  Manufacturer string */
uint8_t USB_STR_1[USB_STR_1_SIZE + USB_STR_DESC_SIZE] =
{ sizeof(USB_STR_1),
    USB_STRING_DESCRIPTOR,
    'F', 0,
    'R', 0,
    'E', 0,
    'E', 0,
    'S', 0,
    'C', 0,
    'A', 0,
    'L', 0,
    'E', 0,
    ' ', 0,
    'S', 0,
    'E', 0,
    'M', 0,
    'I', 0,
    'C', 0,
    'O', 0,
    'N', 0,
    'D', 0,
    'U', 0,
    'C', 0,
    'T', 0,
    'O', 0,
    'R', 0,
    ' ', 0,
    'I', 0,
    'N', 0,
    'C', 0,
    '.', 0
};

/*  Product string */
uint8_t USB_STR_2[USB_STR_2_SIZE + USB_STR_DESC_SIZE] =
{ sizeof(USB_STR_2),
    USB_STRING_DESCRIPTOR,
    'M', 0,
    'Q', 0,
    'X', 0,
    ' ', 0,
    'M', 0,
    'A', 0,
    'S', 0,
    'S', 0,
    ' ', 0,
    'S', 0,
    'T', 0,
    'O', 0,
    'R', 0,
    'A', 0,
    'G', 0,
    'E', 0
};

/*  Serial number string */
uint8_t USB_STR_3[USB_STR_3_SIZE + USB_STR_DESC_SIZE] =
{ sizeof(USB_STR_3),
    USB_STRING_DESCRIPTOR,
    '0', 0,
    '1', 0,
    '2', 0,
    '3', 0,
    '4', 0,
    '5', 0,
    '6', 0,
    '7', 0,
    '8', 0,
    '9', 0,
    'A', 0,
    'B', 0,
    'C', 0,
    'D', 0,
    'E', 0,
    'F', 0
};

uint8_t USB_STR_n[USB_STR_n_SIZE + USB_STR_DESC_SIZE] =
{ sizeof(USB_STR_n),
    USB_STRING_DESCRIPTOR,
    'B', 0,
    'A', 0,
    'D', 0,
    ' ', 0,
    'S', 0,
    'T', 0,
    'R', 0,
    'I', 0,
    'N', 0,
    'G', 0,
    ' ', 0,
    'I', 0,
    'N', 0,
    'D', 0,
    'E', 0,
    'X', 0
};

uint32_t g_std_desc_size[USB_MAX_STD_DESCRIPTORS + 1] =
{ 0,
    DEVICE_DESCRIPTOR_SIZE,
    CONFIG_DESC_SIZE,
    0, /* string */
    0, /* Interface */
    0, /* Endpoint */
#if HIGH_SPEED
    DEVICE_QUALIFIER_DESCRIPTOR_SIZE,
    OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE
#else                                         
    0, /* Device Qualifier */
    0 /* other speed config */
#endif
};

uint8_t *g_std_descriptors[USB_MAX_STD_DESCRIPTORS + 1] =
{
    NULL,
    g_device_descriptor,
    g_config_descriptor,
    NULL, /* string */
    NULL, /* Interface */
    NULL, /* Endpoint */
#if HIGH_SPEED
    g_device_qualifier_descriptor,
    g_other_speed_config_descriptor
#else
    NULL, /* Device Qualifier */
    NULL /* other speed config*/
#endif
};

uint8_t g_string_desc_size[USB_MAX_STRING_DESCRIPTORS + 1] =
{ sizeof(USB_STR_0),
    sizeof(USB_STR_1),
    sizeof(USB_STR_2),
    sizeof(USB_STR_3),
    sizeof(USB_STR_n)
};

uint8_t *g_string_descriptors[USB_MAX_STRING_DESCRIPTORS + 1] =
{ USB_STR_0,
    USB_STR_1,
    USB_STR_2,
    USB_STR_3,
    USB_STR_n
};

usb_language_t usb_language[USB_MAX_SUPPORTED_INTERFACES] =
{ { (uint16_t) 0x0409, g_string_descriptors, g_string_desc_size } };

usb_all_languages_t g_languages =
{ USB_STR_0, sizeof(USB_STR_0), USB_MAX_LANGUAGES_SUPPORTED,
    usb_language
};

uint8_t g_valid_config_values[USB_MAX_CONFIG_SUPPORTED + 1] = { 0, 1 };

/****************************************************************************
 * Global Variables
 ****************************************************************************/
static uint8_t g_alternate_interface[USB_MAX_SUPPORTED_INTERFACES];

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes 
 *****************************************************************************/

/*****************************************************************************
 * Local Variables
 *****************************************************************************/
#if 0
DESC_CALLBACK_FUNCTIONS_STRUCT desc_callback =
{
    0xFF,
    USB_Desc_Get_Descriptor,
    USB_Desc_Get_Endpoints,
    USB_Desc_Get_Interface,
    USB_Desc_Set_Interface,
    USB_Desc_Valid_Configation,
    USB_Desc_Remote_Wakeup,
    NULL,
    NULL
};
#endif

/*****************************************************************************
 * Local Functions - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Descriptor
 *
 * @brief The function returns the corresponding descriptor
 *
 * @param handle:        handle     
 * @param type:          type of descriptor requested     
 * @param str_num:       string index for string descriptor     
 * @param index:         string descriptor language Id     
 * @param descriptor:    output descriptor pointer
 * @param size:          size of descriptor returned
 *
 * @return USB_OK                              When Success
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint8_t USB_Desc_Get_Descriptor
(
    uint32_t handle,  // this is not of type usb_device_handle because of some bug in CW 7.1 RegABI
    uint8_t type,
    uint8_t str_num,
    uint16_t index,
    uint8_t * *descriptor,
    uint32_t *size
)
{
    UNUSED_ARGUMENT (handle)

    /* string descriptors are handled separately */
    if (type == USB_STRING_DESCRIPTOR)
    {
        if (index == 0)
        {
            /* return the string and size of all languages */
            *descriptor = (uint8_t *) g_languages.languages_supported_string;
            *size = g_languages.languages_supported_size;
        }
        else
        {
            uint8_t lang_id = 0;
            uint8_t lang_index = USB_MAX_LANGUAGES_SUPPORTED;

            for (; lang_id < USB_MAX_LANGUAGES_SUPPORTED; lang_id++)
            {
                /* check whether we have a string for this language */
                if (index == g_languages.usb_language[lang_id].language_id)
                { /* check for max descriptors */
                    if (str_num < USB_MAX_STRING_DESCRIPTORS)
                    { /* setup index for the string to be returned */
                        lang_index = str_num;
                    }
                    break;
                }
            }

            /* set return val for descriptor and size */
            *descriptor = (uint8_t *)
            g_languages.usb_language[lang_id].lang_desc[lang_index];
            *size =
            g_languages.usb_language[lang_id].lang_desc_size[lang_index];
        }
    }
    else if (type < USB_MAX_STD_DESCRIPTORS + 1)
    {
        /* set return val for descriptor and size*/
        *descriptor = (uint8_t *) g_std_descriptors[type];

        /* if there is no descriptor then return error */
        if (*descriptor == NULL)
        {
            return USBERR_INVALID_REQ_TYPE;
        }
        *size = g_std_desc_size[type];
    }
    else /* invalid descriptor */
    {
        return USBERR_INVALID_REQ_TYPE;
    }
    return USB_OK;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Interface
 *
 * @brief The function returns the alternate interface
 *
 * @param handle:         handle     
 * @param interface:      interface number     
 * @param alt_interface:  output alternate interface     
 *
 * @return USB_OK                              When Success
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint8_t USB_Desc_Get_Interface
(
    uint32_t handle,
    uint8_t interface,
    uint8_t * alt_interface
)
{
    UNUSED_ARGUMENT (handle)
    /* if interface valid */
    if (interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* get alternate interface*/
        *alt_interface = g_alternate_interface[interface];
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Interface
 *
 * @brief The function sets the alternate interface
 *
 * @param handle:         handle     
 * @param interface:      interface number     
 * @param alt_interface:  input alternate interface     
 *
 * @return USB_OK                              When Success
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint8_t USB_Desc_Set_Interface
(
    uint32_t handle,
    uint8_t interface,
    uint8_t alt_interface
)
{
    UNUSED_ARGUMENT (handle)
    /* if interface valid */
    if (interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* set alternate interface*/
        g_alternate_interface[interface] = alt_interface;
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Valid_Configation
 *
 * @brief The function checks whether the configuration parameter 
 *        input is valid or not
 *
 * @param handle          handle    
 * @param config_val      configuration value     
 *
 * @return TRUE           When Valid
 *         FALSE          When Error
 *****************************************************************************/
bool USB_Desc_Valid_Configation(uint32_t handle, uint16_t config_val)
{
    uint8_t loop_index = 0;
    UNUSED_ARGUMENT (handle)
    /* check with only supported val right now */
    while (loop_index < (USB_MAX_CONFIG_SUPPORTED + 1))
    {
        if (config_val == g_valid_config_values[loop_index])
        {
            return TRUE;
        }
        loop_index++;
    }
    return FALSE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Remote_Wakeup
 *
 * @brief The function checks whether the remote wakeup is supported or not
 *
 * @param handle:        handle     
 *
 * @return REMOTE_WAKEUP_SUPPORT (TRUE) - if remote wakeup supported
 *****************************************************************************/
bool USB_Desc_Remote_Wakeup(uint32_t handle)
{
    UNUSED_ARGUMENT (handle)
    return USBCFG_DEV_REMOTE_WAKEUP;
}

#if 0
/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Endpoints
 *
 * @brief The function returns with the list of all non control endpoints used
 *
 * @param handle:          handle     
 *
 * @return pointer to usb_endpoints_t
 *****************************************************************************/
usb_endpoints_t *USB_Desc_Get_Endpoints(uint32_t handle)
{
    UNUSED_ARGUMENT (handle)
    return &usb_desc_ep;
}
#endif

/**************************************************************************//*!
 *
 * @name  USB_Set_Configuration
 *
 * @brief The function set the configuration value of device 
 *        
 *
 * @param handle          handle     
 * @param config_val      configuration value     
 *
 * @return TRUE           When Valid
 *         FALSE          When Error
 *****************************************************************************/
uint8_t USB_Set_Configuration
(
    msd_handle_t handle, uint8_t config

)
{
    UNUSED_ARGUMENT (handle)
    /* TODO */
    /* if a device has two or more configuration, should set the configuration*/ 
    return USB_OK;
}

//usb_if_struct_t usb_qq[5] = {1,usb_desc_ep};

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Entity
 *
 * @brief The function retrieves the entity specified by type.
 *
 * @param handle            handle     
 *
 * @return USB_OK  - if success
 *****************************************************************************/
uint8_t USB_Desc_Get_Entity(msd_handle_t handle, entity_type type, uint32_t * object)
{

    switch(type)
    {
    case USB_CLASS_INFO:
        usb_if[0].index = 1;
        usb_if[0].endpoints = usb_desc_ep;
        *object = (unsigned long) &usb_dec_class;
        break;

    case USB_MSC_LBA_INFO:

        USB_App_Class_Callback(USB_MSC_DEVICE_GET_INFO, USB_REQ_VAL_INVALID, NULL, (uint32_t *) &usb_msc_lba_info_struct, NULL);
        *object = (unsigned long) &usb_msc_lba_info_struct;
        break;

    default:

        break;
    }/* End Switch */
    return USB_OK;

}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Speed
 *
 * @brief The function is used to set device speed
 *
 * @param handle:         handle     
 * @param speed:          speed    
 *
 * @return USB_OK                              When Success
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint8_t USB_Desc_Set_Speed
(
    uint32_t handle,
    uint16_t speed
    )
{
    descriptor_union_t ptr1, ptr2;
    UNUSED_ARGUMENT(handle);

    ptr1.pntr = g_config_descriptor;
    ptr2.pntr = g_config_descriptor + CONFIG_DESC_SIZE;

    while (ptr1.word < ptr2.word)
    {
        if (ptr1.common->bDescriptorType == USB_DESC_TYPE_EP)
        {
            if (USB_SPEED_HIGH == speed)
            {
                if (ptr1.ndpt->bEndpointAddress & 0x80)
                {
                    ptr1.ndpt->wMaxPacketSize[0] = USB_uint_16_low(HS_BULK_IN_ENDP_PACKET_SIZE);
                    ptr1.ndpt->wMaxPacketSize[1] = USB_uint_16_high(HS_BULK_IN_ENDP_PACKET_SIZE);
                }
                else
                {
                    ptr1.ndpt->wMaxPacketSize[0] = USB_uint_16_low(HS_BULK_OUT_ENDP_PACKET_SIZE);
                    ptr1.ndpt->wMaxPacketSize[1] = USB_uint_16_high(HS_BULK_OUT_ENDP_PACKET_SIZE);
                }
            }
            else
            {
                if (ptr1.ndpt->bEndpointAddress & 0x80)
                {
                    ptr1.ndpt->wMaxPacketSize[0] = USB_uint_16_low(FS_BULK_IN_ENDP_PACKET_SIZE);
                    ptr1.ndpt->wMaxPacketSize[1] = USB_uint_16_high(FS_BULK_IN_ENDP_PACKET_SIZE);
                }
                else
                {
                    ptr1.ndpt->wMaxPacketSize[0] = USB_uint_16_low(FS_BULK_OUT_ENDP_PACKET_SIZE);
                    ptr1.ndpt->wMaxPacketSize[1] = USB_uint_16_high(FS_BULK_OUT_ENDP_PACKET_SIZE);
                }
            }
        }
        ptr1.word += ptr1.common->bLength;
    }

    for (int i = 0; i < MSC_DESC_ENDPOINT_COUNT; i++)
    {
        if (USB_SPEED_HIGH == speed)
        {
            if (USB_SEND == ep[i].direction)
            {
                ep[i].size = HS_BULK_IN_ENDP_PACKET_SIZE;
            }
            else
            {
                ep[i].size = HS_BULK_OUT_ENDP_PACKET_SIZE;
            }
        }
        else
        {
            if (USB_SEND == ep[i].direction)
            {
                ep[i].size = FS_BULK_IN_ENDP_PACKET_SIZE;
            }
            else
            {
                ep[i].size = FS_BULK_OUT_ENDP_PACKET_SIZE;
            }
        }
    }

    return USB_OK;
}

usb_desc_request_notify_struct_t desc_callback =
{
    USB_Desc_Get_Descriptor,
    USB_Desc_Get_Interface,
    USB_Desc_Set_Interface,
    USB_Set_Configuration,
    USB_Desc_Get_Entity
};
/* EOF */
