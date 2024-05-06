# Ejemplo 5: Static web app

## Introduccion

En el ejemplo, empezaremos a crear un servidor _HTTP Restful_ y utilizaremos un navegador como cliente para poder ver una _web app_ estatica.

## Web app estatica

Un servidor, que aloja una una _web app_ estatica, esta preparado para que:

1. El navegador haga un _request_ con el metodo _GET_ del protocolo _HTTP_ sobre uno de sus recursos, que se identifican por una _url_.
2. Que el navegador procese el _request_ y devuelva un _response_ que va a contener un archivo.
3. El navegador reciba el _response_ y procese en archivo en el navegador.

Dentro del _response_, tambien viene un codigo de _status_ que nos dice como se proceso el _request_. Donde las codigos mas comunes son:

- _200 OK_.
- _400 Bad Request_.
- _401 Unauthorized_.
- _404 Not Found_.
- _408 Request Timeout_.
- _500 Internal Server Error_.

Si no se recibio _200 OK_, el navegador no contara con el archivo para ejecutar el paso 3.

## Inspector de la pagina web

1. Clic derecho sobre el contenido de la pagina.
2. Clic en _Inspeccionar_.
3. Ir a la seccion de _Red_.
4. Refrescar la pagina.

Ahora vamos a poder ver el proceso de armado de la _web app_:

1. Hay un _request_ para pedir el archivo _index.html_ en el recurso _/_ del servidor que se hace automaticamente al introducir la _url_ en el navegador.
2. Llega el _response_ con el archivo _index.html_.
3. Cuando se lo procesa, se generan mas _request_ al servidor en el orden en que aparecen en el codigo _html_:

   - Uno para el archivo _materialize.min.css_ en el recurso _/materialize.min.css_.
   - Uno para el archivo _materialize.min.js_ en el recurso _/materialize.min.js_.
   - Uno para el archivo _main.js_ en el recurso _/main.js_.

4. Llegan las _response_ de forma asincronica, es decir no en el orden en que se pidieron.
5. Se termina de armar la _web app_.

Asi, es como interactuan el servidor y un cliente, que en este caso es un navegador, cuando se tiene una _web app_ estatica.

![navegador](/ej5-static-web-app/navegador.png)

![servidor](/ej5-static-web-app/servidor.png)

## Material complementario

https://github.com/espressif/esp-idf/tree/v5.2.1/examples/protocols/http_server/restful_server

https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html

https://developer.mozilla.org/en-US/docs/Learn/Common_questions/Web_mechanics/What_is_a_web_server

https://developer.mozilla.org/es/docs/Web/HTTP/Overview

https://developer.mozilla.org/es/docs/Web/HTTP

https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/GET
