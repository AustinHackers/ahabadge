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
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#include "fsl_device_registers.h"
#include "fsl_clock_manager.h"
//#include "board.h"
#include "fsl_debug_console.h"
#if (TWR_K64F120M || FRDM_K64F || TWR_K60D100M || TWR_K21F120M || TWR_K65F180M)
#include "fsl_mpu_hal.h"
#endif
#include "fsl_port_hal.h"
#include <stdio.h>
#include <stdlib.h>
#endif

#if SD_CARD_APP
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
#include "sd_esdhc_kinetis.h"
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#include "fsl_sdhc_card.h"
#include "fsl_gpio_driver.h"
#include "gpio_pins.h"
#include "board.h"
#else
#include "sd.h"
#endif
#endif

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

extern void Main_Task(uint32_t param);
#define MAIN_TASK       10

#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
    { MAIN_TASK, Main_Task, 2 * 3000L, 9L, "Main", MQX_AUTO_START_TASK, 0, 0 },
    { 0L, 0L, 0L, 0L, 0L, 0L, 0, 0 }
};

MQX_FILE_PTR g_sdcard_handle;
#endif
/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
void TestApp_Init(void);

/****************************************************************************
 * Global Variables
 ****************************************************************************/
/* Add all the variables needed for disk.c to this structure */
extern usb_desc_request_notify_struct_t desc_callback;
usb_application_callback_struct_t msc_application_callback;
usb_vendor_req_callback_struct_t vend_req_callback;
usb_class_specific_callback_struct_t class_specific_callback;

msc_config_struct_t g_msd_config;
#if (RAM_DISK_APP)
/* Here set storage_disk value to put the 24k buffer to RW region, 
 if putting in ZI and 64k ram the ZI region cannot be put in upper/bottom 32k ram for iar */
disk_struct_t g_disk =
{
    .storage_disk = {0x01},
};
#else
disk_struct_t g_disk;
#endif

#if SD_CARD_APP
    #define USE_SDHC_PROTOCOL    (1)
    #define USE_SPI_PROTOCOL     (0)
#endif

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
void Disk_App(void);
/*****************************************************************************
 * Local Variables 
 *****************************************************************************/
#if SD_CARD_APP
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#define SDCARD_BLOCK_SIZE_POWER   (9)
sdhc_card_t card = {0};
sdhc_host_t host = {0};
#endif
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM || OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
uint32_t g_msc_bulk_out_buff_temp[MSD_RECV_BUFFER_NUM][MSD_RECV_BUFFER_SIZE >> 2];
uint32_t g_msc_bulk_in_buff_temp[MSD_SEND_BUFFER_SIZE >> 2];
#endif
uint8_t *g_msc_bulk_out_buff;
uint8_t *g_msc_bulk_in_buff;
#endif
/*****************************************************************************
 * Local Functions
 *****************************************************************************/
#if SD_CARD_APP
os_msgq_handle write_buffer_msg_handler;

/******************************************************************************
 *  
 *   @name        get_write_buffer
 * 
 *   @brief       This function get the buffer for writing
 * 
 *   @param       None
 * 
 *   @return      buffer pointer (return NULL when there is no idle buffer )
 **                
 *****************************************************************************/
uint32_t* get_write_buffer(void)
{
    uint32_t msg;
    /* wait forever for RTOS; return immediately for bm */
    if (OS_MsgQ_recv(write_buffer_msg_handler, (void*)&msg, OS_MSGQ_RECEIVE_BLOCK_ON_EMPTY, 0) == OS_MSGQ_OK)
    {
        return (uint32_t*)msg;
    }
    return NULL;
}

/******************************************************************************
 *  
 *   @name        get_write_buffer
 * 
 *   @brief       This function get the buffer for writing
 * 
 *   @param       buffer : The buffer pointer
 * 
 *   @return      None
 **                
 *****************************************************************************/
void free_write_buffer(uint32_t* buffer)
{
    uint32_t address = (uint32_t)buffer;
    OS_MsgQ_send(write_buffer_msg_handler, (void*)(&address), 0);
}
#endif

