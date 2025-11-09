#include "error_handler.hpp"

#include "gpio.hpp"
#include "hal_init.hpp"
#include "ptimer.hpp"
#include "rtos.hpp"

void error_handle(void (*alert)(void))
{
    if (!alert) alert = critical_error;
    alert();
}

void critical_error(void)
{
    __disable_irq();
    while (1)
    {
        debug_ld.set_state(sca::HIGH);
        tim2.delay_us(1000000);
        debug_ld.set_state(sca::LOW);
        tim2.delay_us(1000000);
    }
}

void warning_error(void)
{
    __disable_irq();

    for (volatile uint8_t i = 0; i < 5; i++)
    {
        debug_ld.set_state(sca::HIGH);
        tim2.delay_us(500000);  // 500 ms
        debug_ld.set_state(sca::LOW);
        tim2.delay_us(500000);
    }

    __enable_irq();
}
