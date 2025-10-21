#pragma once

extern "C"
{
#include "main.h"
}

#include "gpio.hpp"
#include "ptimer.hpp"

class HCSR04
{
   private:
    GPIO &            m_echo, &m_trig;
    PTimer<uint32_t>& m_htim;

   public:
    HCSR04() = delete;
    HCSR04(GPIO& trig, GPIO& echo, PTimer<uint32_t>& htim)
        : m_echo {echo}, m_trig {trig}, m_htim {htim}
    {
    }

    int16_t retrieve();

    ~HCSR04() = default;
};
