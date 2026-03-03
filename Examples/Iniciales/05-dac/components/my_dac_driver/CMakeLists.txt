idf_component_register(
    SRCS "my_dac_driver.c"
    INCLUDE_DIRS "include" 
    REQUIRES esp_driver_dac
)