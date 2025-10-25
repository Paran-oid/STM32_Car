#include "rtos.hpp"

#include "drive_sys.hpp"
#include "hal_init.hpp"
#include "irremote.hpp"

volatile bool is_cmd_sent = false;
volatile bool is_warning  = false;

void stop_motor_callback(void* argument)
{
    if (!is_cmd_sent)
    {
        drive_sys.execute(IR_REMOTE_MUTE);
    }
}

void buzzer_toggle_callback(void* argument)
{
    buzzer.state_toggle();
}

void rtos_init_all()
{
    MemPoolHandle = osMemoryPoolNew(10, sizeof(IRRemoteEntry), &MemPool_attributes);
}
