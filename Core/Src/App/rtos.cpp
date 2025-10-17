#include "rtos.hpp"

#include "drive_sys.hpp"
#include "hal_init.hpp"
#include "irremote.hpp"

bool cmd_sent = false;

void stop_motor_callback(void* args)
{
    if (!cmd_sent) drive_sys.exec(IR_REMOTE_MUTE);
}

void rtos_init_all()
{
    MemPoolHandle = osMemoryPoolNew(10, sizeof(IRRemoteEntry), &MemPool_attributes);
}
