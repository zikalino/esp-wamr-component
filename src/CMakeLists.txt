# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# WAMR definitions
set(wasm_inc "${WAMR_ROOT_DIR}/core/iwasm/include"
             "${WAMR_ROOT_DIR}/core/iwasm/libraries/libc-wasi/"
             "${WAMR_ROOT_DIR}/core/iwasm/common")
add_definitions(-DWASM_ALIGN_SIZE=4)
set (WAMR_BUILD_TARGET "XTENSA")
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_PLATFORM "esp-idf")
set(WAMR_ROOT_DIR "/esp-wamr-component/wasm-micro-runtime")

# the rest of stuff
# include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)


project(hello_world)

# try to include wamr subfolder

link_directories(${WAMR_ROOT_DIR})
