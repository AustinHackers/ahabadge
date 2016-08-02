INCLUDE(CMakeForceCompiler)

# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)

SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

 
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

# DEBUG ASM FLAGS
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -g  -mcpu=cortex-m0plus  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99 ")

# DEBUG C FLAGS
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g  -Wno-format  -fno-strict-aliasing  -mcpu=cortex-m0plus  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99")

# RELEASE ASM FLAGS
SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE}  -mcpu=cortex-m0plus  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99 ")

# RELEASE C FLAGS
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-format  -fno-strict-aliasing  -mcpu=cortex-m0plus  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99")

# ASM MACRO
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -DDEBUG")

# C MACRO
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -D_DEBUG=1")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DCPU_MKL27Z256VLH4")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFRDM_KL27Z")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFREEDOM")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -D_DEBUG=0")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DCPU_MKL27Z256VLH4")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFRDM_KL27Z")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFREEDOM")

# CXX MACRO

# INCLUDE_DIRECTORIES
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../adapter/sources)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../adapter/sources/sdk)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/common)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/include/config)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/msd)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/controller)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/controller/khci)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../hal)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/osa/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/system/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/hal/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/drivers/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../rtos)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices/MKL27Z4/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices/MKL27Z4)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../include/frdmkl27z)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../adapter/sources)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../adapter/sources/sdk)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/common)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/include/config)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/classes/msd)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/controller)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../sources/controller/khci)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../hal)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/osa/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/system/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/hal/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/drivers/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../rtos)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices/MKL27Z4/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../../platform/devices/MKL27Z4)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../include/frdmkl27z)
ENDIF()

# ADD_LIBRARY
ADD_LIBRARY(KsdkUsbDevicePlatformLib STATIC
    "${ProjDirPath}/../../../../../adapter/sources/adapter.h"
    "${ProjDirPath}/../../../../../adapter/sources/adapter_types.h"
    "${ProjDirPath}/../../../../../adapter/sources/sdk/adapter_cfg.h"
    "${ProjDirPath}/../../../../../adapter/sources/sdk/adapter_sdk.c"
    "${ProjDirPath}/../../../../../adapter/sources/sdk/adapter_sdk.h"
    "${ProjDirPath}/../../../../include/compiler.h"
    "${ProjDirPath}/../../../../include/types.h"
    "${ProjDirPath}/../../../../include/usb.h"
    "${ProjDirPath}/../../../../include/usb_desc.h"
    "${ProjDirPath}/../../../../include/usb_error.h"
    "${ProjDirPath}/../../../../include/usb_misc.h"
    "${ProjDirPath}/../../../../include/usb_opt.h"
    "${ProjDirPath}/../../../../include/usb_types.h"
    "${ProjDirPath}/../../../include/usb_device_stack_interface.h"
    "${ProjDirPath}/../../../sources/classes/include/usb_class.h"
    "${ProjDirPath}/../../../sources/classes/include/usb_class_msc.h"
    "${ProjDirPath}/../../../sources/classes/common/usb_class.c"
    "${ProjDirPath}/../../../sources/classes/common/usb_class_internal.h"
    "${ProjDirPath}/../../../sources/classes/msd/usb_msc.c"
    "${ProjDirPath}/../../../sources/classes/msd/usb_msc.h"
    "${ProjDirPath}/../../../sources/classes/msd/usb_msc_scsi.c"
    "${ProjDirPath}/../../../sources/classes/msd/usb_msc_scsi.h"
    "${ProjDirPath}/../../../sources/controller/usb_dev.c"
    "${ProjDirPath}/../../../sources/controller/usb_dev.h"
    "${ProjDirPath}/../../../sources/controller/usb_framework.c"
    "${ProjDirPath}/../../../sources/controller/usb_framework.h"
    "${ProjDirPath}/../../../sources/controller/khci/device_khci_interface.c"
    "${ProjDirPath}/../../../sources/controller/khci/khci_dev.c"
    "${ProjDirPath}/../../../sources/controller/khci/khci_dev.h"
    "${ProjDirPath}/../../../sources/controller/khci/khci_dev_misc.h"
    "${ProjDirPath}/../../../include/frdmkl27z/usb_device_config.h"
    "${ProjDirPath}/../../../sources/bsp/frdmkl27z/usb_dev_bsp.c"
)

SET_TARGET_PROPERTIES(KsdkUsbDevicePlatformLib PROPERTIES OUTPUT_NAME "libusbd_bm.a")
