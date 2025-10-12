#pragma once

extern "C"
{
#include "main.h"
}

#include <type_traits>

enum GPIOState;
class GPIO;

/**
 * @brief 32-bit timer
 *
 */

template <typename T = uint32_t>
class Timer
{
   private:
    TIM_HandleTypeDef& m_htim;

   public:
    static_assert(std::is_same<T, uint32_t> || std::is_same<T, uint16_t>,
                  "Timer<T>: T must be uint16_t or uint32_t");

    Timer() = delete;
    Timer(TIM_HandleTypeDef& htim) : m_htim {htim}
    {
    }

    ~Timer() = default;

    void delay_us(T duration);
    // TODO: consider creating delay_ms

    bool delay_until(GPIO& gpio, GPIOState expected_state, T timeout_us);
};
