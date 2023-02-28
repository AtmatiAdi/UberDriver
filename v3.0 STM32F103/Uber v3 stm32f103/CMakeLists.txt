cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE UberDriver)
project(${EXECUTABLE} LANGUAGES C CXX ASM)

# find -regex '.*\.c' >> CMakeLists.txt
add_executable(
  ${EXECUTABLE}
  ./Core/Startup/startup_stm32f103tbux.s
  ./Core/Src/DRV8305.c
  ./Core/Src/control.c
  ./Core/Src/sysmem.c
  ./Core/Src/system_stm32f1xx.c
  ./Core/Src/main.c
  ./Core/Src/stm32f1xx_hal_msp.c
  ./Core/Src/application.c
  ./Core/Src/stm32f1xx_it.c
  ./Core/Src/syscalls.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
  ./Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c)

# find -regex '.*\.h' >> CMakeLists.txt
target_include_directories(
  ${EXECUTABLE}
  PRIVATE ./Core/Inc ./Drivers/STM32F1xx_HAL_Driver/Inc
          ./Drivers/STM32F1xx_HAL_Driver/Inc/Legacy ./Drivers/CMSIS/Include
          ./Drivers/CMSIS/Device/ST/STM32F1xx/Include)
# set(CMAKE_ASM_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=hard
# -mfpu=fpv4-sp-d16) set(CMAKE_C_FLAGS -mcpu=cortex-m4 -mthumb -mfloat-abi=hard
# -mfpu=fpv4-sp-d16 -fno-common -fsigned-char -fmessage-length=0 -Wall -Wextra
# -Wshadow -Os -ffunction-sections -data-sections -g3 -std=c11)
target_compile_definitions(${EXECUTABLE} PRIVATE USE_HAL_DRIVER STM32F103xB)
target_compile_options(${EXECUTABLE} PRIVATE -mcpu=cortex-m3 -mfloat-abi=soft
                                             -mthumb)
target_link_options(
  ${EXECUTABLE}
  PRIVATE
  -mcpu=cortex-m3
  -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F103TBUX_FLASH.ld
  --specs=nosys.specs
  -Wl,-Map="${EXECUTABLE}.map"
  -Wl,--gc-sections
  -static
  --specs=nano.specs
  -mfloat-abi=soft
  -mthumb
  -Wl,--start-group
  -lc
  -lm
  -Wl,--end-group)
