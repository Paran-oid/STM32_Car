#pragma once

extern "C"
{
#include "main.h"
}

#include "gpio.hpp"
#include "ptimer.hpp"

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint16_t HCSR04_MIN_RANGE = 2;
constexpr uint16_t HCSR04_MAX_RANGE = 400;

/***********************************************************
 * HCSR04 class related to anything concering the sensor
 ***********************************************************/
class HCSR04
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    GPIO &            m_echo, &m_trig;
    PTimer<uint32_t>& m_htim;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    HCSR04() = delete;
    HCSR04(GPIO& trig, GPIO& echo, PTimer<uint32_t>& htim)
        : m_echo {echo}, m_trig {trig}, m_htim {htim}
    {
    }
    ~HCSR04() = default;

    /***********************************************************
     * Public Methods
     ***********************************************************/
    int16_t retrieve();
};
