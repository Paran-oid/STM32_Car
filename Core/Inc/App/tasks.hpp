#pragma once

extern "C"
{
#include "main.h"
}

/***********************************************************
 * Public GPIO enums for specifying instruction type
 ***********************************************************/
enum SensorRequestCode : uint8_t
{
    IR_SIGNAL = 0,
    BUZZER_ON
};

template <typename T>
struct SensorRequest
{
    SensorRequestCode code;
    T                 content;
} __attribute__((packed));

/***************************************************************
 * Function declarations
 ***************************************************************/
extern "C"
{
    void main_task(void* argument);
    void IR_read_task(void* argument);
    void car_move_task(void* argument);
    void HCSR04_read_task(void* argument);
    void controller_task(void* argument);
}
