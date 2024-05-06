//=====[Libraries]=============================================================

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "api_led.h"

//=====[Declaration of private data types]=====================================

typedef struct led
{
    int status;
    gpio_num_t pin;
} led_t;

//=====[Declaration and initialization of private global constants]============

static const char *TAG = "API LED";

//=====[Declaration and initialization of private global variables]============

static led_t led;

static QueueHandle_t xQueue;

static SemaphoreHandle_t xMutex = NULL;

//=====[Implementations of public functions]===================================

/**
 * @brief Inicializa el led.
 *
 * @param pin gpio donde esta conectado el led.
 */
void init_led(gpio_num_t pin)
{
    xMutex = xSemaphoreCreateMutex();
    configASSERT(xMutex != NULL);
    xQueue = xQueueCreate(10, sizeof(int));
    configASSERT(xQueue != NULL);
    led.pin = pin;
    led.status = 0;
    gpio_set_direction(led.pin, GPIO_MODE_OUTPUT);
    gpio_set_level(led.pin, led.status);
}

/**
 * @brief Tarea que se encarga de actualizar el led cuando llegue un comando.
 *
 * @param pvParameters no se utiliza.
 */
void v_task_led(void *pvParameters)
{
    int command = 0;
    for (;;)
    {
        if (xQueueReceive(xQueue, &command, portMAX_DELAY) == pdTRUE)
        {
            if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
            {
                if (command == 1 && led.status == 0)
                {
                    led.status = 1;
                    gpio_set_level(led.pin, led.status);
                }
                else if (command == 0 && led.status == 1)
                {
                    led.status = 0;
                    gpio_set_level(led.pin, led.status);
                }
                xSemaphoreGive(xMutex);
                ESP_LOGI(TAG, "Led status: %d", led.status);
            }
        }
    }
}

/**
 * @brief Nos devuelve el estado del led.
 *
 * @param status 0 apagado o 1 encendido.
 */
void read_led_status(int *status)
{
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
        *status = led.status;
        xSemaphoreGive(xMutex);
    }
}

/**
 * @brief Nos permite enviar un commando al led.
 *
 * @param command 0 apagar o 1 encender.
 */
void write_led_command(int *command)
{
    if (xQueueSend(xQueue, (void *)command, portMAX_DELAY) != pdTRUE)
    {
        ESP_LOGE(TAG, "Could not write data to queue");
    };
}