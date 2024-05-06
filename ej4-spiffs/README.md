# Ejemplo 4: SPIFFS

## Pasos adicionales al inicializar el proyecto

1. Abrir el _ESP-IDF: SDK Configuration Editor (menuconfig)_.
2. Ir a _Partition Table._
3. Seleccionar _Custom partition table CSV_.
4. Crear un archivo llamado _partitions.csv_ en el directorio raiz del proyecto.
5. Copiar al archivo _partitions.csv_ el siguiente contenido:

```
# ESP-IDF Partition Table
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x6000,
phy_init, data, phy,     0xf000,  0x1000,
factory,  app,  factory, 0x10000, 1M,
storage,  data, spiffs,  ,        0xF0000,
```

La nueva tabla de pariciones de la memoria _flash_, es identica a _Single factory app, no OTA_, pero agrega el sector _storage_ que es donde va a estar montado el _SPIFFS_.

## Introduccion

El _SPIFFS_ es un _file system_, que esta pensado para alamacenar archivos en la memoria _flash_ que esta conectada mediante el bus _SPI_ al _ESP32_.

Al mismo, solo se le pueden realizar operaciones de lectura desde nuestro programa. Por ejemplo, para leer los archivos que componen una _web app_ estatica.

Por otro lado, no admite el uso de directorios. Por lo que todos los archivos, van a quedar montados sobre una misma ruta.

## Subir los archivos al SPIFFS de forma automatica al grabar el programa

Para el ejemplo, tenemos una _web app_ estatica dentro de la carpeta llamada _www_.

1. Abrir el archivo _CMakeLists.txt_ que esta dentro de la carpeta _main_.
2. Agregar al final del mismo: _spiffs_create_partition_image(storage ../www FLASH_IN_PROJECT)_.
3. Ejecutar _ESP-IDF Build, Flash and Monitor_.

## Material complementario

https://github.com/espressif/esp-idf/tree/v5.2.1/examples/storage/spiffs

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html

https://www.w3schools.com/html/default.asp

https://www.w3schools.com/css/default.asp

https://materializecss.com/

https://www.w3schools.com/js/default.asp
