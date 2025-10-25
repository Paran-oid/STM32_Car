#include "ptimer.hpp"

#include "gpio.hpp"

template class PTimer<uint32_t>;
template class PTimer<uint16_t>;

const TIM_TypeDef* ABP2_TIMS[] = {TIM1, TIM8};

template <typename T>
uint32_t PTimer<T>::freq_get() const
{
    uint32_t pclk;

    for (const auto& tim : ABP2_TIMS)
    {
        if (m_htim.Instance == tim)
        {
            pclk = HAL_RCC_GetPCLK2Freq();
            if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1) pclk *= 2;
            return pclk / (m_htim.Instance->PSC + 1);
        }
    }

    pclk = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) pclk *= 2;

    return pclk / (m_htim.Instance->PSC + 1);
}

template <typename T>
bool PTimer<T>::start()
{
    return HAL_TIM_Base_Start(&m_htim) == HAL_OK;
}

template <typename T>
bool PTimer<T>::stop()
{
    return HAL_TIM_Base_Stop(&m_htim) == HAL_OK;
}

template <typename T>
uint32_t PTimer<T>::elapsed_us() const
{
    uint32_t ticks = __HAL_TIM_GET_COUNTER(&m_htim);
    uint32_t freq  = this->freq_get();

    return ticks / (freq / 1e6);
}

template <typename T>
void PTimer<T>::delay_us(T timeout_us)
{
    uint32_t m_freq           = this->freq_get();
    uint32_t calculated_ticks = timeout_us * (m_freq / 1e6);

    this->reset();
    while (__HAL_TIM_GetCounter(&m_htim) < calculated_ticks);
}

template <typename T>
bool PTimer<T>::delay_until(GPIO& gpio, GPIOState expected_state, T timeout_us)
{
    this->reset();

    while (gpio.state_get() != expected_state)
    {
        if (this->elapsed_us() >= timeout_us)
        {
            return false;
        }
    }

    return true;
}

template <typename T>
bool PTimer<T>::pwm_start(uint8_t channel)
{
    return HAL_TIM_PWM_Start(&m_htim, channel) == HAL_OK;
}

template <typename T>
void PTimer<T>::pwm_set(uint16_t val, uint8_t channel)
{
}

template <typename T>
bool PTimer<T>::pwm_stop(uint8_t channel)
{
    return HAL_TIM_PWM_Stop(&m_htim, channel) == HAL_OK;
}
