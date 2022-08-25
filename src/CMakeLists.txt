# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(WAMR_ROOT_DIR "/esp-wamr-component/wasm-micro-runtime")
set(wasm_inc "${WAMR_ROOT_DIR}/core/iwasm/include"
             "${WAMR_ROOT_DIR}/core/iwasm/libraries/libc-wasi/"
             "${WAMR_ROOT_DIR}/core/iwasm/common")

# WAMR cmake scripts

add_definitions(-DWASM_ALIGN_SIZE=4)
set (WAMR_BUILD_TARGET "XTENSA")
set(WAMR_BUILD_INTERP 1)
set(WAMR_BUILD_PLATFORM "esp-idf")

include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(wamrlib ${WAMR_RUNTIME_LIB_SOURCE})

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

target_link_libraries(wamrlib)

project(hello_world)
