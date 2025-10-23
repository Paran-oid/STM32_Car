#include "rtos.hpp"

#include "drive_sys.hpp"
#include "hal_init.hpp"
#include "irremote.hpp"

volatile bool is_cmd_sent = false;
volatile bool is_warning  = false;

void stop_motor_callback(void* argument)
{
    if (!is_cmd_sent) drive_sys.execute(IR_REMOTE_MUTE);
}

void stop_warning_timer(void* argument)
{
    if (is_warning)
    {
        if (osMutexAcquire(DistanceWarnerMutexHandle, 0) == osOK)
        {
            buzzer.state_toggle();
            osMutexRelease(DistanceWarnerMutexHandle);
        }
    }
}

void rtos_init_all()
{
    MemPoolHandle = osMemoryPoolNew(10, sizeof(IRRemoteEntry), &MemPool_attributes);
}
