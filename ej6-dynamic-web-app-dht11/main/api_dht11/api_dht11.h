//=====[Libraries]=============================================================

#ifndef _API_DHT11_H_
#define _API_DHT11_H_

//=====[Libraries]=============================================================

#include "driver/gpio.h"

//=====[Declarations (prototypes) of public functions]=========================

void init_dht11(gpio_num_t pin);

void v_task_dht11(void *pvParameters);

void read_dht11(float *temp, float *hum);

//=====[#include guards - end]=================================================

#endif // _API_DHT11_H_