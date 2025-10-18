#pragma once

extern "C"
{
#include "cmsis_os2.h"
#include "main.h"
}

#include "gpio.hpp"
#include "irremote.hpp"
#include "ptimer.hpp"

enum CarDirection
{
    CAR_FORWARD,
    CAR_BACKWARD,

    CAR_FORWARD_RIGHT,
    CAR_FORWARD_LEFT,

    CAR_BACKWARD_RIGHT,
    CAR_BACKWARD_LEFT,

    CAR_ROTATE_RIGHT,
    CAR_ROTATE_LEFT,

    CAR_STATIONARY,
};

class DriveSystem
{
   private:
    // motor1(left): in1 and in2
    // motor2(right): in3 and in4
    GPIO &m_gpio_in1, &m_gpio_in2, &m_gpio_in3, &m_gpio_in4;

    osTimerId_t& m_ostim_motor;

    void pins_set(bool in1, bool in2, bool in3, bool in4);
    void move(CarDirection direction);

   public:
    DriveSystem() = delete;
    DriveSystem(GPIO& in1, GPIO& in2, GPIO& in3, GPIO& in4, osTimerId_t& ostim_motor)
        : m_gpio_in1 {in1},
          m_gpio_in2 {in2},
          m_gpio_in3 {in3},
          m_gpio_in4 {in4},
          m_ostim_motor {ostim_motor}
    {
    }

    void exec(IRRemoteCode code);

    ~DriveSystem() = default;
};
