#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * Public Defines
 ***************************************************************/
constexpr uint8_t BUZZER_START_SIGNAL_AMOUNT = 3;
constexpr uint8_t BUZZER_START_SIGNAL_DELAY = 100;

/***************************************************************
 * Function declarations
 ***************************************************************/
extern "C"
{
    void main_task(void* argument);
    void IR_read_task(void* argument);
    void HCSR04_read_task(void* argument);
    void controller_task(void* argument);
}
