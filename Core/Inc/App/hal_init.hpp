#pragma once

extern "C"
{
#include "main.h"
}

/***************************************************************
 * Forward declarations
 ***************************************************************/
class DriveSystem;

template <typename T>
class PTimer;

class GPIO;
class IRRemote;
class HCSR04;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern TIM_HandleTypeDef  htim2;  // Precise timer (sensor communications)
extern TIM_HandleTypeDef  htim3;  // PWM timer (speed motor)
extern UART_HandleTypeDef huart2;

extern IWDG_HandleTypeDef hiwdg;

/***************************************************************
 * Exported objects
 ***************************************************************/
extern PTimer<uint32_t> tim2;  // used for precise delays

extern GPIO     IRcontrol_gpio;
extern IRRemote remote;

extern GPIO        motor1[2], motor2[2];
extern DriveSystem drive_sys;

extern GPIO   hcsr04_trig, hcsr04_echo;
extern HCSR04 hcsr04;

extern GPIO buzzer;

/***************************************************************
 * Function declarations
 ***************************************************************/
void hal_init_all();
