idf_component_register(
    SRCS 
        "main.c"
        "api_wifi/api_wifi.c"
        "api_rest/api_rest.c"
        "api_dht11/api_dht11.c"
        "api_led/api_led.c"
    INCLUDE_DIRS 
        "."
        "api_wifi"
        "api_rest"
        "api_dht11"
        "api_led"
)

spiffs_create_partition_image(storage ../www FLASH_IN_PROJECT)