#if MUTILE_BUFFER && (((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK) && (USE_RTOS)) || (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX))
os_msgq_handle write_msg_handler;
os_mutex_handle sdhc_mutex;

static void write_task(void *arg)
{
    uint8_t* buff_ptr;
    uint32_t offset;
    uint32_t size;
    uint32_t msg[3];
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
    sdhc_status_t error_code = kStatus_SDHC_NoError;
#else
    _mqx_uint error_code = MQX_OK;
#endif

    while (OS_MsgQ_recv(write_msg_handler, (void*)&msg, OS_MSGQ_RECEIVE_BLOCK_ON_EMPTY, 0) == OS_MSGQ_OK)
    {
        buff_ptr = (uint8_t*)msg[0];
        offset = msg[1];
        size = msg[2];

        OS_Mutex_lock(sdhc_mutex);
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
        error_code = SDCARD_DRV_WriteBlocks(&card, buff_ptr, offset >> SDCARD_BLOCK_SIZE_POWER, size >> SDCARD_BLOCK_SIZE_POWER);
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
        error_code = sd_write_device_sectors(g_sdcard_handle, offset >> SDCARD_BLOCK_SIZE_POWER, size >> SDCARD_BLOCK_SIZE_POWER, 3, buff_ptr, NULL);
#endif
        OS_Mutex_unlock(sdhc_mutex);
        free_write_buffer((uint32_t*)buff_ptr);
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
        if (error_code != kStatus_SDHC_NoError)
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
        if (error_code != MQX_OK)
#endif
        {
            g_disk.read_write_error = 1;
            USB_PRINTF("Write SD card error, error = 0x%x.\tPlease check recv buff size(must be less than 128 sectors).\r\n", error_code);
        }
    }
}
#endif

#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK && SD_CARD_APP)
#if defined(FSL_RTOS_MQX)
#include "fsl_sdhc_driver.h"
void SDHC_MQX_IRQHandler(void)
{
#if defined BSP_FSL_SDHC_USING_IRQ
    SDHC_DRV_DoIrq(0);
#endif
}
#endif

/* don't use interrupt to detect the sdcard */
gpio_input_pin_user_config_t sdhc_detect_pin[2];

void sdhc_detect_io_init(void)
{
    for (int i = 0; i < 2; ++i)
    {
        sdhc_detect_pin[i].pinName = sdhcCdPin[i].pinName;
        sdhc_detect_pin[i].config.isPullEnable = sdhcCdPin[i].config.isPullEnable;
        sdhc_detect_pin[i].config.pullSelect = sdhcCdPin[i].config.pullSelect;
        sdhc_detect_pin[i].config.isPassiveFilterEnabled = sdhcCdPin[i].config.isPassiveFilterEnabled;
        sdhc_detect_pin[i].config.interrupt = kPortIntDisabled; /* don't use interrupt */
    }

    GPIO_DRV_Init(sdhc_detect_pin, NULL);
}

uint8_t sdhc_detect(void)
{
    uint32_t value = 0;
    if (sdhc_detect_pin[0].config.pullSelect == kPortPullUp) /* pull up */
    {
        value = GPIO_DRV_ReadPinInput(sdhc_detect_pin[0].pinName);
        return (!value);
    }
    else /* pull down */
    {
        return (GPIO_DRV_ReadPinInput(sdhc_detect_pin[0].pinName));
    }
}

