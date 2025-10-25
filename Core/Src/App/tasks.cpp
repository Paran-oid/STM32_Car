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

// TODO: include more interrupts in the program
// TODO: consider including DMA for some things
void main_task(void* argument)
{
    /* USER CODE BEGIN 5 */
    while (1)
    {
        HAL_IWDG_Refresh(&hiwdg);  // reset watch dog timer of 4 seconds
        osDelay(1000);
    }
    osThreadTerminate(NULL);
    /* USER CODE END 5 */
}

void IR_read_task(void* argument)
{
    while (1)
    {
        if (osSemaphoreAcquire(IRSemHandle, osWaitForever) != osOK) continue;

        const IRRemoteEntry entry = remote.retrieve();
        if (entry.is_valid)
        {
            const SensorRequest<IRRemoteEntry> req = {IR_SIGNAL, entry};
            rtos_queue_send(req, sensorQueueHandle);
            is_cmd_sent = true;
            osDelay(SIGNAL_DELAY_US);
        }
        else
        {
            is_cmd_sent = false;
        }

        HAL_NVIC_EnableIRQ(
            EXTI15_10_IRQn);  // allow EXTI15_10 interrupt to occur on rising edge again
        // osDelay(1); (not needed because it's event-driven)
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
            ((is_warning && distance > DISTANCE_ALERT_THRESHOLD)))
        {
            const SensorRequest<int16_t> req = {DISTANCE_ALERT, distance};
            rtos_queue_send(req, sensorQueueHandle);
        }

        osDelay(100);
    }

    osThreadTerminate(NULL);
}

void controller_task(void* argument)
{
    void* req;
    while (1)
    {
        if (osMessageQueueGet(sensorQueueHandle, &req, NULL, osWaitForever) != osOK) continue;
        uint8_t& code = *(reinterpret_cast<uint8_t*>(req));

        switch (code)
        {
            case IR_SIGNAL:
            {
                const SensorRequest<IRRemoteEntry>* parsed =
                    reinterpret_cast<const SensorRequest<IRRemoteEntry>*>(req);
                const IRRemoteEntry& entry = parsed->content;

                if (!entry.is_valid) continue;

                drive_sys.execute((IRRemoteCode) entry.data);

                osTimerStart(
                    StopMotorTimerHandle,
                    SIGNAL_DELAY_US + 100);  // 100 ms offset so that car doesn't stop abruptly
            }
            break;
            case DISTANCE_ALERT:
            {
                const SensorRequest<int16_t>* parsed =
                    reinterpret_cast<SensorRequest<int16_t>*>(req);
                int16_t distance = parsed->content;

                if (distance <= DISTANCE_ALERT_THRESHOLD)
                {
                    if (!is_warning) is_warning = true;

                    if (!osTimerIsRunning(BuzzerToggleTimerHandle))
                        osTimerStart(BuzzerToggleTimerHandle, buzzer_toggle_callback_DELAY);
                }
                else if (is_warning && distance > DISTANCE_ALERT_THRESHOLD)
                {
                    osTimerStop(BuzzerToggleTimerHandle);
                    buzzer.state_set(LOW);
                    is_warning = false;
                }
                break;
            }
        }

        osMemoryPoolFree(MemPoolHandle, req);
        // osDelay(1) (not needed because it's event-driven);
    }

    osThreadTerminate(NULL);
}
