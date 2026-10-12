# Cmake subfile to list required inputs for RTOS we want to use

if (DEFINED RTOS_THREADX)
   message(FATAL_ERROR "ThreadX not supported on RP2350 yet")
elseif(DEFINED RTOS_FREERTOS)
   message(STATUS  "Selected RTOS: FreeRTOS")

   if(NOT DEFINED FREERTOS_KERNEL_PATH)
      set(FREERTOS_KERNEL_PATH ${PRJ_EXTSRC_PATH}/FreeRTOS-Kernel)
      message(STATUS "FREERTOS_KERNEL_PATH = ${FREERTOS_KERNEL_PATH}")
   endif()

else()
   message (WARNING "No RTOS selected")
endif()

