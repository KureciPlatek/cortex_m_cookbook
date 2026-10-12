cmake_minimum_required(VERSION 3.20)
message("Project RP2350")

# Be sure to have configured your PATHs for STM32 HAL/LL drivers and ThreadX in the following file:
include(${CMAKE_SOURCE_DIR}/cortex_m_toolchain.cmake)

# --- Select desired project amongst those supported
set (MCU_FAMILY_UPPERCASE ${UPPERCASE_MCU_FAMILY_RP})
set (MCU_FAMILY_LOWERCASE ${LOWERCASE_MCU_FAMILY_RP})

# --- Pico SDK configuration
set(PICO_BOARD pico2_w)          # Other values: pico, pico_w and pico2
set(picotool_DIR /opt/picotool)
set(PICO_SDK_PATH ${PRJ_EXTSRC_PATH}/pico-sdk)
include(${CMAKE_SOURCE_DIR}/pico_sdk_import.cmake)

# --- Set project architecture parameters
set (MCPU         ${MCPU_CORTEX_M33})
set (MCPU_PATH    ${MCPU_CORTEX_M33_PATH})

# --- Selection of RTOS
set (RTOS_FREERTOS 1)   # Selection of RTOS, choose between RTOS_THREADX and RTOS_FREERTOS
include(${CMAKE_SOURCE_DIR}/cortex_m_rtos.cmake)

# --- Project name and declaration
set (CMAKE_PROJECT_NAME prj_${MCU_FAMILY_LOWERCASE})
project(${CMAKE_PROJECT_NAME} C CXX ASM)

# --- Init Pico SDK environment
pico_sdk_init()

# --- Project/mcu target variables
set (MFPU       ${MFPU_FPV4_SP_D16})
set (MFLOAT_ABI ${MFLOAT_ABI_HARD})

# Import FreeRTOS with CMake. Take directly from FreeRTOS repo
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

# --- Includes
set(INCLUDE_DIR
   ${CMAKE_SOURCE_DIR}/app
#   ${CMAKE_SOURCE_DIR}/lib
)

# --- Sources
# @todo: use target_sources() to avoid specifing those at add_executable() cmd
set (PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/app/main.c
                     ${CMAKE_SOURCE_DIR}/app/app_freeRtos.c
                     ${CMAKE_SOURCE_DIR}/app/comm_with_flaw.c
                     ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
)

# --- Project preprocessors
add_compile_definitions(
   FREE_RTOS_KERNEL_SMP=1
#   configNUMBER_OF_CORES=2 # Overwrite number of cores. Set to 2 if use dual
   CYW43_LWIP=0
   PICO_CYW43_ARCH_FREERTOS=1
   PICO_STDIO_UART_DEFAULT_BAUDRATE=921600 # Set printf() over UART baudrate
)

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
   pico_cyw43_arch_sys_freertos
   hardware_adc
   FreeRTOS-Kernel
   FreeRTOS-Kernel-Heap4 # Could be -Static or any other heap implementation proposed by FreeRTOS
)

# Add .elf and .hex files
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
