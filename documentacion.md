# Bienvenidos

## Introduccion

El presente documento o **CheatSheet** sirve de guia para las funciones o variables mas utilizadas a la hora de programar con el framework **ESP-IDF**.

El microcontrolador utilizado es el **ESP32**, la aclaracion es debido a que para otras placas se utilizan otras funciones.

En el siguiente link se encuentra el [Repositorio de GitHub](https://github.com/tvecchiUTN/ESP-IDF-Examples/tree/main) en donde se guardaron los ejemplos. Ahi se puede ver las variables necesitadas y sus funciones.

## Indice

### Parte 1: Funciones basicas

- [GPIO](#gpio-general-purpose-input-output)
- [Analog Input o ADC (Analog Digital Converter)](#adc-analog-digital-converter)
- [PWM (Pulse Width Modulation)](#pwm-pulse-widht-modulation)
- DAC (Digital to Analog Converter)
- Sensor Touch (Pines tactiles)
- UART echo (Universal Asynchronous Receiver-Transmitter)
- NVS Storage (Non-Volatile Storage)
- Timers (GPTimer)
- Power Modes

### Parte 2: Aprendiendo el freeRTOS

- Creacion de tareas
- Pasar argumentos a las tareas
- Prioridad de tareas
- Modo Dual Core
- Colas (Productor-Consumidor)
- Semaforos o MUTEX (Evitar Race Conditions)
- Grupos de eventos
- Notificacion de tareas

### Parte 3: Conectividad y Drivers

- Estacion WiFi (Conectarse a un Router, manejo de eventos IP)
- SoftAP WiFi (Crear propia red de WiFi)
- Cliente HTTP
- Escaner I2C
- Driver I2C customizable
- Maestro SPI
- RMT infrarojo

## Desarrollo de temas

### GPIO (General Purpose Input Output)

Los dos ejemplos claros que se vieron en este tema es: encender un led (Blinker) y recibir una señal digital (pulsador)

#### Libreria: `#include "driver/gpio.h"`

#### Estructura para configuracion: `gpio_config_t`

| Variable | Descripcion | Tipo de variable |
| ----------- | ----------- | ----------- |
| pin_bit_mask | Mascara del pin a setear (1ULL[^1] << Pin Deseado)[^2] | uint64_t |
| mode | modo del GPIO | gpio_mode_t |
| pull_up_en | Habilita la resistencia Pull-Up | gpio_pullup_t |
| pull_down_en | Habilita la resistencia Pull-Down | gpio_pulldown_t |
| intr_type | El tipo de interrupcion | gpio_int_type_t |

[^1]: 1ULL significa un 1 en Unsigned Long Long, muy utilizado en aritmetica a nivel de bits.

[^2]: Si se quiere añadir un pin mas, se utiliza el signo OR a nivel de bits (" | "), ejemplo: (1ULL << PinDeseado0) | (1ULL << PinDeseado1)

---> ==Caracteristicas==

1) Los modos que pueden tener el GPIO pueden ser, OUTPUT (Leds, actuadores, etc), INPUT (Pulsadores, sensores, etc), INPUT_OUTPUT (Habilita ambos para, por ejemplo, saber el estado del led), y los por ultimo, los modos open-drain (Utilizados para protocolos de comunicacion, por ejemplo, I2C).
2) Los valores que se le pueden dar a pullUp y pullDown pueden ser entre 0 o 1, no es necesario utilizar la macro.
3) Para los actuadores, el tipo de interrupcion suele ser DISABLE, pero suele ser muy utilizado por pulsadores, o entradas, para que interrumpa una tarea cuando se activan. Su uso se ve en el siguiente [ejemplo](https://github.com/tvecchiUTN/ESP-IDF-Examples/blob/066bb0f6af8ad119528b7f6d8cbe68b96804833e/Iniciales/2.1-SigInpInter/main/main.c). Los valores de interrupt son demasiadas pero dependen pricipalmente de como quiero que se comporte la entrada; cuando la señal decae, es positiva, es negativa, etc.

#### Funciones de configuracion: `esp_err_t gpio_config(const gpio_config_t *pGPIOConfig)`

---> **Parametros**

- pGPIOConfig: Puntero a la estructura **gpio_config_t**.

---> **Retorna**

- ESP_OK si no hay errores.
- ESP_ERR_INVALID_ARGS si hay errores de argumentos.

#### Funcion de seteo de salida: `esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level)`

---> **Parametros**

- gpio_num: Numero de GPIO. Si es GPIO16 por ejemplo, el parametro debe ser GPIO_NUM_16 (16) o bien puede ser el numero.

- level: Nivel de salida. 0 para LOW; 1 para HIGH

---> **Retorna**

- ESP_OK si no hay errores.
- ESP_ERR_INVALID_ARGS si hay errores de argumentos.

#### Funcion entrada de señal: `int gpio_get_level(gpio_num_t gpio_num)`

> AVISO: Si no fue configurado como entrada, el valor de retorno siempre es 0

---> **Parametros**

- gpio_num: Numero de GPIO. Si es GPIO16 por ejemplo, el parametro debe ser GPIO_NUM_16 (16) o bien puede ser el numero.

---> **Retorna**

- 0 si el nivel de la entrada es 0
- 1 si el nivel de la entrada es 1

### ADC (Analog Digital Converter)

#### Libreria: `#include "esp_adc/adc_oneshot.h"`

#### Estructura para configuracion inicial: `adc_oneshot_unit_init_cfg_t`

| Variable | Descripcion | Tipo de variable |
| ----------- | ----------- | ----------- |
| unit_id | Unidad del ADC | adc_unit_t |
| clk_src | Origen del reloj | adc_oneshot_clk_src_t |
| ulp_mode | ADC controlado por ULP[^3] | adc_ulp_mode_t |

[^3]: ULP (Ultra Low Power) procesador pequeño y dedicado que funciona con un consumo minimo de energia

---> ==Caracteristicas==

1) La unidad del adc suele ser 1 (`ADC_UNIT_1`) o 2 (`ADC_UNIT_2`)
2) Selecciona el orien del reloj, comunmente se utiliza 0 para que utilize el default o por defecto
3) Selecciona si el adc va a funcionar cuando este en modo ULP. Comunmente se utiliza `ADC_ULP_MODE_DISABLE`

