//=====[Libraries]=============================================================

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "driver/gpio.h"
#include "api_wifi.h"
#include "api_rest.h"
#include "api_dht11.h"
#include "api_led.h"

//=====[Declaration and initialization of private global constants]============

static const char *SSID = "FV-IOT";
static const char *SSID_PASS = "FVVR#iot1980";
static const char *DEVICE = "DAIoTxx";
static const char *PASSWD_AP = "12345678";

static const gpio_num_t DHT11_PIN = GPIO_NUM_33;
static const gpio_num_t LED_PIN = GPIO_NUM_2;

//=====[Main function, the program entry point after power on or reset]========

void app_main(void)
{
    init_dht11(DHT11_PIN); // Inicializacion del DHT11
    init_led(LED_PIN);     // Inicializacion led led

    // Inicializacion del Non-Volatile-Storage
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    wifi_init_softap_sta(WIFI_MODE_APSTA, SSID, SSID_PASS, DEVICE, PASSWD_AP); // Conexion a la red Wi-Fi y creacion de la red del ESP32

    ESP_ERROR_CHECK(init_fs()); // Inicializacion del SPIFFS

    ESP_ERROR_CHECK(start_rest_server()); // Inicializacion del servidor HTTP Restful

    BaseType_t ret;
    ret = xTaskCreatePinnedToCore( // Creo la tarea que va a leer periodicamente el DHT11
        v_task_dht11,
        "Tarea del DHT11",
        (2 * configMINIMAL_STACK_SIZE),
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);

    ret = xTaskCreatePinnedToCore( // Creo la tarea que va a actualizar el led cuando llegue un comando
        v_task_led,
        "Tarea del led",
        (2 * configMINIMAL_STACK_SIZE),
        NULL,
        (tskIDLE_PRIORITY + 1U),
        NULL,
        APP_CPU_NUM);
    configASSERT(ret == pdPASS);

    vTaskDelete(NULL); // Elimino la tarea IDLE
}
