/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <esp_err.h>

/* Enable ESP Mesh-Lite in the application
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_mesh_lite_start(void);

esp_err_t esp_mesh_lite_root_send(int node_id, bool value);