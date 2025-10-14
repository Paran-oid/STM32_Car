extern "C"
{
#include "main.h"

#include "cmsis_os2.h"
}

#include <stdio.h>

#include "drive_sys.hpp"
#include "gpio.hpp"
#include "irremote.hpp"
#include "ptimer.hpp"

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
     * ! Second part:
     * * - make sure when I hold a button my remote it keeps moving
     * * - Do a beta test for car (move just forward and backwards)
     *
     * ! Third part:
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
void default_task()
{
    while (1)
    {
        IRRemoteEntry entry = remote.receive();
        if (entry.state)
        {
            printf("data: 0x%x\r\n", entry.data);
            printf("addr: 0x%x\r\n", entry.addr);
            printf("\r\n");

            if (entry.data == IR_REMOTE_2)
            {
                // drive_sys.move(CAR_FORWARD);
                while ((entry.state))
                {
                    entry = remote.receive();
                    if (entry.data != IR_REMOTE_REPEAT_CODE || entry.addr != IR_REMOTE_REPEAT_CODE)
                    {
                        break;
                    }
                    printf("held front\r\n");
                    // drive_sys.move(CAR_FORWARD);
                }
            }
            else if (entry.data == IR_REMOTE_8)
            {
                // drive_sys.move(CAR_BACKWARD);
                while ((entry.state = remote.refresh()))
                {
                    entry = remote.receive();
                    if (entry.data != IR_REMOTE_REPEAT_CODE || entry.addr != IR_REMOTE_REPEAT_CODE)
                    {
                        break;
                    }
                    printf("held backward\r\n");
                    // drive_sys.move(CAR_BACKWARD);
                }
            }
        }

        osDelay(1);
    }

    osThreadTerminate(NULL);
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

    void main_default_task(void)
    {
        default_task();
    }
}
