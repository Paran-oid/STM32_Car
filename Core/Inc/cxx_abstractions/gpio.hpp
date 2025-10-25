#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * Public GPIO enums for state and mode management
 ***************************************************************/
enum GPIOMode : uint8_t
{
    OUTPUT = GPIO_MODE_OUTPUT_PP,
    INPUT  = GPIO_MODE_INPUT
};

enum GPIOState : uint8_t
{
    HIGH = GPIO_PIN_SET,
    LOW  = GPIO_PIN_RESET
};

/***************************************************************
 * GPIO class for handling general-purpose register interactions
 ***************************************************************/
class GPIO
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    GPIO_TypeDef* m_port;
    uint16_t      m_pin;

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    GPIO() = delete;
    GPIO(GPIO_TypeDef* port, uint16_t pin) : m_port {port}, m_pin {pin}
    {
    }

    ~GPIO() = default;

    /***********************************************************
     * Getters and Setters
     ***********************************************************/
    void mode_set(GPIOMode mode);

    void      state_set(GPIOState state);
    void      state_toggle();
    GPIOState state_get() const;

    inline uint16_t pin_get() const
    {
        return m_pin;
    }

    inline GPIO_TypeDef* port_get() const
    {
        return m_port;
    }
};