#### Estructura para configuracion de canal: `adc_oneshot_chan_cfg_t`

| Variable | Descripcion | Tipo de variable |
| ----------- | ----------- | ----------- |
| atten | Atenuacion del ADC | adc_atten_t |
| bitwidth | Ancho de bits | adc_bitwidth_t |

---> ==Caracteristicas==

1) Diferentes parametros determinan el rango del adc. Los valores posibles son, 0, 2.5, 6 y 12. Un ejemplo es `ADC_ATTEN_DB_12`
2) El ancho de bits del resultado de la conversión sin procesar. Un ejemplo es `ADC_BITWIDTH_DEFAULT`

#### Handle para ADC en modo oneshot: `adc_oneshot_unit_handle_t`

Con este handler manejo al dispositivo. Si quiero manejar 3 ADC, utilizo 3 handlers.

#### Funcion para configuracion inicial: `esp_err_t adc_oneshot_new_unit(const adc_oneshot_unit_init_cfg_t *init_config, adc_oneshot_unit_handle_t *ret_unit)`

---> **Parametros**

- init_config: Puntero a la estructura de configuracion inicial.
- ret_unit: Puntero al Handler de la unidad.

--> **Retorna**

- ESP_OK: si no hay errores.
- ESP_ERR_INVALID_ARG: Argumentos invalidos
- ESP_ERR_NO_MEM: Memoria insuficiente
- ESP_ERR_NOT_FOUND: Periferico ADC en uso
- ESP_FAIL: El reloj no se inicializo correctamente

#### Funcion para configuracion de canal: `esp_err_t adc_oneshot_config_channel(adc_oneshot_unit_handle_t handle, adc_channel_t channel, const adc_oneshot_chan_cfg_t *config)`

> AVISO: No utilizarla en un contexto ISR. La funcion es thread-safe.

---> **Parametros**

- handle: Handler de la unidad
- channel: Canal ADC a configurar, su macro es `ADC_CHANNEL_0` y va desde 0 a 10
- config: Puntero a la estructura de configuracion de canal

---> **Retorna**

- ESP_OK: si no hay errores
- ESP_ERR_INVALID_ARG: Argumentos invalidos

#### Funcion para leer el valor: `esp_err_t adc_oneshot_read(adc_oneshot_unit_handle_t handle, adc_channel_t chan, int *out_raw)`

---> **Parametros**

- handle: Handler de la unidad.
- chan: Canal ADC
- out_raw: Puntero a la variable donde se guarda el valor leido

---> **Retorna**

- ESP_OK: si no hay errores
- ESP_ERR_INVALID_ARG: Argumentos invalidos
- ESP_ERR_TIMEOUT: Timeout, el resultado del adc es invalido

#### Funcion para eliminar la unidad: `esp_err_t adc_oneshot_del_unit(adc_oneshot_unit_handle_t handle)`

---> **Parametros**

- handle: Handler de la unidad.

---> **Retorna**

- ESP_OK: si no hay errores
- ESP_ERR_INVALID_ARG: Argumentos invalidos
- ESP_ERR_NOT_FOUND: El periferico no se utiliza.

---

==Funcion complementaria==

#### Funcion para saber el canal a partir del numero de GPIO: `esp_err_t adc_oneshot_io_to_channel(int io_num, adc_unit_t *const unit_id, adc_channel_t *const channel)`

---> **Parametros**

- io_num: Numero de GPIO
- unit_id: Unidad ADC
- channel: Canal ADC

---> **Retorna**

- ESP_OK: si no hay errores
- ESP_ERR_INVALID_ARG: Argumentos invalidos
- ESP_ERR_NOT_FOUND: El pin no es un pad ADC valido

### PWM (Pulse Widht Modulation)
