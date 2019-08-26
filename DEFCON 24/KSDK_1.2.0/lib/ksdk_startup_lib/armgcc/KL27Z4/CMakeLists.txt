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
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices/MKL27Z4/startup)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/CMSIS/Include)
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../platform/devices)
ENDIF()

# ADD_LIBRARY
ADD_LIBRARY(KsdkStartupLib STATIC
    "${ProjDirPath}/../../../../platform/devices/MKL27Z4/startup/system_MKL27Z4.c"
    "${ProjDirPath}/../../../../platform/devices/startup.c"
    "${ProjDirPath}/../../../../platform/devices/startup.h"
)

SET_TARGET_PROPERTIES(KsdkStartupLib PROPERTIES OUTPUT_NAME "libksdk_startup.a")
