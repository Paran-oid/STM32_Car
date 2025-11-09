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

namespace sca
{
template <typename T>
class ptimer;

class gpio;
class iwdg;

}  // namespace sca

class IRRemote;
class HCSR04;

/***************************************************************
 * HAL generated variables in main.c
 ***************************************************************/
extern TIM_HandleTypeDef htim2;  // Precise timer (sensor communications)
extern TIM_HandleTypeDef htim3;  // PWM timer (speed motor)

extern IWDG_HandleTypeDef hiwdg;

/***************************************************************
 * Exported objects
 ***************************************************************/
extern sca::ptimer<uint32_t> tim2;  // used for precise delays
extern sca::ptimer<uint16_t> tim3;  // used for handling motor speed

extern sca::iwdg iwdg;

extern sca::gpio IRcontrol;
extern IRRemote  remote;

extern sca::gpio   motor1[2], motor2[2];
extern DriveSystem drive_sys;

extern sca::gpio hcsr04_trig, hcsr04_echo;
extern HCSR04    hcsr04;

extern sca::gpio buzzer;
extern sca::gpio debug_ld;

/***************************************************************
 * Function declarations
 ***************************************************************/
void hal_init_all();
