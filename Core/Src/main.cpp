
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

/**
 * TODO LIST:
 *
 * ! Before last part:
 * * - Add photoresistor (read using ADC)
 * * - if it's too dark light lighting leds (infront of car)
 *
 * ! Last part:
 * * - Introduce error handling (along with error codes)
 * * - Enhance documentation and verify that every module is explained throughthly
 * * - Once again optimize and clean the project (verify that power is used efficently too)
 * * - Use RAII to stop peripherals when needed
 * * - Disable USART and other peripherals not in use
 * * - Write with the use of AI a good README.md and add a video of the project
 * * - Switch from debug to release
 *
 * 
 *
 * ?--> Once done we will start to work on other tasks (dive deeper in bluetooth)
 */

void setup()
{
    hal_init_all();
    rtos_init_all();
}

void loop()
{
    // should never be reached
}

extern "C"
{
    // Mapping main.c to main.cpp functions/tasks

    void main_setup(void)
    {
        setup();
    }

    void main_loop(void)
    {
        loop();  // should never be reached
    }
}
