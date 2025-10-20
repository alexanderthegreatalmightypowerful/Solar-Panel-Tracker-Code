idf_component_register(SRCS "main.c" 
INCLUDE_DIRS "."
REQUIRES openthread
PRIV_REQUIRES esp_http_server esp_timer esp_wifi nvs_flash pca9685)
