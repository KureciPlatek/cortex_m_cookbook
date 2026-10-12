# Set GNU-ARM toolchain parameters, path, variables and so on 
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Variables to sort out which STM32XX or RP2XXX family we are compiling for
set (UPPERCASE_MCU_FAMILY_H7 H7)
set (LOWERCASE_MCU_FAMILY_H7 h7)
set (UPPERCASE_MCU_FAMILY_F4 F4)
set (LOWERCASE_MCU_FAMILY_F4 f4)
set (UPPERCASE_MCU_FAMILY_N6 N6)
set (LOWERCASE_MCU_FAMILY_N6 n6)
set (UPPERCASE_MCU_FAMILY_RP RP)
set (LOWERCASE_MCU_FAMILY_RP rp)

# Variables to sort Cortex-M architecture family
set (MCPU_CORTEX_M55       "cortex-m55")
set (MCPU_CORTEX_M55_PATH  cortex_m55)
set (MCPU_CORTEX_M7        "cortex-m7")
set (MCPU_CORTEX_M7_PATH   cortex_m7)
set (MCPU_CORTEX_M4        "cortex-m4")
set (MCPU_CORTEX_M4_PATH   cortex_m4)
set (MCPU_CORTEX_M0        "cortex-m0")
set (MCPU_CORTEX_M0_PATH   cortex_m0)
set (MCPU_CORTEX_M33       "cortex-m33")
set (MCPU_CORTEX_M33_PATH  cortex_m33)

# Some GNU-ARM compiler options
set (MFLOAT_ABI_SOFT       "soft")
set (MFLOAT_ABI_HARD       "hard")
set (MFLOAT_ABI_MIX        "softfp")
set (MFPU_FPV4_SP_D16      "fpv4-sp-d16")
set (MFPU_FPV5_D16         "fpv5-d16")
set (RUNTIME_LIB_SYSMEM    "nano.specs")
set (RUNTIME_LIB_SYSCALLS  "nosys.specs")
# set (SECURE_CODE           "-mcmse")   # On H7, target CPU does not support ARMv8-M Security Extensions

message("Running on ${CMAKE_HOST_SYSTEM_NAME}")
# Here I set variable PRJ_EXTSRC_PATH in my bash env to ~/work and put all external source/repositories there.
# Then I fetch those path in other cmake files. How you prefer to fetch those external source/repos is up to you
# you may adapt this as you please
# In my .bash_env I have following line:
# export PRJ_EXTSRC_PATH=~/work
if(DEFINED ENV{PRJ_EXTSRC_PATH} AND NOT "$ENV{PRJ_EXTSRC_PATH}" STREQUAL "")
   set(PRJ_EXTSRC_PATH $ENV{PRJ_EXTSRC_PATH})
else()
   message(FATAL_ERROR "Environment variable PRJ_EXTSRC_PATH not set. Please give path to ThreadX and STMCubeHAL")
endif()

message(STATUS "External source path: ${PRJ_EXTSRC_PATH}")

# Specify the cross-compiler
set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_OBJCOPY       arm-none-eabi-objcopy)
set(CMAKE_SIZE          arm-none-eabi-size)
set(CMAKE_LINKER        arm-none-eabi-ld)

# Prevent CMake from testing the compiler
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Search for programs in the host environment, libraries and headers in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

