/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_pthread.h"

#include "esp_log.h"
#include "esp_system.h"

#include "wasm_export.h"

extern const uint8_t test_bin_start[] asm("_binary_test_wasm_start");
extern const uint8_t test_bin_end[]   asm("_binary_test_wasm_end");

void app_main(void)
{
    RuntimeInitArgs init_args;

    // TRY TO INCLUDE WAMR
    memset(&init_args, 0, sizeof(RuntimeInitArgs));

    init_args.mem_alloc_type = Alloc_With_Allocator;
    init_args.mem_alloc_option.allocator.malloc_func = malloc;
    init_args.mem_alloc_option.allocator.realloc_func = realloc;
    init_args.mem_alloc_option.allocator.free_func = free;
    if (!wasm_runtime_full_init(&init_args))
    {
        printf("WASM runtime init failed!");
    }
    else
    {
        const int test_bin_size = test_bin_end - test_bin_start - 1;
        uint8_t *load_buffer;
        char error_buf[128];
         wasm_module_t wasm_module;
        wasm_module_inst_t wasm_module_inst;

        load_buffer = malloc(test_bin_size);
        memcpy(load_buffer, test_bin_start, test_bin_size);

        printf("WASM runtime initialized");
    /* load WASM module */
        if (!(wasm_module = wasm_runtime_load(load_buffer, test_bin_size,
                                            error_buf, sizeof(error_buf))))
        {
            printf("Module loading failed!\n");
        }
        else
        {
            printf("Module loaded correctly\n");
            /* instantiate the module */
            if (!(wasm_module_inst = wasm_runtime_instantiate(
                    wasm_module, CONFIG_WAMR_APP_STACK_SIZE, CONFIG_WAMR_APP_HEAP_SIZE,
                    error_buf, sizeof(error_buf)))) {
                printf("WASM module instantiation failed! %s\n", error_buf);
            }
            else
            {
                const char *exception;
                int app_argc = 0;
                char **app_argv = {NULL, };

                printf("WASM runtime instantiate module succeded\n");

                /* invoke the main function */
                wasm_application_execute_main(wasm_module_inst, app_argc, app_argv);
                if ((exception = wasm_runtime_get_exception(wasm_module_inst))) {
                    printf("Exception during execution of WASM Module!\n");
                }

                printf("WASM runtime execute app's main function succeded\n");

                /* destroy the module instance */
                wasm_runtime_deinstantiate(wasm_module_inst);
            }
        }
    }

    // standard hello world application here...
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%uMB %s flash\n", (unsigned int)(flash_size / (1024 * 1024)),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", (int)esp_get_minimum_free_heap_size());

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);


    esp_restart();
}
