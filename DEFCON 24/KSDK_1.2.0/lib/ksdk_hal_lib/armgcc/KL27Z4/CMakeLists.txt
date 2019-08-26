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
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -g  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mthumb  -mapcs  -std=gnu99  -mcpu=cortex-m0plus  -mfloat-abi=soft ")

# DEBUG C FLAGS
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mthumb  -mapcs  -std=gnu99  -mcpu=cortex-m0plus  -mfloat-abi=soft  -MMD  -MP")

# RELEASE ASM FLAGS
SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE}  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mthumb  -mapcs  -std=gnu99  -mcpu=cortex-m0plus  -mfloat-abi=soft ")

# RELEASE C FLAGS
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -Os  -mthumb  -mapcs  -std=gnu99  -mcpu=cortex-m0plus  -mfloat-abi=soft  -MMD  -MP")

# ASM MACRO
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -DDEBUG")

# C MACRO
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DDEBUG")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DCPU_MKL27Z256VLH4")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DCPU_MKL27Z256VLH4")

# CXX MACRO

# INCLUDE_DIRECTORIES
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices/MKL27Z4/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/hal/inc)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices/MKL27Z4/include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/utilities/inc)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/hal/inc)
ENDIF()

# ADD_LIBRARY
ADD_LIBRARY(KsdkHalLib STATIC
    "${ProjDirPath}/../../../../platform/hal/src/adc16/fsl_adc16_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_adc16_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/dac/fsl_dac_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_dac_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/dma/fsl_dma_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_dma_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/dmamux/fsl_dmamux_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_dmamux_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/cmp/fsl_cmp_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_cmp_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/cop/fsl_cop_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_cop_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/gpio/fsl_gpio_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_gpio_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/i2c/fsl_i2c_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_i2c_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/llwu/fsl_llwu_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_llwu_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/lptmr/fsl_lptmr_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_lptmr_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/mcglite/fsl_mcglite_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_mcglite_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/mcglite/fsl_mcglite_hal_modes.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_mcglite_hal_modes.h"
    "${ProjDirPath}/../../../../platform/hal/src/osc/fsl_osc_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_osc_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/pit/fsl_pit_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_pit_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/port/fsl_port_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_port_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/rcm/fsl_rcm_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_rcm_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/rtc/fsl_rtc_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_rtc_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/sim/MKL27Z4/fsl_sim_hal_MKL27Z4.c"
    "${ProjDirPath}/../../../../platform/hal/src/sim/MKL27Z4/fsl_sim_hal_MKL27Z4.h"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_sim_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/smc/fsl_smc_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_smc_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/spi/fsl_spi_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_spi_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/sai/fsl_sai_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_sai_hal.h"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_tpm_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/tpm/fsl_tpm_hal.c"
    "${ProjDirPath}/../../../../platform/hal/src/lpuart/fsl_lpuart_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_lpuart_hal.h"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_uart_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/uart/fsl_uart_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_pmc_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/flexio/fsl_flexio_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_flexio_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/flexio/fsl_flexio_uart_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_flexio_uart_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/flexio/fsl_flexio_spi_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_flexio_spi_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/flexio/fsl_flexio_i2s_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_flexio_i2s_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/flexio/fsl_flexio_i2c_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_flexio_i2c_hal.h"
    "${ProjDirPath}/../../../../platform/hal/src/vref/fsl_vref_hal.c"
    "${ProjDirPath}/../../../../platform/hal/inc/fsl_vref_hal.h"
)

SET_TARGET_PROPERTIES(KsdkHalLib PROPERTIES OUTPUT_NAME "libksdk_hal.a")
