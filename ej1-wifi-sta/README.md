# Ejemplo 1: Wi-Fi station

## Inicializar el proyecto

1. Abrir el _ESP-IDF: SDK Configuration Editor (menuconfig)_.
2. Ir a _Serial flasher config_.
3. En _Flash size_ seleccionar _4MB_.
4. Clic en _Save_.
5. Ejecutar _ESP-IDF: Select Flash Method_.
6. Elegir _UART_.
7. Ejecutar _ESP-IDF: Build Project_.

## Introduccion

El modo _station_ o _sta_, se utiliza cuando queremos conectar el _ESP32_ a una red _Wi-Fi_.

## Crear un modulo para la conexion Wi-Fi en modo sta

1. Dentro del directorio _main_ crear una carpeta llamada _api_wifi_.
2. Dentro de _api_wifi_ crear un archivo llamada _api_wifi.h_.
3. Dentro de _api_wifi_ crear un archivo llamada _api_wifi.c_.

## Incluir el modulo al proceso de compilacion

1. Abrir el archivo _CMakeLists.txt_ que esta dentro de la carpeta _main_.
2. Dentro de _SRCS_ agregar _api_wifi/api_wifi.c_.
3. Dentro de _INCLUDE_DIRS_ agregar _api_wifi_.

## Material complementario

https://github.com/espressif/esp-idf/tree/v5.2.1/examples/wifi/getting_started/station

https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_wifi.html

https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/wifi.html

https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/storage/nvs_flash.html

https://www.freertos.org/FreeRTOS-Event-Groups.html
