#include "drive_sys.hpp"

extern "C"
{
#include "cmsis_os.h"
}

void DriveSystem::stop()
{
    m_gpio_in1.set(LOW);
    m_gpio_in2.set(LOW);
    m_gpio_in3.set(LOW);
    m_gpio_in4.set(LOW);
}

void DriveSystem::move(CarDirection direction)
{
    switch (direction)
    {
        case CAR_FORWARD:
            m_gpio_in1.set(HIGH);
            m_gpio_in2.set(LOW);
            m_gpio_in3.set(HIGH);
            m_gpio_in4.set(LOW);
            break;

        case CAR_BACKWARD:
            m_gpio_in1.set(LOW);
            m_gpio_in2.set(HIGH);
            m_gpio_in3.set(LOW);
            m_gpio_in4.set(HIGH);
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

        case CAR_STATIONARY:
        default:
            this->stop();
            // stop motors or handle invalid direction
            break;
    }
}

void DriveSystem::exec(IRRemoteCode code)
{
    static CarDirection direction = CAR_STATIONARY;

    // map code to direction
    switch (code)
    {
        case IR_REMOTE_2:
            direction = CAR_FORWARD;
            break;

        case IR_REMOTE_8:
            direction = CAR_BACKWARD;
            break;
        case IR_REMOTE_REPEAT_CODE:
            break;
        case IR_REMOTE_MUTE:
        default:
            direction = CAR_STATIONARY;
            break;
    }

    this->move(direction);
}
