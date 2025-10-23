#include "tasks.hpp"

extern "C"
{
#include "cmsis_os2.h"
#include "main.h"
}

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

// TODO: try to use externs less
void IR_read_task(void* argument)
{
    while (1)
    {
        osDelay(1);

        IRRemoteEntry entry = remote.receive();
        if (entry.is_valid)
        {
            SensorRequest<IRRemoteEntry> req = {IR_SIGNAL, entry};
            rtos_queue_send(req, sensorQueueHandle);
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

// TODO: try to optimize code by using references (&) and pointers and such and avoid
// TODO: copying

// TODO: don't use #defines for constant expressions rather use constexpr
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
                IRRemoteEntry& entry = parsed->content;

                if (!entry.is_valid) continue;

                drive_sys.execute((IRRemoteCode) entry.data);

                osTimerStart(StopMotorTimerHandle, SIGNAL_DELAY_US + 100);
            }
            break;
            case DISTANCE_ALERT:
            {
                static int16_t previous_distance = 0;

                SensorRequest<int16_t>* parsed   = reinterpret_cast<SensorRequest<int16_t>*>(req);
                int16_t&                distance = parsed->content;

                if (distance <= DISTANCE_ALERT_THRESHOLD)
                {
                    uint32_t warning_duration =
                        distance * WARNING_DURATION_MULTIPLIER + WARNING_DURATION_BASE;

                    if (!is_warning)
                    {
                        is_warning = true;
                        osTimerStart(StopWarningTimerHandle, warning_duration);
                    }

                    else if (distance != previous_distance)
                    {
                        osMutexAcquire(DistanceWarnerMutexHandle, osWaitForever);

                        osTimerStop(StopWarningTimerHandle);
                        osTimerStart(StopWarningTimerHandle, warning_duration);

                        osMutexRelease(DistanceWarnerMutexHandle);
                    }

                    previous_distance = distance;
                }
                else if (is_warning)
                {
                    osMutexAcquire(DistanceWarnerMutexHandle, osWaitForever);

                    osTimerStop(StopWarningTimerHandle);
                    buzzer.state_set(LOW);
                    is_warning = false;

                    osMutexRelease(DistanceWarnerMutexHandle);
                }
                break;
            }
        }

        osMemoryPoolFree(MemPoolHandle, req);

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
        if (distance == -1) continue;

        if (distance <= 10 || (is_warning && distance > 10))
        {
            SensorRequest<int16_t> req = {DISTANCE_ALERT, distance};
            rtos_queue_send(req, sensorQueueHandle);
        }

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
