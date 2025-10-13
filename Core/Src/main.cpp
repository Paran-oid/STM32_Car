extern "C"
{
#include "main.h"

#include "cmsis_os2.h"
}

#include "gpio.hpp"
#include "irremote.hpp"
#include "ptimer.hpp"

extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart2;

PTimer<uint32_t> tim2 {htim2};  // used for precise delays
PTimer<uint32_t> tim3 {htim3};  // pwm-related

//
// Main thread
//
void setup()
{
    /**
     * TODO LIST
     * ! Second part:
     * * - Test driver for IR decoder
     * * - Write logic for motors.c (all possible movements)
     * * - Test car with new added logic
     *
     * ! Third part:
     * * - Add bluetooth
     * * - Test logic and make sure 100% of the code works
     * * - Control the car using infra-red signals (for now)
     *
     * ?--> Once done we will start on other tasks (bluetooth in this case)
     */

    tim2.start();

    tim3.pwm_start(TIM_CHANNEL_2);
    tim3.pwm_start(TIM_CHANNEL_3);
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
        // HAL_GPIO_WritePin(MOTOR1_1_GPIO_Port, MOTOR1_1_Pin, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(MOTOR1_2_GPIO_Port, MOTOR1_2_Pin, GPIO_PIN_RESET);

        // HAL_GPIO_WritePin(MOTOR2_1_GPIO_Port, MOTOR2_1_Pin, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(MOTOR2_2_GPIO_Port, MOTOR2_2_Pin, GPIO_PIN_RESET);

        // osDelay(500);

        // HAL_GPIO_WritePin(MOTOR1_1_GPIO_Port, MOTOR1_1_Pin, GPIO_PIN_RESET);
        // HAL_GPIO_WritePin(MOTOR1_2_GPIO_Port, MOTOR1_2_Pin, GPIO_PIN_SET);

        // HAL_GPIO_WritePin(MOTOR2_1_GPIO_Port, MOTOR2_1_Pin, GPIO_PIN_RESET);
        // HAL_GPIO_WritePin(MOTOR2_2_GPIO_Port, MOTOR2_2_Pin, GPIO_PIN_SET);

        // osDelay(500);

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
