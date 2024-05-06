# Ejemplo 6: Dynamic web app para ver los valores de temperatura y humedad leidos por un sensor DHT11

## Introduccion

En el ejemplo, haremos que el servidor _HTTP Restful_ adquiera la capacidad de entregarnos los valores temperatura y humedad leidos por un sensor _DHT11_.

Asi, el navegador podra actualizar esos valores en la _web app_ de forma automatica y cada 5 segundos.

Lo dicho se hara sin necesidad de recargar la _web app_, por lo que la misma dejara de ser estatica y pasara a ser dinamica.

## Objetos jSON

El servidor _HTTP Restful_, devolvera un objeto del tipo _jSON_ cada vez que el navegador solicite actualizar los valores de temperatura y humedad.

El formato de serializacion _jSON_ tiene la siguiente sintaxis:

```
{
    "field1": "text",
    "field2": 42,
    "field3": true,
    "field4": [],
}
```

## Modulos de terceros

El sensor _DHT11_, tiene un protocolo de comunicacion especial de 1 hilo y no debe ser leido con un intervalo de tiempo menor a los 2 segundos.

El siguiente repositorio, tiene una gran cantidad de modulos desarrollados para el _ESP32_:

https://github.com/UncleRus/esp-idf-lib/tree/master

Hay uno llamado _dht_ que podemos utilizar para leer el sensor _DHT11_.

1. Abrir una terminal de _Git bash_ dentro del directorio del proyecto.
2. Ejecutar: _git clone https://github.com/UncleRus/esp-idf-lib.git_.
3. Verificar que aparezca dentro del proyecto la siguiente ruta: esp-idf-lib/components/dht.
4. Abrir el CMakeLists.txt que esta dentro del directorio del proyecto. No el de la carpeta main.
5. Antes de la linea _include($ENV{IDF_PATH}/tools/cmake/project.cmake)_, agregar _set(EXTRA_COMPONENT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/esp-idf-lib/components)_.
6. Volver a compilar el proyecto.

## FreeRTOS

El _ESP32_ tiene 2 _CPUs_ el _PRO_CPU_ y el _APP_CPU_, y hasta el momento todos nuestros ejemplos solo utilizaban el _PRO_CPU_. Pero a partir de este ejemplo, empezaremos a utilizar los 2 _CPUs_.

En el _APP_CPU_:

- Leeremos el sensor _DHT11_ periodicamente dentro un tarea.

En el _PRO_CPU_:

- Haremos que el _ESP32_ funcione en modo _sta_ y _Soft-AP_.
- Crearemos el servidor _HTTP Restful_.

Debido a que la memoria es compartida entre los 2 _CPUs_, se debe emplear un _Mutex_ para evitar que el servidor intente obtener los valores de temperatura y humedad, mientras los mismos pueden estar siendo modificados.

## AJAX

Es el mecanismo que nos permitira:

- Leer los valores de temperatura y humedad del servidor. Para lo que, el navegador hara un _request_ con el metodo _GET_ del protocolo _HTTP_ al recurso _/api/data_ cada 5 segundos.
- Actualizar el contenido de la _web app_, sin necesidad de recargarla, con el contenido del objeto _jSON_ que viene en cada _response_.

El codigo, esta en _main.js_.

![jsON](/ej6-dynamic-web-app-dht11/json.png)

## Material complementario:

https://developer.mozilla.org/es/docs/Learn/JavaScript/Objects/JSON

http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html.

https://github.com/UncleRus/esp-idf-lib

https://docs.espressif.com/projects/esp-idf/en/v4.3/esp32/api-reference/system/freertos.html

https://www.freertos.org/taskandcr.html

https://www.freertos.org/Real-time-embedded-RTOS-mutexes.html

https://www.w3schools.com/js/js_ajax_intro.asp