uint8_t SD_Init(void)
{
    sdhc_user_config_t config = {0};
    /* enable SDHC module instance */
    CLOCK_SYS_EnableSdhcClock(0);
    /* disable mpu for kSdhcTransModeAdma2 */
#if (TWR_K64F120M || FRDM_K64F || TWR_K60D100M || TWR_K21F120M || TWR_K65F180M)
    MPU_HAL_Disable(MPU);
#endif
    /* initialize user sdhc configuration structure */
    config.transMode = kSdhcTransModeAdma2;  //kSdhcTransModePio;
    config.clock = SDMMC_CLK_100KHZ;
    config.cdType = kSdhcCardDetectGpio;
    /* initialize the SDHC driver with the user configuration */
    if (SDHC_DRV_Init(BOARD_SDHC_INSTANCE, &host, &config) != kStatus_SDHC_NoError)
    {
        USB_PRINTF("SDHC_DRV_Init failed\r\n");
        return 0;
    }

#if defined (FSL_RTOS_MQX)
    OS_install_isr(SDHC_IRQn, SDHC_MQX_IRQHandler, NULL);
#endif
#if (USE_RTOS)
    OS_intr_init(SDHC_IRQn, 5, 0, TRUE);
#endif

    /* A card is detected, now initialize the card driver */
    if (kStatus_SDHC_NoError != SDCARD_DRV_Init(&host, &card))
    {
        USB_PRINTF("SDCARD_DRV_Init failed\r\n");
        SDHC_DRV_Shutdown(card.hostInstance);
        return 0;
    }
    return 1;
}

#endif

/******************************************************************************
 * 
 *    @name       Disk_App
 *    
 *    @brief      
 *                  
 *    @param      None
 * 
 *    @return     None
 *    
 *****************************************************************************/
void Disk_App(void)
{
    /* User Code */
    return;
}

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
#if SD_CARD_APP
            *((uint32_t *)val) = (uint32_t)MSD_SEND_BUFFER_SIZE;
#elif RAM_DISK_APP
            *((uint32_t *)val) = (uint32_t)DISK_SIZE_NORMAL;
