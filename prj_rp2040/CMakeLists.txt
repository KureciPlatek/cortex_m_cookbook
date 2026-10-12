cmake_minimum_required(VERSION 3.20)
message("Project RP2040")

# Be sure to have configured your PATHs for STM32 HAL/LL drivers and ThreadX in the following file:
include(${CMAKE_SOURCE_DIR}/cortex_m_toolchain.cmake)

# --- Select desired project amongst those supported
set (MCU_FAMILY_UPPERCASE ${UPPERCASE_MCU_FAMILY_RP})
set (MCU_FAMILY_LOWERCASE ${LOWERCASE_MCU_FAMILY_RP})

# --- Pico SDK configuration
set(PICO_BOARD pico)             # Other values: pico2_w, pico_w and pico2
set(picotool_DIR /opt/picotool)
set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/../../pico-sdk)
include(${CMAKE_SOURCE_DIR}/pico_sdk_import.cmake)

# --- Set project architecture parameters
set (MCPU         ${MCPU_CORTEX_M0})
set (MCPU_PATH    ${MCPU_CORTEX_M0_PATH})

# --- Selection of RTOS
set (RTOS_THREADX 1)
include(${CMAKE_SOURCE_DIR}/cortex_m_rtos.cmake)

# --- Project name and declaration
set (CMAKE_PROJECT_NAME prj_${MCU_FAMILY_LOWERCASE})
project(${CMAKE_PROJECT_NAME} C CXX ASM)

# --- Init Pico SDK environment
pico_sdk_init()

# --- Project/mcu target variables
set (MFPU       ${MFPU_FPV4_SP_D16})
set (MFLOAT_ABI ${MFLOAT_ABI_HARD})

# --- Includes
set(INCLUDE_DIR
   ${CMAKE_SOURCE_DIR}/app
   ${THREADX_INCLUDE_DIR}
)

# --- Sources
# @todo: use target_sources() to avoid specifing those at add_executable() cmd
set (PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/app/main.c
                     ${CMAKE_SOURCE_DIR}/app/app_threadx.c
                     ${CMAKE_SOURCE_DIR}/system/tx_initialize_low_level.S
                     ${THREADX_PROJECT_SOURCES}
)

# --- Linker
# Not required as already handled by PICO-SDK

# --- Project preprocessors
add_compile_definitions(
#   configNUMBER_OF_CORES=1
#   configSUPPORT_STATIC_ALLOCATION=0
#   configSUPPORT_DYNAMIC_ALLOCATION=1
)

# --- Project compilation, assembler and link flags/parameters:
# Not required as already handled by PICO-SDK

# --- Create project's Makefile
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

# Configure some common used peripherals:
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)

# Add project specific libraries
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# Add pico-sdk libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
   pico_stdlib
   cmsis_core
)

# Add .elf and .hex files
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
