# Bienvenidos

## Introduccion

El presente documento o **CheatSheet** sirve de guia para las funciones o variables mas utilizadas a la hora de programar con el framework **ESP-IDF**.

El microcontrolador utilizado es el **ESP32**, esto es debido a que para otras placas se utilizan otras funciones.

En el siguiente link se encuentra el [Repositorio de GitHub](https://github.com/tvecchiUTN/ESP-IDF-Examples/tree/main) en donde se guardaron los ejemplos. Ahi se puede ver las variables necesitadas y sus funciones.

## Indice

### Parte 1: Funciones basicas

- GPIO
- Analog Input o ADC (Analog Digital Converter)
- PWM (Pulse Width Modulation)
- UART echo (Universal Asynchronous Receiver-Transmitter)
- DAC (Digital to Analog Converter)

### Parte 2: Aprendiendo el freeRTOS

- Task Create

## Desarrollo de temas

### GPIO (General Purpose Input Output)

Los dos ejemplos claros que se vieron en este tema es: encender un led (Blinker) y recibir una señal digital (pulsador)

*Libreria:* `#include "driver/gpio.h"`

*Estructura para configuracion:* `gpio_config_t`

| Variable | Descripcion | Tipo de variable |
| ----------- | ----------- | ----------- |
| pin_bit_mask | Mascara del pin a setear (1ULL[^1] << Pin Deseado)[^2] | uint64_t |
| mode | modo del GPIO | gpio_mode_t |
| pull_up_en | Habilita la resistencia Pull-Up | gpio_pullup_t |
| pull_down_en | Habilita la resistencia Pull-Down | gpio_pulldown_t |
| intr_type | El tipo de interrupcion | gpio_int_type_t |

Observaciones:

- Los modos que pueden tener el GPIO pueden ser, OUTPUT (Leds, actuadores, etc), INPUT (Pulsadores, sensores, etc), INPUT_OUTPUT (Habilita ambos para, por ejemplo, saber el estado del led), y los por ultimo, los modos open-drain (Utilizados para protocolos de comunicacion, por ejemplo, I2C).
- Los valores que se le pueden dar a pullUp y pullDw pueden ser entre 0 o 1, no es necesario utilizar la macro.
- Para los actuadores, el tipo de interrupcion suele ser DISABLE, pero suele ser muy utilizado por pulsadores para que interumpa una tarea cuando se presiona el pulsador. Su uso se ve en el siguiente [ejemplo](https://github.com/tvecchiUTN/ESP-IDF-Examples/blob/066bb0f6af8ad119528b7f6d8cbe68b96804833e/Iniciales/2.1-SigInpInter/main/main.c). Los valores de interrupt son demasiadas pero dependen pricipalmente de como quiero que se comporte la entrada; cuando la señal decae, el positiva, es negativa, etc.



[^1]: 1ULL significa un 1 en Unsigned Long Long, muy utilizado en aritmetica a nivel de bits.

[^2]: Si se quiere añadir un pin mas, se utiliza el signo OR a nivel de bits (" | "), ejemplo: (1ULL << PinDeseado0) | (1ULL << PinDeseado1)