#endif
        }
    }
    else if (event_type == USB_MSC_DEVICE_GET_RECV_BUFF_INFO)
    {
        if (NULL != val)
        {
#if SD_CARD_APP
            *((uint32_t *)val) = (uint32_t)MSD_RECV_BUFFER_SIZE;
#elif RAM_DISK_APP
            *((uint32_t *)val) = (uint32_t)DISK_SIZE_NORMAL;
#endif
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
#if SD_CARD_APP && (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
    _mqx_uint error_code = MQX_OK;
#endif
#if SD_CARD_APP && (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
    sdhc_status_t error_code = kStatus_SDHC_NoError;
#endif

    if (g_disk.read_write_error)
    {
        return USBERR_ERROR;
    }

    switch(event_type)
    {
    case USB_DEV_EVENT_DATA_RECEIVED:
        /* Add User defined code -- if required*/
        lba_data_ptr = (lba_app_struct_t*) size;
#if RAM_DISK_APP
#elif SD_CARD_APP
        if (lba_data_ptr->size == 0)
        free_write_buffer((uint32_t*)lba_data_ptr->buff_ptr);
        else
        {
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
            SD_Write_Block(lba_data_ptr);
            free_write_buffer((uint32_t*)lba_data_ptr->buff_ptr);
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#if MUTILE_BUFFER && (USE_RTOS)
                    uint32_t msg[3] = {(uint32_t)lba_data_ptr->buff_ptr, lba_data_ptr->offset, lba_data_ptr->size};
            OS_MsgQ_send(write_msg_handler, (void*)(msg), 0);
#else
            error_code = SDCARD_DRV_WriteBlocks(&card, lba_data_ptr->buff_ptr, lba_data_ptr->offset >> SDCARD_BLOCK_SIZE_POWER,
            lba_data_ptr->size >> SDCARD_BLOCK_SIZE_POWER);
            free_write_buffer((uint32_t*)lba_data_ptr->buff_ptr);
            if (error_code != kStatus_SDHC_NoError)
            {
                error = USBERR_ERROR;
                g_disk.read_write_error=1;
                USB_PRINTF("Write SD card error, error = 0x%x.\tPlease check recv buff size(must be less than 128 sectors).\r\n", error_code);
            }
#endif

#else
#if MUTILE_BUFFER
                    uint32_t msg[3] = {(uint32_t)lba_data_ptr->buff_ptr, lba_data_ptr->offset, lba_data_ptr->size};
            OS_MsgQ_send(write_msg_handler, (void*)(msg), 0);
#else
            error_code = sd_write_device_sectors(g_sdcard_handle, (lba_data_ptr->offset >> SDCARD_BLOCK_SIZE_POWER),
            (lba_data_ptr->size >> SDCARD_BLOCK_SIZE_POWER) , 3, lba_data_ptr->buff_ptr,NULL);
            free_write_buffer((uint32_t*)lba_data_ptr->buff_ptr);
            if (error_code != MQX_OK)
            {
                error = USBERR_ERROR;
                g_disk.read_write_error=1;
                USB_PRINTF("Write SD card error, error = 0x%x.\tPlease check recv buff size(must be less than 128 sectors).\r\n", error_code);
            }
#endif
#endif
        }
#endif
        break;
    case USB_DEV_EVENT_SEND_COMPLETE:
        /* Add User defined code -- if required*/
        lba_data_ptr = (lba_app_struct_t*) size;
        if (lba_data_ptr->size != 0)
        {
            /* read data from mass storage device to driver buffer */
#if RAM_DISK_APP
            if(data != NULL)
            {
                *data = g_disk.storage_disk + lba_data_ptr->offset;
            }
#elif SD_CARD_APP
#endif
        }
        break;
    case USB_MSC_START_STOP_EJECT_MEDIA:
        /*  Code to be added by user for starting, stopping or 
         ejecting the disk drive. e.g. starting/stopping the motor in 
         case of CD/DVD*/
        break;
    case USB_MSC_DEVICE_READ_REQUEST:
        lba_data_ptr = (lba_app_struct_t*) size;

#if RAM_DISK_APP
        if(data != NULL)
        {
            *data = g_disk.storage_disk + lba_data_ptr->offset;
        }
#elif SD_CARD_APP
        if(data != NULL)
        {
            *data = g_msc_bulk_in_buff;
        }
        lba_data_ptr->buff_ptr = g_msc_bulk_in_buff;
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        SD_Read_Block(lba_data_ptr);
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#if MUTILE_BUFFER && (USE_RTOS)
        OS_Mutex_lock(sdhc_mutex);
#endif
        error_code = SDCARD_DRV_ReadBlocks(&card, lba_data_ptr->buff_ptr, lba_data_ptr->offset >> SDCARD_BLOCK_SIZE_POWER,
        lba_data_ptr->size >> SDCARD_BLOCK_SIZE_POWER);
#if MUTILE_BUFFER && (USE_RTOS)
        OS_Mutex_unlock(sdhc_mutex);
#endif
        if (error_code != kStatus_SDHC_NoError)
        {
            error = USBERR_ERROR;
            g_disk.read_write_error=1;
            USB_PRINTF("Read SD card error, error = 0x%x.\tPlease check send buff size(must be less than 128 sectors).\r\n", error_code);
        }
#else
#if MUTILE_BUFFER
        OS_Mutex_lock(sdhc_mutex);
#endif
        error_code = sd_read_device_sectors(g_sdcard_handle, (lba_data_ptr->offset >> SDCARD_BLOCK_SIZE_POWER),
        (lba_data_ptr->size >> SDCARD_BLOCK_SIZE_POWER) , 3, lba_data_ptr->buff_ptr,NULL);
#if MUTILE_BUFFER
        OS_Mutex_unlock(sdhc_mutex);
#endif
        if (error_code != MQX_OK)
        {
            error = USBERR_ERROR;
            g_disk.read_write_error=1;
            USB_PRINTF("Read SD card error, error = 0x%x.\tPlease check send buff size(must be less than 128 sectors).\r\n", error_code);
        }
#endif
#endif 
        break;
    case USB_MSC_DEVICE_WRITE_REQUEST:
        lba_data_ptr = (lba_app_struct_t*) size;
#if RAM_DISK_APP
        if(data != NULL)
        {
            *data = g_disk.storage_disk + lba_data_ptr->offset;
        }
#elif SD_CARD_APP
        if(data != NULL)
        {
            /* get_write_buffer should return NULL when there are no buffers for writing */
            *data = (uint8_t*)get_write_buffer();
        }
#endif
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
#if RAM_DISK_APP
        device_lba_info_ptr->total_lba_device_supports = TOTAL_LOGICAL_ADDRESS_BLOCKS_NORMAL;
        device_lba_info_ptr->length_of_each_lab_of_device = LENGTH_OF_EACH_LAB;
#elif SD_CARD_APP
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM)
        SD_Card_Info(&device_lba_info_ptr->total_lba_device_supports,
        &device_lba_info_ptr->length_of_each_lab_of_device);
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
        device_lba_info_ptr->total_lba_device_supports = card.blockCount;
        device_lba_info_ptr->length_of_each_lab_of_device = card.blockSize;
#else
        sd_get_block_size(g_sdcard_handle, &device_lba_info_ptr->length_of_each_lab_of_device);
        sd_get_blocks_num(g_sdcard_handle, &device_lba_info_ptr->total_lba_device_supports);
#endif
#endif
        device_lba_info_ptr->num_lun_supported = LOGICAL_UNIT_SUPPORTED;
        break;
    default:
        break;
    }

    return error;
}

