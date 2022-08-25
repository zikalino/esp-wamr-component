set(srcs "hello_world_main.c")

if(CONFIG_WAMR_APP_TEST_BIN_WASM)
set(test_bin "test.wasm")
else()
set(test_bin "test.aot")
endif()

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "."
                       EMBED_TXTFILES "${test_bin}")
