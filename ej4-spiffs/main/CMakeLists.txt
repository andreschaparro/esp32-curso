idf_component_register(
    SRCS 
        "main.c"
        "api_wifi/api_wifi.c"
        "api_spiffs/api_spiffs.c"
    INCLUDE_DIRS 
        "."
        "api_wifi"
        "api_spiffs"
)

spiffs_create_partition_image(storage ../www FLASH_IN_PROJECT)