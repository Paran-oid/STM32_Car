#pragma once

extern "C"
{
#include "cmsis_os2.h"
}

//* NOTE: many of these are already initialized in main.c

extern osMemoryPoolId_t   MemPoolHandle;
extern osMemoryPoolAttr_t MemPool_attributes;

extern osMessageQueueId_t         carInstructionsHandle;
extern const osMessageQueueAttr_t carInstructions_attributes;

extern osTimerId_t         stopMotorTimerHandle;
extern const osTimerAttr_t stopMotorTimer_attributes;

extern osMutexId_t         ptimerMutexHandle;
extern const osMutexAttr_t ptimerMutex_attributes;

extern bool cmd_sent;

void stop_motor_callback(void* args);

void rtos_init_all();
