#pragma once

extern "C"
{
#include "main.h"
}

#include "gpio.hpp"
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
    PTimer<uint16_t>& m_htim;  // speed controller
    GPIO &            m_gpio_in1, &m_gpio_in2, &m_gpio_in3, &m_gpio_in4;

   public:
    DriveSystem() = delete;
    DriveSystem(PTimer<uint16_t>& htim, GPIO& in1, GPIO& in2, GPIO& in3, GPIO& in4)
        : m_htim(htim), m_gpio_in1(in1), m_gpio_in2(in2), m_gpio_in3(in3), m_gpio_in4(in4)
    {
    }

    void move(CarDirection direction);
    void stop();

    ~DriveSystem() = default;
};
