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

void main_task()
{
    // background task
    while (1)
    {
        osDelay(1);
    }

    osThreadTerminate(NULL);
}

void IR_read_task()
{
    while (1)
    {
        // osDelay(1);

        // IRRemoteEntry entry = remote.receive();
        // if (entry.state)
        // {
        //     IRRemoteEntry* item = (IRRemoteEntry*) osMemoryPoolAlloc(MemPoolHandle,
        //     osWaitForever); if (!item)
        //     {
        //         printf("couldn't send item");
        //         Error_Handler();
        //     }
        //     memcpy(item, &entry, sizeof(entry));
        //     osMessageQueuePut(carInstructionsHandle, &item, 0, osWaitForever);

        //     cmd_sent = true;
        //     osDelay(250);
        // }
        // else
        // {
        //     cmd_sent = false;
        // }

        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void car_move_task()
{
    IRRemoteEntry* r_entry;  // received entry
    while (1)
    {
        // osDelay(1);

        // if (osMessageQueueGet(carInstructionsHandle, &r_entry, NULL, osWaitForever) != osOK)
        //     continue;

        // if (!r_entry->state) continue;

        // drive_sys.execute((IRRemoteCode) r_entry->data);

        // osMemoryPoolFree(MemPoolHandle, r_entry);
        // osTimerStart(stopMotorTimerHandle, 300);
        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void HCSR04_read_task()
{
    uint16_t distance = 0;
    while (1)
    {
        distance = hcsr04.retrieve();
        printf("distance is %lu\r\n", distance);
        osDelay(500);
    }
    osThreadTerminate(NULL);
}
