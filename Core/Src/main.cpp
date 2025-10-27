
#include "gpio.hpp"
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

/**
 * TODO LIST:
 * ! Last part:
 * * - Enhance documentation and verify that every module is explained throughthly
 * * - Write with the use of AI a good README.md and add a video of the project
 * * - Finish with the project :)
 *
 * § features to implement in the future :
 * * - Introduce bluetooth
 * * - Introduce WIFI
 * * - Control car through one of them
 * * - Create a logger
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
