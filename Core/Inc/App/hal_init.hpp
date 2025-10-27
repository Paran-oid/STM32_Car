#pragma once

extern "C"
{
#include "main.h"
}

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint16_t IWDG_DELAY = 1000;  // in ms

/***************************************************************
 * Forward declarations
 ***************************************************************/
class DriveSystem;

template <typename T>
class PTimer;

class GPIO;
class IRRemote;
class HCSR04;
class _IWDG;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern TIM_HandleTypeDef htim2;  // Precise timer (sensor communications)
extern TIM_HandleTypeDef htim3;  // PWM timer (speed motor)

extern IWDG_HandleTypeDef hiwdg;

/***************************************************************
 * Exported objects
 ***************************************************************/
extern PTimer<uint32_t> tim2;  // used for precise delays
extern PTimer<uint16_t> tim3;  // used for handling motor speed

extern _IWDG iwdg;

extern GPIO     IRcontrol;
extern IRRemote remote;

extern GPIO        motor1[2], motor2[2];
extern DriveSystem drive_sys;

extern GPIO   hcsr04_trig, hcsr04_echo;
extern HCSR04 hcsr04;

extern GPIO buzzer;
extern GPIO debug_ld;

/***************************************************************
 * Function declarations
 ***************************************************************/
void hal_init_all();
