# Parte 2: Aprendiendo el freeRTOS

- [Gestion de tareas](#creacion-de-tareas-task-create)
- [Colas o Queues (Productor-Consumidor)](#colas-o-queue)
- Semaforos y MUTEX
- Notificaciones directas (Task notifications)
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