//=====[Libraries]=============================================================

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "dht.h"
#include "api_dht11.h"

//=====[Declaration of private data types]=====================================

typedef struct dht11
{
    float temp;
    float hum;
    gpio_num_t pin;
} dht11_t;

//=====[Declaration and initialization of private global constants]============

static const uint8_t AVG_SAMPLES = 10;

static const char *TAG = "API DHT11";

//=====[Declaration and initialization of private global variables]============

static dht11_t dht11;

static SemaphoreHandle_t xMutex = NULL;

//=====[Implementations of public functions]===================================

/**
 * @brief Inicializa el DHT11.
 *
 * @param pin gpio donde esta conectado el DHT11.
 */
void init_dht11(gpio_num_t pin)
{
    xMutex = xSemaphoreCreateMutex();
    configASSERT(xMutex != NULL);
    dht11.temp = 0.0;
    dht11.hum = 0.0;
    dht11.pin = pin;
}

/**
 * @brief Tarea que se encarga de leer periodicamente el DHT11.
 *
 * @param pvParameters no se utiliza.
 */
void v_task_dht11(void *pvParameters)
{
    float temp = 0.0;
    float hum = 0.0;
    uint8_t i = 0;
    float buf_temp = 0.0;
    float buf_hum = 0.0;
    for (;;)
    {
        if (dht_read_float_data(DHT_TYPE_DHT11, dht11.pin, &hum, &temp) != ESP_OK)
        {
            ESP_LOGE(TAG, "Could not read data from sensor");
        }
        else
        {
            if (i < AVG_SAMPLES)
            {
                i++;
                buf_temp += temp;
                buf_hum += hum;
            }
            else
            {
                if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
                {
                    dht11.temp = buf_temp / AVG_SAMPLES;
                    dht11.hum = buf_hum / AVG_SAMPLES;
                    xSemaphoreGive(xMutex);
                }
                i = 0;
                buf_temp = 0.0;
                buf_hum = 0.0;
                ESP_LOGI(TAG, "Temp: %.1f°C Hum: %.1f%%", dht11.temp, dht11.hum);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2 * 1000));
    }
}

/**
 * @brief Nos devuelve los valores de temperatura y humedad.
 *
 * @param temp valor de temperatura
 * @param hum valor de humedad
 */
void read_dht11(float *temp, float *hum)
{
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
        *temp = dht11.temp;
        *hum = dht11.hum;
        xSemaphoreGive(xMutex);
    }
}