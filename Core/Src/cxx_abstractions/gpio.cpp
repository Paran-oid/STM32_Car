#include "gpio.hpp"

void GPIO::set_mode(GPIOMode mode)
{
    GPIO_InitTypeDef gpio_initializer = {0};

    gpio_initializer.Pin  = m_pin;
    gpio_initializer.Mode = mode;
    gpio_initializer.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(m_port, &gpio_initializer);
}

void GPIO::set(GPIOState state)
{
    HAL_GPIO_WritePin(m_port, m_pin, (GPIO_PinState) state);
}

GPIOState GPIO::get()
{
    return HAL_GPIO_ReadPin(m_port, m_pin) == GPIO_PIN_RESET ? LOW : HIGH;
}
