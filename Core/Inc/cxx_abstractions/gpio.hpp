#pragma once

extern "C"
{
#include "main.h"
}

enum GPIOMode
{
    OUTPUT = GPIO_MODE_OUTPUT_PP,
    INPUT  = GPIO_MODE_INPUT
};

enum GPIOState
{
    HIGH = GPIO_PIN_SET,
    LOW  = GPIO_PIN_RESET
};

class GPIO
{
   private:
    GPIO_TypeDef* m_port;
    uint16_t      m_pin;

   public:
    GPIO() = delete;
    GPIO(GPIO_TypeDef* port, uint16_t pin) : m_port {port}, m_pin {pin}
    {
    }

    ~GPIO() = default;

    void set_mode(GPIOMode mode);
    void set(GPIOState state);

    GPIOState get();
};
