# Parte 3: Conectividad y Drivers

- [Estacion WiFi (Conectarse a un Router, manejo de eventos IP)](#estacion-wifi)
- SoftAP WiFi (Crear propia red de WiFi)
- Cliente HTTP
- Escaner I2C
- Driver I2C customizable
- Maestro SPI
- RMT infrarojo

## Desarrollo de temas

---

### Estacion WiFi

#### Librerias: `#include "esp_wifi.h"`

#### Estructura para la configuracion inicial del wifi

`wifi_init_config_t`

> El mismo contiene demasiados parametros, por lo tanto se utiliza la siguiente macro para inicializar los valores: `WIFI_INIT_CONFIG_DEFAULT`. Si se desea cambiar miembros, sobreescribalos despues de colocarla

#### Funcion para inicializar el wifi

`esp_err_t esp_wifi_init(const wifi_init_config_t *config)`

---> **Parametros**

- config: Puntero a la estructura con la configuracion del wifi

---> **Retorna**

- ESP_OK: Si salio todo bien
- ESP_ERR_NO_MEM: Sin memoria insuficiente
- otro: error referido en: `esp_err.h`

#### Funcion para setear el modo de operacion

`esp_err_t esp_wifi_set_mode(wifi_mode_t mode)`

---> **Parametros**

- mode: Modo de operacion, revisar el tipo `wifi_mode_t`. En este seccion usaremos  `WIFI_MODE_STA`

---> **Retorna**

- ESP_OK: Si salio todo bien
- ESP_ERR_WIFI_NOT_INIT: Wifi no inicializado
- ESP_ERR_INVALID_ARG: error de argumentos
- others: error referido en: `esp_err.h`

#### Funcion para iniciar el wifi

`esp_err_t esp_wifi_start(void)`

> Iniciar WiFi según la configuración actual. Si el modo es `WIFI_MODE_STA`, crea un bloque de control de estación e inicia la estación

---> **Retorna**

- ESP_OK: Si salio todo bien
- ESP_ERR_WIFI_NOT_INIT: Wifi no inicializado
- ESP_ERR_INVALID_ARG: No suele ocurrir. La función llamada dentro de la API recibió un argumento no válido. El usuario debe verificar si la configuración del WiFi es correcta.
- ESP_ERR_NO_MEM: Memoria insuficiente
- ESP_ERR_WIFI_CONN: Error interno del WiFi; el bloque de control de la estación o del punto de acceso suave es incorrecto.
- ESP_FAIL: Otros errores internos del WiFi

#### Estructura de escaneo de SSID

`wifi_scan_config_t`

| Variable | Descripcion | Tipo de variable |
| -------- | ----------- | ---------------- |
| ssid | Nombre del wifi | uint8_t* |
| bssid | Direccion MAC del wifi | uint8_t* |
| channel | Escanea un canal especifico | uint8_t |
| show_hidden | Habilita si escanea SSID que estan ocultos | bool |
| scan_type | Tipo de escaneo, ya sea pasivo o activo | wifi_scan_type_t |
| scan_time | Estructura con la informacion del tiempo de escaneo del canal | wifi_scan_time_t |
| - | - | - |
| - | - | - |
| - | - | - |
