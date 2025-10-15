extern "C"
{
#include "main.h"

#include "cmsis_os2.h"
}

#include <stdio.h>
#include <string.h>

#include "drive_sys.hpp"
#include "gpio.hpp"
#include "irremote.hpp"
#include "ptimer.hpp"

// TODO: make main.cpp have less code

extern osMemoryPoolId_t   mem_pool;
extern osMemoryPoolAttr_t mem_pool_attributes;

extern osMessageQueueId_t carInstructionsHandle;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart2;

PTimer<uint32_t> tim2 {htim2};  // used for precise delays
PTimer<uint16_t> tim3 {htim3};  // pwm-related

GPIO     IRcontrol_gpio = {IRcontrol_GPIO_Port, IRcontrol_Pin};
IRRemote remote         = {IRcontrol_gpio, tim2};

GPIO motor1[] = {{MOTOR1_1_GPIO_Port, MOTOR1_1_Pin}, {MOTOR1_2_GPIO_Port, MOTOR1_2_Pin}};
GPIO motor2[] = {{MOTOR2_1_GPIO_Port, MOTOR2_1_Pin}, {MOTOR2_2_GPIO_Port, MOTOR2_2_Pin}};

DriveSystem drive_sys = {tim3, motor1[0], motor1[1], motor2[0], motor2[1]};

//
// Main thread
//
void setup()
{
    /**
     * TODO LIST
     * ! Third part:
     * * - finish beta car testing part
     * * - Write logic for motors.c (all possible movements)
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

    mem_pool = osMemoryPoolNew(10, sizeof(IRRemoteEntry), &mem_pool_attributes);
    tim2.start();

    tim3.pwm_start(TIM_CHANNEL_2);
    tim3.pwm_start(TIM_CHANNEL_3);
    IRcontrol_gpio.set_mode(INPUT);
}

// should never be reached
void loop()
{
}

//
// Tasks
//
void main_task()
{
    while (1)
    {
        // background task
        osDelay(1);
    }

    osThreadTerminate(NULL);
}

void IR_read_task()
{
    while (1)
    {
        IRRemoteEntry entry = remote.receive();
        if (entry.state)
        {
            IRRemoteEntry* item = (IRRemoteEntry*) osMemoryPoolAlloc(mem_pool, osWaitForever);
            if (!item)
            {
                printf("couldn't send item");
                Error_Handler();
            }
            memcpy(item, &entry, sizeof(entry));

            osMessageQueuePut(carInstructionsHandle, item, 0, osWaitForever);
        }
        osDelay(1);
    }
}

void car_move_task()
{
    while (1)
    {
        IRRemoteEntry* entry;
        if (osMessageQueueGet(carInstructionsHandle, &entry, NULL, osWaitForever) == osOK)
        {
            // !fix this crap and then continue with the rest
            printf("data: 0x%x\r\n", entry->data);
            printf("addr: 0x%x\r\n", entry->addr);
            printf("\r\n");

            osMemoryPoolFree(mem_pool, entry);
        }

        // if (entry.state)
        // {
        //     if (entry.data == IR_REMOTE_2)
        //     {
        //         drive_sys.move(CAR_FORWARD);
        //         move = CAR_FORWARD;
        //     }
        //     else if (entry.data == IR_REMOTE_8)
        //     {
        //         drive_sys.move(CAR_BACKWARD);
        //         move = CAR_BACKWARD;
        //     }
        //     else if (entry.data == IR_REMOTE_MUTE)
        //     {
        //         drive_sys.stop();
        //         move = CAR_STATIONARY;
        //     }
        //     else if (IR_REPEAT_CHECK(entry))
        //     {
        //         drive_sys.move(previous_car_direction);
        //     }

        //     previous_car_direction = move;
        // }

        osDelay(1);
    }
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
}
