#include "tasks.hpp"

extern "C"
{
#include "cmsis_os2.h"
#include "main.h"
}

#include <stdio.h>
#include <string.h>

#include "drive_sys.hpp"
#include "hal_init.hpp"
#include "hcsr04.hpp"
#include "irremote.hpp"
#include "rtos.hpp"

void MainTask(void* argument)
{
    // background task
    while (1)
    {
        osDelay(1);
    }

    osThreadTerminate(NULL);
}

void IR_read_task(void* argument)
{
    while (1)
    {
        osDelay(1);

        IRRemoteEntry entry = remote.receive();
        if (entry.state)
        {
            IRRemoteEntry* item = (IRRemoteEntry*) osMemoryPoolAlloc(MemPoolHandle, osWaitForever);
            if (!item)
            {
                printf("couldn't send item");
                Error_Handler();
            }
            memcpy(item, &entry, sizeof(entry));
            osMessageQueuePut(sensorQueueHandle, &item, 0, osWaitForever);

            cmd_sent = true;
            osDelay(SIGNAL_DELAY_US);
        }
        else
        {
            cmd_sent = false;
        }

        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void car_move_task(void* argument)
{
    IRRemoteEntry* r_entry;  // received entry
    while (1)
    {
        osDelay(1);

        if (osMessageQueueGet(sensorQueueHandle, &r_entry, NULL, osWaitForever) != osOK) continue;

        if (!r_entry->state) continue;

        drive_sys.execute((IRRemoteCode) r_entry->data);

        osMemoryPoolFree(MemPoolHandle, r_entry);
        osTimerStart(stopMotorTimerHandle, SIGNAL_DELAY_US + 100);

        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void HCSR04_read_task(void* argument)
{
    int16_t distance = 0;
    while (1)
    {
        distance = hcsr04.retrieve();
        osDelay(50);
    }
    osThreadTerminate(NULL);
}

void controller_task(void* argument)
{
    while (1)
    {
        osDelay(1);
    }
}
