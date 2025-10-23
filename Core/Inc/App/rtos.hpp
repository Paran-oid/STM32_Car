#pragma once

extern "C"
{
#include "cmsis_os2.h"
}

#include <stdio.h>
#include <string.h>
/***************************************************************
 * Public defines
 ***************************************************************/
constexpr uint8_t STOP_WARNING_TIMER_DELAY = 100;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern osMemoryPoolId_t   MemPoolHandle;
extern osMemoryPoolAttr_t MemPool_attributes;

extern osMessageQueueId_t         sensorQueueHandle;
extern const osMessageQueueAttr_t sensorQueue_attributes;

extern osTimerId_t         StopMotorTimerHandle;
extern const osTimerAttr_t StopMotorTimer_attributes;

extern osTimerId_t         StopWarningTimerHandle;
extern const osTimerAttr_t StopWarningTimer_attributes;

extern osMutexId_t         HTIM3MutexHandle;
extern const osMutexAttr_t htim3Mutex_attributes;

/***************************************************************
 * Exported objects
 ***************************************************************/
volatile extern bool is_cmd_sent;
volatile extern bool is_warning;

/***************************************************************
 * Function declarations
 ***************************************************************/

extern "C"
{
    void stop_motor_callback(void* argument);
    void stop_warning_timer(void* argument);
}

void rtos_init_all();

template <typename T>
bool rtos_queue_send(T data, osMessageQueueId_t queue)
{
    T* item = (T*) osMemoryPoolAlloc(MemPoolHandle, osWaitForever);
    if (!item)
    {
        printf("couldn't allocate item in memory pool to be sent to the queue...");
        while (1);
    }
    memcpy(item, &data, sizeof(data));
    return osMessageQueuePut(sensorQueueHandle, &item, 0, osWaitForever) == osOK;
}
