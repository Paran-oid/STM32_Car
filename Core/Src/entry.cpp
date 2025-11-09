
#include "gpio.hpp"
#include "hal_init.hpp"
#include "rtos.hpp"
#include "tasks.hpp"

void setup()
{
    hal_init_all();
    rtos_init_all();
}

void loop()
{
    // should never be reached
}
