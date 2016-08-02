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

# DEBUG LINK FILE
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T../KSDK_1.2.0/platform/devices/MKL27Z4/linker/gcc/MKL27Z256xxx4_flash.ld  -static")

# RELEASE LINK FILE
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T../KSDK_1.2.0/platform/devices/MKL27Z4/linker/gcc/MKL27Z256xxx4_flash.ld  -static")

# DEBUG ASM FLAGS
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g  -mcpu=cortex-m0plus  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99")

# DEBUG C FLAGS
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g  -mcpu=cortex-m0plus  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99")

# DEBUG LD FLAGS
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g  --specs=nano.specs  -lm  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mthumb  -mapcs  -Xlinker --gc-sections  -Xlinker -static  -Xlinker -z  -Xlinker muldefs  -Xlinker --defsym=__stack_size__=0x300  -Xlinker --defsym=__heap_size__=0x200 -Xlinker --defsym=__ram_vector_table__=1")

# RELEASE ASM FLAGS
SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m0plus  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99")

# RELEASE C FLAGS
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mcpu=cortex-m0plus  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mapcs  -std=gnu99")

# RELEASE LD FLAGS
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} --specs=nano.specs  -lm  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mthumb  -mapcs  -Xlinker --gc-sections  -Xlinker -static  -Xlinker -z  -Xlinker muldefs  -Xlinker --defsym=__stack_size__=0x300  -Xlinker --defsym=__heap_size__=0x200 -Xlinker --defsym=__ram_vector_table__=1")

# ASM MACRO
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -DDEBUG")

# C MACRO
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DDEBUG")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DCPU_MKL27Z256VLH4")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFRDM_KL27Z")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFREEDOM")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DCPU_MKL27Z256VLH4")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFRDM_KL27Z")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFREEDOM")

# CXX MACRO

# INCLUDE_DIRECTORIES
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/osa/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/devices)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/devices/MKL27Z4/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/adapter/sources)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/adapter/sources/sdk)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/include/frdmkl27z)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/common)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/include/config)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/msd)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/controller/khci)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/controller)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/hal)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/hal/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/drivers/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/system/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/.)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/osa/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/devices)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/devices/MKL27Z4/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/adapter/sources)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/adapter/sources/sdk)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/include/frdmkl27z)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/common)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/include/config)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/classes/msd)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/controller/khci)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/sources/controller)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/hal)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/hal/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/drivers/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/system/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/.)
ENDIF()

# ADD_EXECUTABLE
ADD_EXECUTABLE(hello_world 
    "${ProjDirPath}/../KSDK_1.2.0/platform/utilities/src/fsl_debug_console.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/utilities/inc/fsl_debug_console.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/utilities/src/print_scan.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/utilities/src/print_scan.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/devices/MKL27Z4/startup/gcc/startup_MKL27Z4.S"
    "${ProjDirPath}/../KSDK_1.2.0/platform/devices/MKL27Z4/startup/system_MKL27Z4.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/devices/startup.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/devices/startup.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/dma/fsl_dma_irq.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flexio/fsl_flexio_irq.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/lptmr/fsl_lptmr_irq.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/lpuart/fsl_lpuart_irq.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/spi/fsl_spi_irq.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/CopyToRam.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/DEFlashPartition.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashGetProtection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/DFlashSetProtection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/EEEWrite.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMGetProtection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/EERAMSetProtection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCheckSum.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashCommandSequence.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseAllBlock.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseBlock.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseResume.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSector.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashEraseSuspend.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashGetSecurityState.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashInit.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgram.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramCheck.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramOnce.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashProgramSection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadOnce.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashReadResource.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashSecurityBypass.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyAllBlock.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifyBlock.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/FlashVerifySection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashGetProtection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSetProtection.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwap.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/PFlashSwapCtl.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/source/SetEEEEnable.c"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include/FTFx_KX_flash_config.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include/SSD_FTFx.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include/SSD_FTFx_Common.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include/SSD_FTFx_Internal.h"
    "${ProjDirPath}/../KSDK_1.2.0/platform/drivers/src/flash/C90TFS/drvsrc/include/SSD_Types.h"
    "${ProjDirPath}/main.c"
    "${ProjDirPath}/epaper.c"
    "${ProjDirPath}/epaper.h"
    "${ProjDirPath}/text.c"
    "${ProjDirPath}/text.h"
    "${ProjDirPath}/radio.c"
    "${ProjDirPath}/radio.h"
    "${ProjDirPath}/RFM69registers.h"
    "${ProjDirPath}/disk.c"
    "${ProjDirPath}/disk.h"
    "${ProjDirPath}/usb_descriptor.c"
    "${ProjDirPath}/usb_descriptor.h"
    "${ProjDirPath}/flash.c"
    "${ProjDirPath}/flash.h"
)
SET_TARGET_PROPERTIES(hello_world PROPERTIES OUTPUT_NAME "hello_world.elf")

TARGET_LINK_LIBRARIES(hello_world -Wl,--start-group)
# LIBRARIES
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    TARGET_LINK_LIBRARIES(hello_world ${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/build/armgcc/usbd_sdk_frdmkl27z_bm/debug/libusbd_bm.a)
    TARGET_LINK_LIBRARIES(hello_world ${ProjDirPath}/../KSDK_1.2.0/lib/ksdk_platform_lib/armgcc/KL27Z4/debug/libksdk_platform.a)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    TARGET_LINK_LIBRARIES(hello_world ${ProjDirPath}/../KSDK_1.2.0/usb/usb_core/device/build/armgcc/usbd_sdk_frdmkl27z_bm/release/libusbd_bm.a)
    TARGET_LINK_LIBRARIES(hello_world ${ProjDirPath}/../KSDK_1.2.0/lib/ksdk_platform_lib/armgcc/KL27Z4/release/libksdk_platform.a)
ENDIF()

# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(hello_world m)
TARGET_LINK_LIBRARIES(hello_world c)
TARGET_LINK_LIBRARIES(hello_world gcc)
TARGET_LINK_LIBRARIES(hello_world nosys)
TARGET_LINK_LIBRARIES(hello_world -Wl,--end-group)

# MAP FILE
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}  -Xlinker -Map=debug/hello_world.map")
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}  -Xlinker -Map=release/hello_world.map")

# BIN AND HEX
ADD_CUSTOM_COMMAND(TARGET hello_world POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Oihex ${EXECUTABLE_OUTPUT_PATH}/hello_world.elf ${EXECUTABLE_OUTPUT_PATH}/hello_world.hex)
ADD_CUSTOM_COMMAND(TARGET hello_world POST_BUILD COMMAND ${CMAKE_OBJCOPY} -Obinary ${EXECUTABLE_OUTPUT_PATH}/hello_world.elf ${EXECUTABLE_OUTPUT_PATH}/hello_world.bin)

# vim: set expandtab ts=4 sw=4:
