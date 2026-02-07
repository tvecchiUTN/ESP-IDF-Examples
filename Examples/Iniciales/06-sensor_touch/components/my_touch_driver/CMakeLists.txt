idf_component_register(
    SRCS "my_touch_driver.c"
    INCLUDE_DIRS "include" 
    REQUIRES driver esp_driver_touch_sens freertos esp_common
)