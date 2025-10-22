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
        if (entry.is_valid)
        {
            SensorRequest<IRRemoteEntry> req  = {IR_SIGNAL, entry};
            void*                        item = osMemoryPoolAlloc(MemPoolHandle, osWaitForever);
            if (!item)
            {
                printf("couldn't allocate item in memory pool");
                Error_Handler();
            }
            memcpy(item, &req, sizeof(req));
            osMessageQueuePut(sensorQueueHandle, &item, 0, osWaitForever);

            is_cmd_sent = true;
            osDelay(SIGNAL_DELAY_US);
        }
        else
        {
            is_cmd_sent = false;
        }

        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void car_move_task(void* argument)
{
    void* req;  // received request pointer
    while (1)
    {
        osDelay(1);

        if (osMessageQueueGet(sensorQueueHandle, &req, NULL, osWaitForever) != osOK) continue;
        uint8_t code = *(reinterpret_cast<uint8_t*>(req));

        switch (code)
        {
            case IR_SIGNAL:
            {
                SensorRequest<IRRemoteEntry>* parsed =
                    reinterpret_cast<SensorRequest<IRRemoteEntry>*>(req);

                if (!parsed->content.is_valid) continue;

                drive_sys.execute((IRRemoteCode) parsed->content.data);

                osMemoryPoolFree(MemPoolHandle, parsed);
                osTimerStart(StopMotorTimerHandle, SIGNAL_DELAY_US + 100);
            }
            break;
            case BUZZER_ON:
            {
                break;
            }
        }

        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void HCSR04_read_task(void* argument)
{
    uint32_t previous_warning_duration = 0;
    int16_t  distance                  = 0;

    while (1)
    {
        distance = hcsr04.retrieve();
        if (distance == -1) continue;

        // TODO: add this to control task
        if (distance <= 5)
        {
            uint32_t warning_duration = distance * 20 + 100;  // between 200 and 400 ms duration

            if (!is_warning)
            {
                is_warning = true;
                osTimerStart(StopWarningTimerHandle, warning_duration);
            }
            else if (warning_duration != previous_warning_duration)
            {
                osTimerStop(StopWarningTimerHandle);
                osTimerStart(StopWarningTimerHandle, warning_duration);
            }
            previous_warning_duration = warning_duration;
        }
        else if (is_warning)
        {
            osTimerStop(StopWarningTimerHandle);
            buzzer.state_set(LOW);
            is_warning = false;
        }

        osDelay(100);
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
