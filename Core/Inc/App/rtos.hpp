#pragma once

extern "C"
{
#include "cmsis_os2.h"
}

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

extern osMutexId_t         ptimerMutexHandle;
extern const osMutexAttr_t ptimerMutex_attributes;

/***************************************************************
 * Exported objects
 ***************************************************************/
volatile extern bool is_cmd_sent;
volatile extern bool is_warning;

/***************************************************************
 * Function declarations
 ***************************************************************/
void rtos_init_all();

extern "C"
{
    void stop_motor_callback(void* argument);
    void stop_warning_timer(void* argument);
}
