#include "interrupts.hpp"

#include "hal_init.hpp"
#include "irremote.hpp"
#include "rtos.hpp"

void HAL_gpio_EXTI_Callback(uint16_t gpio_pin)
{
    if (gpio_pin == GPIO_PIN_10)
    {
        HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);  // disables interrupts for EXTI15_10
        osSemaphoreRelease(IRSemHandle);
    }
}

// TODO: read value of photoresistor using ADC
/*
 * if less than threshold light LEDS up
 * else close them (if they are lighten up)
 */
