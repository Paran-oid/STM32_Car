
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

/**
 * TODO LIST
 *
 * ! Fourth part: *
 * * - Do all your TODO tasks
 * * - Read value of battery
 * * - blink a led if battery too low (make too leds for both power sources)
 * * - Make sure we save as much battery as possible (through sleep modes)
 *
 * ! Fifth part:
 * * - Clean code and optimize it significantly
 * * - Add bluetooth
 *
 * ! Last part:
 * * - Introduce error handling (along with error codes)
 * * - Enhance documentation and verify that every module is explained throughthly
 * * - Once again optimize and clean the project
 * * - Write with the use of AI a good README.md and add a video of the project
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
        loop();
    }
}