/******************************************************************************
 *  
 *   @name        TestApp_Init
 * 
 *   @brief       This function is the entry for mouse (or other usage)
 * 
 *   @param       None
 * 
 *   @return      None
 **                
 *****************************************************************************/

void APP_init(void)
{
#if SD_CARD_APP

#if ((OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM) || (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK))
    g_msc_bulk_in_buff = (uint8_t*)g_msc_bulk_in_buff_temp;
    g_msc_bulk_out_buff = (uint8_t*)g_msc_bulk_out_buff_temp;
#elif (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
    g_msc_bulk_in_buff = (uint8_t*)OS_Mem_alloc_uncached_align(MSD_SEND_BUFFER_SIZE, 32);
    g_msc_bulk_out_buff = (uint8_t*)OS_Mem_alloc_uncached_align(MSD_RECV_BUFFER_SIZE * MSD_RECV_BUFFER_NUM, 32);
#endif

#endif

#if SD_CARD_APP
    USB_PRINTF("Please insert SD card\n");
#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_BM || OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_SDK)
#if USE_SDHC_PROTOCOL
    sdhc_detect_io_init();
#endif
    while (!sdhc_detect())
    {
        ;
    } /* SD Card inserted */
    if(!SD_Init()) return; /* Initialize SD_CARD and SPI Interface */
#else
    g_sdcard_handle = sd_init();

#endif
    USB_PRINTF("\nSD card inserted!\n");

#endif
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

#if SD_CARD_APP
    write_buffer_msg_handler = OS_MsgQ_create((MSD_RECV_BUFFER_NUM + 1), 1);  //msg_size = 4Bytes
    for (int i = 0; i < MSD_RECV_BUFFER_NUM; ++i)
    {
        uint32_t address = (uint32_t)(&g_msc_bulk_out_buff[i * MSD_RECV_BUFFER_SIZE]);
        OS_MsgQ_send(write_buffer_msg_handler, (void*)(&address), 0);
    }
#endif
}

void APP_task()
{
    if (g_disk.read_write_error)
    {
        return;
    }
    Disk_App();
}

#if (OS_ADAPTER_ACTIVE_OS == OS_ADAPTER_MQX)
/*FUNCTION*----------------------------------------------------------------
 * 
 * Function Name  : Main_Task
 * Returned Value : None
 * Comments       :
 *     First function called.  Calls Test_App
 *     callback functions.
 * 
 *END*--------------------------------------------------------------------*/
void Main_Task
(
    uint32_t param
    )
{
    UNUSED_ARGUMENT (param)
    APP_init();
#if MUTILE_BUFFER
    /* write sdhc task */
    sdhc_mutex = OS_Mutex_create();
    write_msg_handler = OS_MsgQ_create((MSD_RECV_BUFFER_NUM), 3);/* msg_size = 12Bytes */
    OS_Task_create(write_task, NULL, 8L, 2000L, "write_task", NULL);
#endif
}
#endif

/* EOF */
