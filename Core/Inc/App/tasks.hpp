#pragma once

/***************************************************************
 * Function declarations
 ***************************************************************/
extern "C"
{
    void main_task(void* argument);
    void IR_read_task(void* argument);
    void car_move_task(void* argument);
    void HCSR04_read_task(void* argument);
    void controller_task(void* argument);
}
