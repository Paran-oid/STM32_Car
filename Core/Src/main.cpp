
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

/**
 * TODO LIST
 *
 * ! Fourth part (0.5):
 * * - Introduce the feature of distance warning
 * * (The more I approach my hand/an object, the more beeps it outputs)
 * * - Add distance safety mechanism (if too close to an object output faster beeps and stop car)
 * * - Make sure mutex ensures htim isn't modified or smth during critical section by differnet
 * * thread for htim3
 *
 * ! Fourth part (1):
 * * - Reintroduce speed timer (pwm)
 * * - Introduce error codes
 * * - Read value of battery
 * * - blink a led if battery too low (make too leds for both power sources)
 * * - Add bluetooth
 *
 * ! Last part:
 * * - Make sure we save as much battery as possible (through sleep modes)
 * * - Enhance documentation and verify that every module is explained throughthly
 * * - Write with the use of AI a good README.md and add a video of the project
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
        loop();
    }
}
