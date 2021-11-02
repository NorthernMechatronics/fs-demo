/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2020, Northern Mechatronics, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <am_bsp.h>
#include <am_mcu_apollo.h>
#include <am_util.h>

#include <FreeRTOS.h>
#include <queue.h>

#include "lfs.h"
#include "lfs_ambiq.h"

#include "application.h"

TaskHandle_t application_task_handle;

#define CACHE_SIZE      16
#define LOOKAHEAD_SIZE  16

static lfs_t lfs;
static uint8_t lfs_read_buffer[CACHE_SIZE];
static uint8_t lfs_prog_buffer[CACHE_SIZE];
static uint8_t lfs_lookahead_buffer[LOOKAHEAD_SIZE];

const struct lfs_config cfg = {
    .context = (void *)64,                // starting page number
    .read = littlefs_ambiq_read,
    .prog = littlefs_ambiq_prog,
    .erase = littlefs_ambiq_erase,
    .sync = littlefs_ambiq_sync,
    .read_size = 4,               // minimum number of bytes per read
    .prog_size = 4,               // minimum number of bytes per write
    .block_size = 8192,           // size per page
    .block_count = 4,             // number of pages used for the file system
    .cache_size = CACHE_SIZE,
    .lookahead_size = LOOKAHEAD_SIZE,
    .block_cycles = 500,
    .read_buffer = lfs_read_buffer,
    .prog_buffer = lfs_prog_buffer,
    .lookahead_buffer = lfs_lookahead_buffer,
};

void fs_init()
{
    int err = lfs_mount(&lfs, &cfg);
    if (err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }
}

void fs_deinit()
{
    lfs_unmount(&lfs);
}

uint32_t fs_boot_count()
{
    uint32_t boot_count = 0;
    lfs_file_t file;

    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

    boot_count += 1;

    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));
    lfs_file_close(&lfs, &file);

    return boot_count;
}

void application_task(void *pvParameters)
{
    uint32_t boot_count = 0;

    fs_init();
    boot_count = fs_boot_count();
    fs_deinit();

    am_util_stdio_printf("Little FS Demo\r\n");
    am_util_stdio_printf("Boot Count: %d\r\n", boot_count);


    while (1) {
        am_hal_gpio_state_write(AM_BSP_GPIO_LED0, AM_HAL_GPIO_OUTPUT_TOGGLE);
        vTaskDelay(500);
    }
}
