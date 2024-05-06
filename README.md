# Curso de Programacion del ESP32 utilizando el ESP-IDF para desarrollar un dispostivo IoT

## Introduccion

Este repositorio, contiene los diferentes ejemplos que conforman el curso.

El objetivo del mismo, es hacer un dispositivo IoT que puede ser un nodo sensor y/o actuador. Al que desarrollaremos forma progresiva, conforme avancemos con los ejemplos. En otras palabras, es un curso completamente practico.

Utilizaremos el _SDK_ oficial de _Espressif_ para programar el _SOC ESP32_ que es el _ESP-IDF_.

El que desee utilizar el _SDK ESP-Arduino_, les aclaro que el mismo es un _wrapper_ del anterior. Por lo que, perderemos funcionalidades y parte del control de lo que ocurre en nuestro _SOC_. Asi que es momento de tomar un decision...

![pastilla](/pastilla.png)

Si eligieron la pastilla roja, en las siguientes paginas del fabricante tendran acceso a la documentacion, ejemplos, y modulos que no vienen con el _SDK_ por defecto:

https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html

https://github.com/espressif/esp-idf/tree/master/examples

https://components.espressif.com/

Lo vuelvo a aclarar, no es un curso donde se explica linealemnte:

- La aquitectura del _SOC_.
- Lenguaje _C_.
- Cada _API_ del _SDK_.
- Protocolos de comunicacion para sistemas embebidos.
- _FreeRTOS_.
- Desarrollo _web_.

Se daran los conceptos mas importante y material adicional para que el alumno pueda consultar y profundizar su conocimiento.

**ADVERTENCIA: Si su idea es clonar el repositorio y probar los ejemplos no va a aprender nada. Va a tener que invertir horas culo como decimos los programadores...**

Ahora, y sin mas preambulos ¡Empecemos!

Ing. Andres Chaparro

## Software

A continuacion, seguir la guia del fabricante que esta en la siguiente pagina:

https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md

En el paso 6 seleccionar _Global_.

En el paso 8 seleccionar _Express_.

En el paso 10:

- Elegir la ultima version estable.
- _IDF_PATH_ debe valer _C:\Users\usuario\esp_.
- _IDF_TOOLS_PATH_ debe valer _C:\Users\usuario\.expressif_.

Una vez finalizada la instalacion, en la siguiente pagina puede verse como usar la extension de _VCS_:

https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/basic_use.md

Para nuestros ejemplos, siempre partiremos del _sample_project_.

## Hardware

El kit de desarrollo es el siguiente:

![SOC](/soc.png)
