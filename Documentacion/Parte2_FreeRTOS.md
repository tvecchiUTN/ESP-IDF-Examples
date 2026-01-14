# Parte 2: Aprendiendo el freeRTOS

- [Gestion de tareas](#creacion-de-tareas-task-create)
- [Colas o Queues (Productor-Consumidor)](#colas-o-queue)
- [Semaforos y MUTEX](#semaforos-y-mutex)
- [Notificaciones directas (Task notifications)](#notificaciones-directas-task-notifications)
- Grupos de eventos

## Desarrollo de temas

---

### Creacion de tareas (Task create)

#### Librerias: `#include "freertos/FreeRTOS.h"` y `#include "freertos/task.h"`

#### Funcion para crear una tarea

`static inline BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, const char *const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void *const pvParameters, UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask)`

> Esta funcion utiliza memoria dinamica para crearse. Su funcion contraparte utiliza no usa memoria dinamica, `xTaskCreateStatic`

---> **Parametros**

- pxTaskCode: Puntero a la funcion a ejecutar. Recordar que esta funcion no debe terminar nunca y tampoco debe retornar. Su prototipo es `void func(void* args)`
- pcName: Puntero al string con el nombre. Esta debe ser menor a 16 caracteres o 16 bytes
- usStackDepth: Tamaño del stack, un tamaño de 2048 para tareas simples esta bien. Usar 4096 si la funcion usa `printf` o logs
- pvParameters: Parametros que se le pasan a la funcion. En la practica, para pasarle muchos parametros a una funcion, es mejor encapsularla en una estructura. Para que esa estructura no se pierda, se debe crearla con memoria dinamica usando `malloc()`
- uxPriority: Nivel de prioridad siende 0 la prioridad mas baja y 24 la mas alta
- pxCreatedTask: Puntero al handle de la tarea

---> **Retorna**

`pdPASS` si la tarea se creo correctamente o un error definido en el archivo `projdefs.h`

#### Funcion para crear una tarea en un nucleo especifico

`BaseType_t xTaskCreatePinnedToCore(TaskFunction_t pxTaskCode, const char *const pcName, const uint32_t ulStackDepth, void *const pvParameters, UBaseType_t uxPriority, TaskHandle_t *const pxCreatedTask, const BaseType_t xCoreID)`

---> **Parametros**

- Los mismos que la funcion `xTaskCreate()`
- xCoreID: El nucleo en la cual la tarea es fijada. Comunemente para Wifi/BT usar el pin 0. Si quiero que el sistema decida el pin, usar la macro `tskNO_AFFINITY`

#### Funcion para eliminar una tarea

`void vTaskDelete(TaskHandle_t xTaskToDelete)`

---> **Parametros**

- xTaskToDelete: Manejador de la tarea a eliminar. Si la funcion se aplica sobre la tarea misma, usar `NULL`

#### Funcion para aplicar un delay a la tarea

`void vTaskDelay(const TickType_t xTicksToDelay)`

---> **Parametros**

- xTicksToDelay: Tiempo, en ticks, en la que la tarea se bloquea. La macro `pdMS_TO_TICKS(ms)` convierte milisegundos a ticks del sistema

---

### Colas o queue

#### Libreria: `#include "freertos/queue.h"`

#### Macro para crear la cola

`xQueueCreate(uxQueueLength, uxItemSize)`

---> **Parametros**

- uxQueueLenght: Tamaño de la cola o cantidad de elementos
- uxItemSize: Tamaño, en bytes, de cada elemento o item

---> **Retorna**

- El handle o manejador de la cola
- Si no se pudo crear, retorna 0

#### Macro para añadir un elemento al fondo

`xQueueSend(xQueue, pvItemToQueue, xTicksToWait)`

> Su macro contrario es `xQueueSendToFront` que añade el item al inicio

---> **Parametros**

- xQueue: Manejador de la cola
- pvItemToQueue: Puntero al item a guardar
- xTicksToWait: Tiempo, en ticks, que debe esperar la tarea para añadir un elemento en caso de que la cola este llena

---> **Retorna**

- pdTRUE: En caso de que el item se pudo añadir
- errQUEUE_FULL: Otro error

#### Funcion para obtener un item de la cola

`BaseType_t xQueueReceive(QueueHandle_t xQueue, void *const pvBuffer, TickType_t xTicksToWait)`

---> **Parametros**

- xQueue: Manejador de la cola
- pvBuffer: Puntero al buffer donde se guarda el elemento
- xTicksToWait: Tiempo, en ticks, que debe esperar la tarea para retirar un elemento en caso de que la cola este vacia

---> **Retorna**

- pdTRUE: Si el item se pudo retirar de la cola
- pdFALSE: Si hubo un error

---

#### Uso en Interrupciones (ISR)

Las funciones xQueueSendFromISR y xQueueReceiveFromISR tienen un parámetro extra vital: `BaseType_t *pxHigherPriorityTaskWoken`

**¿Qué es?**: Un puntero a una variable booleana.

**¿Para qué sirve?**: La función pondrá esta variable en pdTRUE si la operación despertó a una tarea de mayor prioridad que la que se interrumpió.

**¿Qué debo hacer?**: Si la variable termina en pdTRUE, debes llamar a `portYIELD_FROM_ISR()` al final de la interrupción para forzar el cambio de contexto inmediato.

---

### Semaforos y MUTEX

#### Libreria: `#include "freertos/semphr.h"`

#### Macro para crear el semaforo binario

`xSemaphoreCreateBinary()`

---> **Retorna**

- Manejador del semaforo binario, debe ser del tipo `SemaphoreHandle_t`, este inicia vacio por lo que le tenemos que dar un primer valor para que arranque disponible

#### Macro para crear el MUTEX

`xSemaphoreCreateMutex()`

---> **Retorna**

- Manejador del mutex, debe ser del tipo `SemaphoreHandle_t`, este inicia disponible

#### Macro para bloquear el semaforo

`xSemaphoreTake(xSemaphore, xBlockTime)`

> El termino bloquear o "Lock" en ingles viene de que en C, el mutex se bloqua y se desbloquea. Capaz con este termino se entiende mejor el uso

---> **Parametros**

- xSemaphore: Manejador del semaforo
- xBlockTime: Tiempo, en ticks, que debe esperar hasta que el semaforo este disponible

---> **Retorna**

- pdTRUE: Si el semaforo fue obtenido
- pdFALSE: Si el tiempo expiro sin que el semaforo este disponible

#### Macro para liberar el semaforo

`xSemaphoreGive(xSemaphore)`

---> **Parametros**

- xSemaphore: Manejador del semaforo

---> **Retorna**

- pdTRUE: Si el semaforo fue liberado
- pdFalse: Si ocurrio un error

---

| Caracteristica | Semaforo binario | Mutex |
| -------------- | ---------------- | ----- |
| **Uso Principal** | **Sincronización:** Una tarea (o ISR) avisa a otra que algo pasó. | **Protección (Exclusión Mutua):** Proteger una variable o bus para que solo uno la use a la vez. |
| **Dueño (Ownership)** | **NO.** Cualquiera puede dar o tomar. | **SÍ.** Solo quien lo toma puede liberarlo. |
| **Herencia de Prioridad** | **NO.** Sufre de "Inversión de Prioridad". | **SÍ.** Evita bloqueos si tareas de distinta prioridad compiten. |
| **Uso en ISR** | **Permitido** (`GiveFromISR`). | **PROHIBIDO.** No se puede usar Mutex dentro de una interrupción. |

---

### Notificaciones directas (Task notifications)

#### Libreria: Misma que semaforo

#### Macro para dar la notificacion

`xTaskNotifyGiveIndexed(xTaskToNotify, uxIndexToNotify)`

---> **Parametros**

- xTaskToNotify: Manejador de la tarea a notificar. Es el mismo del que retorna `xTaskCreate()`
- uxIndexToNotify: El indice en la que el array de notificacion de la tarea destino se va a enviar

---> **Retorna**

- Siempre retorna pdPASS

> Razón técnica: Esta función simplemente incrementa un contador dentro de la tarea destino. A diferencia de una cola, no puede "llenarse" ni fallar por falta de memoria, así que siempre tiene éxito.
