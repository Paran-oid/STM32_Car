#pragma once

extern "C"
{
#include "main.h"
}

#include <type_traits>

#include "gpio.hpp"

/**
 * @brief precise (micro second level) 32/16-bit timer
 * @note it's assumed that timers use internal clock of MCU
 */

template <typename T = uint32_t>
class PTimer
{
   private:
    TIM_HandleTypeDef& m_htim;

    uint32_t freq_get() const;

   public:
    static_assert(std::is_same_v<T, uint32_t> || std::is_same_v<T, uint16_t>,
                  "Timer<T>: T must be uint16_t or uint32_t");

    PTimer() = delete;
    PTimer(TIM_HandleTypeDef& htim) : m_htim {htim}
    {
    }

    ~PTimer() = default;

    // timer specific
    bool start();
    bool stop();

    inline void reset() const
    {
        __HAL_TIM_SET_COUNTER(&m_htim, 0);
    }

    uint32_t elapsed_us() const;
    void     delay_us(T period_us);
    bool     delay_until(GPIO& gpio, GPIOState expected_state, T period_us);

    // pulse-width modulation generation
    bool pwm_start(uint8_t channel);
    bool pwm_stop(uint8_t channel);
};
