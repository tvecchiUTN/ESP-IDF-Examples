# Guía de Desarrollo Profesional para ESP32 (ESP-IDF)

Bienvenido a esta guía de referencia rápida (*CheatSheet*) y mejores prácticas para el desarrollo de firmware embebido utilizando el framework oficial de Espressif (**ESP-IDF**).

Este documento está diseñado para ingenieros y desarrolladores que buscan migrar de Arduino a un entorno profesional o necesitan recordar rápidamente la sintaxis y funciones clave del ESP32.

> **Repositorio de Ejemplos:** [GitHub - ESP-IDF-Examples](https://github.com/tvecchiUTN/ESP-IDF-Examples/tree/main)

---

## ⚡ Antes de empezar: El Hardware (ESP32)

El ESP32 es mucho más potente que un Arduino estándar, pero tiene reglas estrictas:

* **Voltaje Lógico:** **3.3V**. ¡Cuidado! Conectar sensores de 5V directamente a los pines quemará el chip.
* **Núcleos:** Cuenta con 2 núcleos (Core 0 y Core 1).
    * *Core 0:* Se encarga del WiFi y Bluetooth (Radio).
    * *Core 1:* Ejecuta tu código (`app_main`).
* **Corriente:** Los pines GPIO pueden entregar máximo **12mA** (suficiente para un LED, no para motores).

---

## 🛠️ Filosofía ESP-IDF vs Arduino

En este framework, el control es tuyo. Ten en cuenta estas diferencias vitales:

1.  **No existe el `loop()` infinito:**
    * Tu código corre en una tarea de **FreeRTOS**.
    * **Regla de Oro:** Nunca hagas bucles vacíos (`while(1){}`). Siempre debes ceder el control con `vTaskDelay` para evitar que el *Watchdog Timer* reinicie el chip por sospecha de bloqueo.

2.  **Manejo de Errores (Obligatorio):**
    * Casi todas las funciones retornan `esp_err_t` (un código de error).
    * **Usa siempre:** `ESP_ERROR_CHECK(funcion())`. Si la función falla, el ESP32 se reiniciará y te imprimirá el error exacto en la consola. ¡No ignores los errores!

3.  **Logs > Serial.print:**
    * Usamos la librería de logging (`ESP_LOGI`, `ESP_LOGE`) que permite colorear, etiquetar y desactivar mensajes por niveles globalmente.

---

## 📚 Índice de Contenidos

La documentación está dividida en tres módulos según tu necesidad:

### 🟢 [Parte 1: Funciones Básicas y Periféricos](./Documentacion/Parte1_Basicos.md)
*Dominando el Hardware.*
* **Controlar pines:** [GPIO (Salida/Entrada)](./Parte1_Basicos.md#gpio-general-purpose-input-output)
* **Leer sensores:** [ADC (Analógico)](./Parte1_Basicos.md#adc-analog-digital-converter)
* **Mover motores/LEDs:** [PWM](./Parte1_Basicos.md#pwm-pulse-widht-modulation) y [DAC](./Parte1_Basicos.md#dac-digital-to-analog-converter)
* **Guardar datos (Memoria no volátil):** [NVS](./Parte1_Basicos.md#nvs-non-volatile-storage)
* **Comunicación Serial:** [UART](./Parte1_Basicos.md#uart-universal-asynchronous-receiver-transmitter)
* **Ahorro de energía:** [Power Modes](./Parte1_Basicos.md#power-modes) y [Timers](./Parte1_Basicos.md#gptimer-general-purpose-timer)

### 🟡 [Parte 2: Sistema Operativo (FreeRTOS)](./Documentacion/Parte2_FreeRTOS.md)
*Dominando la Multitarea.*
* Creación de Tareas (Multithreading).
* Sincronización (Semáforos, Mutex).
* Comunicación entre tareas (Colas/Queues).
* Manejo de eventos.

### 🔵 [Parte 3: Conectividad y Protocolos](./Documentacion/Parte3_Conectividad.md)
*Dominando el IoT.*
* **WiFi:** Modo Estación (Cliente) y SoftAP (Router).
* **Internet:** Cliente HTTP.
* **Buses:** I2C, SPI.
* **Control:** RMT (Infrarrojos/LEDs Addressable).

---

## 💡 Recomendaciones Finales

* **Documentación Oficial:** Esta guía es un resumen. Para detalles profundos, siempre consulta la [API Reference de Espressif](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/index.html).
* **Monitor:** Mantén siempre abierto el monitor serie (`idf.py monitor`) para ver los logs y trazas de error (Backtrace) cuando ocurra un *Crash*.

## PinOut del ESP32

![PinOut de la placa ESP32](./imagenes/PinOut-ESP32.jpg)