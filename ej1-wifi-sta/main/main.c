//=====[Libraries]=============================================================

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "api_wifi.h"

//=====[Declaration and initialization of private global constants]============

static const char *SSID = "MiSSID";
static const char *SSID_PASS = "MiPasswd";

//=====[Main function, the program entry point after power on or reset]========

void app_main(void)
{
    // Inicializacion del Non-Volatile-Storage
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    wifi_init_sta(SSID, SSID_PASS); // Conexion a la red Wi-Fi

    vTaskDelete(NULL); // Elimino la tarea IDLE
}