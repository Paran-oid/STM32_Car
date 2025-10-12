#include "timer.hpp"

#include "gpio.hpp"

// TODO: make it calculate how many ticks are needed (this works only for 1ms ticks timer)

template <typename T>
void Timer<T>::delay_us(T duration)
{
    __HAL_TIM_SET_COUNTER(m_htim, 0);
    while (__HAL_TIM_GetCounter(&m_htim) < duration);
}

template <typename T>
bool Timer<T>::delay_until(GPIO& gpio, GPIOState expected_state, T timeout_us)
{
    __HAL_TIM_SET_COUNTER(m_htim, 0);
    while (__HAL_TIM_GetCounter(&m_htim) < timeout_us)
    {
        count++;
        if (gpio.get() == expected_state) return true;
    }
    return false;
}
