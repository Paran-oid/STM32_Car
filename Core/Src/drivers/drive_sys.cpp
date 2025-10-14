#include "drive_sys.hpp"

extern "C"
{
#include "cmsis_os.h"
}

void DriveSystem::clear()
{
    HAL_GPIO_WritePin(MOTOR1_1_GPIO_Port, MOTOR1_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR1_2_GPIO_Port, MOTOR1_2_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTOR2_1_GPIO_Port, MOTOR2_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR2_2_GPIO_Port, MOTOR2_2_Pin, GPIO_PIN_RESET);
}

void DriveSystem::move(CarDirection direction)
{
    switch (direction)
    {
        case CAR_FORWARD:
            HAL_GPIO_WritePin(MOTOR1_1_GPIO_Port, MOTOR1_1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR1_2_GPIO_Port, MOTOR1_2_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(MOTOR2_1_GPIO_Port, MOTOR2_1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(MOTOR2_2_GPIO_Port, MOTOR2_2_Pin, GPIO_PIN_RESET);

            break;

        case CAR_BACKWARD:
            HAL_GPIO_WritePin(MOTOR1_1_GPIO_Port, MOTOR1_1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR1_2_GPIO_Port, MOTOR1_2_Pin, GPIO_PIN_SET);

            HAL_GPIO_WritePin(MOTOR2_1_GPIO_Port, MOTOR2_1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(MOTOR2_2_GPIO_Port, MOTOR2_2_Pin, GPIO_PIN_SET);
            break;

        case CAR_FORWARD_RIGHT:
            // left motor forward, right motor slower or stopped
            break;

        case CAR_FORWARD_LEFT:
            // right motor forward, left motor slower or stopped
            break;

        case CAR_BACKWARD_RIGHT:
            // left motor backward, right motor slower or stopped
            break;

        case CAR_BACKWARD_LEFT:
            // right motor backward, left motor slower or stopped
            break;

        case CAR_ROTATE_RIGHT:
            // left motor forward, right motor backward
            break;

        case CAR_ROTATE_LEFT:
            // right motor forward, left motor backward
            break;

        default:
            // stop motors or handle invalid direction
            break;
    }

    osDelay(350);
    // TODO: create some sort of interrupt to stop car movement
    this->clear();  // basically stop
}

