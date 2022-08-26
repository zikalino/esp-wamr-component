set(WAMR_ROOT_DIR "${COMPONENT_PATH}/wasm-micro-runtime")

if(CONFIG_WAMR_IMPORT_ESP_LIBC_WASI)
set(wasm_inc "${WAMR_ROOT_DIR}/core/iwasm/include"
             "${WAMR_ROOT_DIR}/core/iwasm/libraries/libc-wasi/"
             "${WAMR_ROOT_DIR}/core/iwasm/common")


set(srcs "port/esp_libc_wasi_wrapper.c")
else()
set(wasm_inc "${WAMR_ROOT_DIR}/core/iwasm/include")

set(srcs "")
endif()

set(incs ${wasm_inc} "include")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${incs}
                       REQUIRES esp_timer)

# WAMR Configuration

set(WAMR_BUILD_PLATFORM "esp-idf")

set (WAMR_APP_THREAD_STACK_SIZE_MAX ${CONFIG_WAMR_APP_THREAD_STACK_SIZE_MAX})

if(CONFIG_WAMR_ENABLE_INTERP)
    set(WAMR_BUILD_INTERP 1)
else()
    set(WAMR_BUILD_INTERP 0)
endif()

if(CONFIG_WAMR_INTERP_FAST)
    set(WAMR_BUILD_FAST_INTERP 1)
else()
    set(WAMR_BUILD_FAST_INTERP 0)
endif()

if(CONFIG_WAMR_INTERP_LOADER_MINI)
    set(WAMR_BUILD_MINI_LOADER 1)
else()
    set(WAMR_BUILD_MINI_LOADER 0)
endif()

if(CONFIG_WAMR_ENABLE_AOT)
    set(WAMR_BUILD_AOT 1)
else()
    set(WAMR_BUILD_AOT 0)
endif()

if(CONFIG_WAMR_ENABLE_LIBC_BUILTIN)
    set(WAMR_BUILD_LIBC_BUILTIN 1)
else()
    set(WAMR_BUILD_LIBC_BUILTIN 0)
endif()

if(CONFIG_IDF_TARGET_ARCH_XTENSA)
    set (WAMR_BUILD_TARGET "XTENSA")
else()
    if(CONFIG_IDF_TARGET_ARCH_RISCV)
        set (WAMR_BUILD_TARGET "RISCV")
    endif()
endif()

if(CONFIG_WAMR_BUILD_DEBUG)
    set (CMAKE_BUILD_TYPE Debug)
else()
    set (CMAKE_BUILD_TYPE Release)
endif()

if(CONFIG_WAMR_ENABLE_MULTI_MODULE)
    set (WAMR_BUILD_MULTI_MODULE 1)
else()
    set (WAMR_BUILD_MULTI_MODULE 0)
endif()

if(CONFIG_WASM_ENABLE_SHARED_MEMORY)
    set (WASM_BUILD_SHARED_MEMORY 1)
else()
    set (WASM_BUILD_SHARED_MEMORY 0)
endif()

if(CONFIG_WAMR_ENABLE_MEMORY_PROFILING)
    set (WAMR_BUILD_MEMORY_PROFILING 1)
else()
    set (WAMR_BUILD_MEMORY_PROFILING 0)
endif()

if(CONFIG_WAMR_ENABLE_PERF_PROFILING)
    set (WAMR_BUILD_PERF_PROFILING 1)
else()
    set (WAMR_BUILD_PERF_PROFILING 0)
endif()

if(CONFIG_WAMR_ENABLE_REF_TYPES)
    set (WAMR_BUILD_REF_TYPES 1)
else()
    set (WAMR_BUILD_REF_TYPES 0)
endif()

# WAMR cmake scripts

add_definitions(-DWASM_ALIGN_SIZE=4)

include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)
add_library(wamrlib ${WAMR_RUNTIME_LIB_SOURCE})

# Append WAMR library to component

target_link_libraries(${COMPONENT_LIB} INTERFACE wamrlib)
