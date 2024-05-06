# Ejemplo 7: Dynamic web app para actuar sobre un led

## Introduccion

En el ejemplo, haremos que el servidor _HTTP Restful_ adquiera la capacidad de procesar comandos que vienen desde la _web app_ para encender o apagar un led.

Para ello, la _web app_ tendra un elemento del tipo _switch_ que al presionarlo, generara un _request_ con el metodo _POST_ del protocolo _HTTP_ sobre el recurso del servidor que procesara el comando. Y, el mismo terminara recibiendo un objeto _jSON_ del navegador con los datos del comando a procesar.

Se volvera a usar _AJAX_ para enviar los comandos. El codigo, esta en _main.js_.

![jsON](/ej7-dynamic-web-app-led/json.png)

## FreeRTOS

El _ESP32_ tiene 2 _CPUs_ el _PRO_CPU_ y el _APP_CPU_, y hasta el momento todos nuestros ejemplos solo utilizaban el _PRO_CPU_. Pero a partir de este ejemplo, empezaremos a utilizar los 2 _CPUs_.

En el _APP_CPU_:

- Leeremos el sensor _DHT11_ periodicamente dentro un tarea.
- Debera actualizar el estado led cuando llegue un comando dentro de una tarea.

En el _PRO_CPU_:

- Haremos que el _ESP32_ funcione en modo _sta_ y _Soft-AP_.
- Crearemos el servidor _HTTP Restful_.

Debido a que la memoria es compartida entre los 2 _CPUs_, se debe emplear un _Mutex_ para evitar que el servidor intente obtener los valores de temperatura y humedad, mientras los mismos pueden estar siendo modificados.

Debido a que el servidor _HTTP Restful_ es un productor de comandos y la tarea que maneja el led un consumidor, necesitaremos una _Queue_.

Y de forma similar a como pasa con el _DHT11_, se necesitara un _Mutex_ para evitar que el servidor intente obtener el estado del led mientras puede estar siendo modificado.

## Material complementario:

https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST

https://www.freertos.org/Embedded-RTOS-Queues.html
