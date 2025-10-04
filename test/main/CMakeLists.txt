idf_component_register(
    SRCS "hello_world_main.c"
    REQUIRES nvs_flash esp_netif freertos driver log bt esp_wifi spi_flash
)
