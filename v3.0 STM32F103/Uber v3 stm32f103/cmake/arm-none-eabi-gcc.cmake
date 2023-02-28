set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(ARM_COMPILER_EXT "")
set(ARM_COMPILER_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/gcc-arm-none-eabi-10.3-2021.10/bin)

set(CMAKE_C_COMPILER ${ARM_COMPILER_PATH}/arm-none-eabi-gcc${ARM_COMPILER_EXT})
set(CMAKE_CXX_COMPILER
    ${ARM_COMPILER_PATH}/arm-none-eabi-g++${ARM_COMPILER_EXT})
set(CMAKE_ASM_COMPILER
    ${ARM_COMPILER_PATH}/arm-none-eabi-gcc${ARM_COMPILER_EXT})
set(CMAKE_LINKER ${ARM_COMPILER_PATH}/arm-none-eabi-gcc${ARM_COMPILER_EXT})
set(CMAKE_SIZE_UTIL ${ARM_COMPILER_PATH}/arm-none-eabi-size${ARM_COMPILER_EXT})
set(CMAKE_OBJCOPY ${ARM_COMPILER_PATH}/arm-none-eabi-objcopy${ARM_COMPILER_EXT})
set(CMAKE_OBJDUMP ${ARM_COMPILER_PATH}/arm-none-eabi-objdump${ARM_COMPILER_EXT})
set(CMAKE_NM_UTIL ${ARM_COMPILER_PATH}/arm-none-eabi-gcc-nm${ARM_COMPILER_EXT})
set(CMAKE_AR ${ARM_COMPILER_PATH}/arm-none-eabi-gcc-ar${ARM_COMPILER_EXT})
set(CMAKE_RANLIB
    ${ARM_COMPILER_PATH}/arm-none-eabi-gcc-ranlib${ARM_COMPILER_EXT})

# Perform compiler test with the static library
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
