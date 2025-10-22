#include "gpio.hpp"

void GPIO::mode_set(GPIOMode mode)
{
    GPIO_InitTypeDef gpio_initializer = {0};

    gpio_initializer.Pin  = m_pin;
    gpio_initializer.Mode = mode;
    gpio_initializer.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(m_port, &gpio_initializer);
}

void GPIO::state_set(GPIOState state)
{
    HAL_GPIO_WritePin(m_port, m_pin, (GPIO_PinState) state);
}

void GPIO::state_toggle()
{
    HAL_GPIO_TogglePin(m_port, m_pin);
}

GPIOState GPIO::state_get() const
{
    return HAL_GPIO_ReadPin(m_port, m_pin) == GPIO_PIN_RESET ? LOW : HIGH;
}
