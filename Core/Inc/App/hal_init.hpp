#pragma once

extern "C"
{
#include "main.h"
}

class DriveSystem;
template <typename T>
class PTimer;
class GPIO;
class IRRemote;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern UART_HandleTypeDef huart2;

// exported objects to other files
extern PTimer<uint32_t> tim2;  // used for precise delays
extern PTimer<uint16_t> tim3;  // pwm-related

extern GPIO     IRcontrol_gpio;
extern IRRemote remote;

extern GPIO        motor1[2];
extern GPIO        motor2[2];
extern DriveSystem drive_sys;

void hal_init_all();
