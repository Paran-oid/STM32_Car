#pragma once

extern "C"
{
#include "cmsis_os2.h"
#include "main.h"
}

#include "gpio.hpp"
#include "irremote.hpp"
#include "ptimer.hpp"

/***********************************************************
 * Public defines
 ***********************************************************/
constexpr uint8_t DISTANCE_ALERT_THRESHOLD = 10;  // (cm)

// default: between 10-202ms signal
constexpr uint8_t WARNING_DURATION_MULTIPLIER = 24;
constexpr uint8_t WARNING_DURATION_BASE       = 10;

constexpr uint8_t  PWM_SPEED_STEP = 50;
constexpr uint16_t PWM_SPEED_MAX  = 999;
constexpr uint16_t PWM_SPEED_MIN  = 499;

constexpr uint16_t BUZZER_SIGNAL = 200;

/***********************************************************
 * Public GPIO enums for specifying directions
 ***********************************************************/
enum CarDirection : uint8_t
{
    CAR_FORWARD,
    CAR_BACKWARD,

    CAR_FORWARD_RIGHT,
    CAR_FORWARD_LEFT,

    CAR_BACKWARD_RIGHT,
    CAR_BACKWARD_LEFT,

    CAR_ROTATE_RIGHT,
    CAR_ROTATE_LEFT,

    CAR_STATIONARY,
};

/***************************************************************
 * DriveSystem class for controlling the movement of the car (through the motor)
 ***************************************************************/
class DriveSystem
{
   private:
    /***********************************************************
     * Private Members
     ***********************************************************/
    // motor1(left): in1 and in2
    // motor2(right): in3 and in4
    GPIO &m_gpio_in1, &m_gpio_in2, &m_gpio_in3, &m_gpio_in4;

    osTimerId_t&      m_ostim_motor;
    PTimer<uint16_t>* m_htim_pwm;

    /***********************************************************
     * Private Methods
     ***********************************************************/
    void pins_set(bool in1, bool in2, bool in3, bool in4);
    void move(const CarDirection& direction);

   public:
    /***********************************************************
     * Constructors / Destructor
     ***********************************************************/
    DriveSystem() = delete;
    DriveSystem(GPIO& in1, GPIO& in2, GPIO& in3, GPIO& in4, osTimerId_t& ostim_motor,
                PTimer<uint16_t>* htim_pwm = nullptr)
        : m_gpio_in1 {in1},
          m_gpio_in2 {in2},
          m_gpio_in3 {in3},
          m_gpio_in4 {in4},
          m_ostim_motor {ostim_motor},
          m_htim_pwm {htim_pwm}
    {
    }

    ~DriveSystem() = default;

    /***********************************************************
     * Public Methods
     ***********************************************************/
    void execute(const IRRemoteCode& code);
};
