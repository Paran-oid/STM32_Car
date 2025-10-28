#include "drive_sys.hpp"

extern "C"
{
#include "cmsis_os.h"
}

#include "hal_init.hpp"
#include "rtos.hpp"

void DriveSystem::pins_set(bool in1, bool in2, bool in3, bool in4)
{
    m_gpio_in1.state_set(in1 ? HIGH : LOW);
    m_gpio_in2.state_set(in2 ? HIGH : LOW);
    m_gpio_in3.state_set(in3 ? HIGH : LOW);
    m_gpio_in4.state_set(in4 ? HIGH : LOW);
}

void DriveSystem::move(CarDirection direction)
{
    switch (direction)
    {
        // Move both motors forward → car goes straight forward
        case CAR_FORWARD:
            this->pins_set(1, 0, 1, 0);  // Left motor forward, Right motor forward
            break;

        // Move both motors backward → car goes straight backward
        case CAR_BACKWARD:
            this->pins_set(0, 1, 0, 1);  // Left motor backward, Right motor backward
            break;

        // Turn slightly right while moving forward → left motor runs, right motor stops
        case CAR_FORWARD_RIGHT:
            this->pins_set(1, 0, 0, 0);  // Left motor forward, Right motor stop
            break;

        // Turn slightly left while moving forward → right motor runs, left motor stops
        case CAR_FORWARD_LEFT:
            this->pins_set(0, 0, 1, 0);  // Left motor stop, Right motor forward
            break;

        // Move backward and turn slightly right → left motor runs backward, right motor stops
        case CAR_BACKWARD_RIGHT:
            this->pins_set(0, 1, 0, 0);  // Left motor backward, Right motor stop
            break;

        // Move backward and turn slightly left → right motor runs backward, left motor stops
        case CAR_BACKWARD_LEFT:
            this->pins_set(0, 0, 0, 1);  // Left motor stop, Right motor backward
            break;

        // Rotate clockwise (on the spot) → left motor forward, right motor backward
        case CAR_ROTATE_RIGHT:
            this->pins_set(1, 0, 0, 1);  // Left forward, Right backward
            break;

        // Rotate counterclockwise (on the spot) → left motor backward, right motor forward
        case CAR_ROTATE_LEFT:
            this->pins_set(0, 1, 1, 0);  // Left backward, Right forward
            break;

        case CAR_STATIONARY:
        default:
            this->pins_set(0, 0, 0, 0);
            break;
    }
}

void DriveSystem::speed_handle(IRRemoteCode code)
{
    uint16_t cmp_val = m_htim_pwm->pwm_get(TIM_CHANNEL_1);

    const bool can_increase = cmp_val < PWM_SPEED_MAX;
    const bool can_decrease = cmp_val > PWM_SPEED_MIN;

    if (!can_increase && !can_decrease) return;

    if (can_increase && code == IR_REMOTE_VOL_UP)
    {
        cmp_val += PWM_SPEED_STEP;
    }
    else if (can_decrease && code == IR_REMOTE_VOL_DOWN)
    {
        cmp_val -= PWM_SPEED_STEP;
    }

    m_htim_pwm->pwm_set(cmp_val, TIM_CHANNEL_1);
    buzzer.state_set(HIGH);
    osTimerStart(BuzzerToggleTimerHandle, 200);
}

void DriveSystem::execute(IRRemoteCode code)
{
    static CarDirection direction = CAR_STATIONARY;
    // maps IR code to direction
    switch (code)
    {
        case IR_REMOTE_2:
            direction = CAR_FORWARD;
            break;

        case IR_REMOTE_8:
            direction = CAR_BACKWARD;
            break;

        case IR_REMOTE_3:
            direction = CAR_FORWARD_RIGHT;
            break;

        case IR_REMOTE_1:
            direction = CAR_FORWARD_LEFT;
            break;

        case IR_REMOTE_6:
            direction = CAR_ROTATE_RIGHT;
            break;

        case IR_REMOTE_4:
            direction = CAR_ROTATE_LEFT;
            break;

        case IR_REMOTE_9:
            direction = CAR_BACKWARD_RIGHT;
            break;

        case IR_REMOTE_7:
            direction = CAR_BACKWARD_LEFT;
            break;

        case IR_REMOTE_REPEAT_CODE:
            // the same direction will be used just as previously (no code written on purpose)
            break;

        case IR_REMOTE_VOL_UP:
        case IR_REMOTE_VOL_DOWN:
            if (!m_htim_pwm) break;
            this->speed_handle(code);
            break;

        case IR_REMOTE_OFF:
            NVIC_SystemReset();
            break;

        case IR_REMOTE_MUTE:
        default:
            direction = CAR_STATIONARY;
            break;
    }

    this->move(direction);
}
