
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

extern void stopMotorCallback(void* args);

//
// Main thread
//
void setup()
{
    /**
     * TODO LIST
     * ! Third part:
     * * - Improve code (less code in main.cpp and organize other files)
     * * - Write logic for motors.c (all possible movements)
     * * - Think about a way to make the car work without usb connection
     * * - Test car with new added logic
     * * - Use RTOS techniques (threads, mutexes, queues) to communicate between IR and driver
     * * system
     *
     * ! Fourth part:
     * * - Add bluetooth
     * * - Test logic and make sure 100% of the code works
     * * - Control the car using infra-red signals (for now)
     *
     * ?--> Once done we will start on other tasks (dive deeper in bluetooth)
     */
    hal_init_all();
    rtos_init_all();
}

// should never be reached
void loop()
{
}

extern "C"
{
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
