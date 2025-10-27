#pragma once

extern "C"
{
#include "cmsis_os2.h"
}

#include <string.h>

#include "error_handler.hpp"
/***************************************************************
 * Public defines
 ***************************************************************/
constexpr uint8_t buzzer_toggle_callback_DELAY = 100;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern osMemoryPoolId_t   MemPoolHandle;
extern osMemoryPoolAttr_t MemPool_attributes;

extern osMessageQueueId_t         sensorQueueHandle;
extern const osMessageQueueAttr_t sensorQueue_attributes;

extern osTimerId_t         StopMotorTimerHandle;
extern const osTimerAttr_t StopMotorTimer_attributes;

extern osTimerId_t         BuzzerToggleTimerHandle;
extern const osTimerAttr_t BuzzerToggleTimer_attributes;

extern osMutexId_t         HTIM3MutexHandle;
extern const osMutexAttr_t htim3Mutex_attributes;

extern osSemaphoreId_t         IRSemHandle;
extern const osSemaphoreAttr_t IRSem_attributes;

/***************************************************************
 * Exported objects
 ***************************************************************/
volatile extern bool is_cmd_sent;
volatile extern bool is_warning;

/***********************************************************
 * Public GPIO enums for specifying instruction type
 ***********************************************************/
enum SensorRequestCode : uint8_t
{
    IR_SIGNAL = 0,
    DISTANCE_ALERT
};

/***********************************************************
 * Sensor request struct for data commnunication between tasks
 ***********************************************************/
template <typename T>
struct SensorRequest
{
    SensorRequestCode code;
    T                 content;
};

/***************************************************************
 * Function declarations
 ***************************************************************/
extern "C"
{
    void stop_motor_callback(void* argument);
    void buzzer_toggle_callback(void* argument);
}

void rtos_init_all();

template <typename T>
bool rtos_queue_send(const T& data, osMessageQueueId_t queue)
{
    T* item = (T*) osMemoryPoolAlloc(MemPoolHandle, osWaitForever);
    if (!item) error_handle(critical_error);

    memcpy(item, &data, sizeof(data));
    return osMessageQueuePut(sensorQueueHandle, &item, 0, osWaitForever) == osOK;
}
