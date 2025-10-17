
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

/**
 * TODO LIST
 *
 * ! Fourth part (0.5):
 * * - Read value of battery
 * * - Output battery percentage (through either an LCD or another display)
 * * - blink a led if battery too low
 *
 * ! Fourth part (1):
 * * - Write driver for buzzer
 * * - Introduce the feature of distance warning
 * * (The more I approach my hand/an object, the more beeps it outputs)
 *
 * ! Fourth part (2):
 * * - Write driver for Ultrasonic sensor
 * * - Test written driver
 * * - Add distance safety mechanism (if too close to an object output faster beeps)
 * ! Fifth part:
 * * - Add speed adjustment
 * * - Add bluetooth
 * * - Test logic and make sure 100% of the code works
 *
 *
 * ! Last part:
 * * - Make sure we save as much battery as possible (through sleep modes)
 * * - Introduce documentation (on the whole project)
 * * - Write with the use of AI a good README.md and add a video of the project
 *
 *
 *
 * ?--> Once done we will start on other tasks (dive deeper in bluetooth)
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
    // Mapping main.c to main.cpp functions

    void main_setup(void)
    {
        setup();
    }

    void main_loop(void)
    {
        loop();
    }

    void main_task_exec(void)
    {
        main_task();
    }

    void IR_read_task_exec(void)
    {
        IR_read_task();
    }

    void car_move_task_exec(void)
    {
        car_move_task();
    }

    void stop_motor_callback_exec(void)
    {
        stop_motor_callback(NULL);
    }
}
