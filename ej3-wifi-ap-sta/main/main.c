//=====[Libraries]=============================================================

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "api_wifi.h"

//=====[Declaration and initialization of private global constants]============

static const char *SSID = "MiSSID";
static const char *SSID_PASS = "MiPasswd";
static const char *DEVICE = "DAIoTxx";
static const char *PASSWD_AP = "12345678";

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

    wifi_init_softap_sta(WIFI_MODE_APSTA, SSID, SSID_PASS, DEVICE, PASSWD_AP); // Coneccion a la red Wi-Fi y creacion de la red del ESP32

    vTaskDelete(NULL); // Elimino la tarea IDLE
}