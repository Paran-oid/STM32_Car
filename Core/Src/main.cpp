extern "C"
{
#include "main.h"

#include "cmsis_os2.h"
}

extern UART_HandleTypeDef huart2;

//
// Main thread
//
void setup()
{
    HAL_UART_Transmit(&huart2, (const uint8_t*) "setup done!\r\n", 13, 100);
}

// should never be reached
void loop()
{
}

//
// Tasks
//
void default_task()
{
    while (1)
    {
        osDelay(1);
    }

    osThreadTerminate(NULL);
}

extern "C"
{
    void main_setup(void)
    {
        setup();
    }

    void main_loop(void)
    {
        loop();
    }

    void main_default_task(void)
    {
        default_task();
    }
}
