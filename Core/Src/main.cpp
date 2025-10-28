
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

extern "C"
{
    // Mapping main.c to main.cpp functions/tasks

    void main_setup(void)
    {
        setup();
    }

    void main_loop(void)
    {
        loop();  // should never be reached
    }
}
