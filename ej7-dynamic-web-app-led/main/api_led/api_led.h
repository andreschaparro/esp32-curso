//=====[Libraries]=============================================================

#ifndef _API_LED_H_
#define _API_LED_H_

#include "driver/gpio.h"

//=====[Declarations (prototypes) of public functions]=========================

void init_led(gpio_num_t pin);

void v_task_led(void *pvParameters);

void read_led_status(int *status);

void write_led_command(int *command);

//=====[#include guards - end]=================================================

#endif // _API_LED_H_