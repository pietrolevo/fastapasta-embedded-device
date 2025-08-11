idf_component_register(
  SRCS
    "app_main.c"
    "state_machine.c"
  INCLUDE_DIRS # Header files included above
    "."
    "../include"
  PRIV_REQUIRES # Private dependencies
    mqtt
    nvs_flash
    esp_netif
)
