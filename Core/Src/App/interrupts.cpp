#include "interrupts.hpp"

#include "hal_init.hpp"
#include "irremote.hpp"
#include "rtos.hpp"

void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
    if (gpio_pin == GPIO_PIN_10)
    {
        HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
        osSemaphoreRelease(IRSemHandle);
        // TODO: make sure interrupt doesn't occur each damn time on rising edge
    }
}
