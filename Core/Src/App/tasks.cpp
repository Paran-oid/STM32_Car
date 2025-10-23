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

void car_move_task(void* argument)
{
    while (1)
    {
        osDelay(1);
    }
    osThreadTerminate(NULL);
}

void HCSR04_read_task(void* argument)
{
    while (1)
    {
        int16_t distance = hcsr04.retrieve();

        if (distance <= -1) continue;

        if (distance <= DISTANCE_ALERT_THRESHOLD ||
            (is_warning && distance > DISTANCE_ALERT_THRESHOLD))
        {
            SensorRequest<int16_t> req = {DISTANCE_ALERT, distance};
            rtos_queue_send(req, sensorQueueHandle);
        }

        osDelay(150);
    }
    osThreadTerminate(NULL);
}

void controller_task(void* argument)
{
    void* req;
    while (1)
    {
        osDelay(1);

        if (osMessageQueueGet(sensorQueueHandle, &req, NULL, osWaitForever) != osOK) continue;
        uint8_t& code = *(reinterpret_cast<uint8_t*>(req));

        switch (code)
        {
            case IR_SIGNAL:
            {
                SensorRequest<IRRemoteEntry>* parsed =
                    reinterpret_cast<SensorRequest<IRRemoteEntry>*>(req);
                IRRemoteEntry& entry = parsed->content;

                if (!entry.is_valid) continue;

                drive_sys.execute((IRRemoteCode) entry.data);

                osTimerStart(
                    StopMotorTimerHandle,
                    SIGNAL_DELAY_US + 100);  // 100 ms offset so that car doesn't stop abruptly
            }
            break;
            case DISTANCE_ALERT:
            {
                SensorRequest<int16_t>* parsed   = reinterpret_cast<SensorRequest<int16_t>*>(req);
                int16_t&                distance = parsed->content;

                if (distance <= DISTANCE_ALERT_THRESHOLD)
                {
                    if (!is_warning)
                    {
                        is_warning = true;
                        osTimerStart(StopWarningTimerHandle, STOP_WARNING_TIMER_DELAY);
                    }
                }
                else if (is_warning)
                {
                    osTimerStop(StopWarningTimerHandle);
                    buzzer.state_set(LOW);
                    is_warning = false;
                }
                break;
            }
        }

        osMemoryPoolFree(MemPoolHandle, req);

        osDelay(1);
    }
}
