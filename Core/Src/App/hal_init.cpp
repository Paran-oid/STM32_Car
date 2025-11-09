#include "hal_init.hpp"

#include "drive_sys.hpp"
#include "error_handler.hpp"
#include "gpio.hpp"
#include "hcsr04.hpp"
#include "irremote.hpp"
#include "iwdg.hpp"
#include "ptimer.hpp"
#include "rtos.hpp"

sca::ptimer<uint32_t> tim2 {htim2};
sca::ptimer<uint16_t> tim3 {htim3};
sca::iwdg             iwdg {hiwdg};

sca::gpio IRControl_gpio = {IRControl_GPIO_Port, IRControl_Pin};
IRRemote  remote         = {IRControl_gpio, tim2};

sca::gpio motor1[] = {{MOTOR1_1_GPIO_Port, MOTOR1_1_Pin}, {MOTOR1_2_GPIO_Port, MOTOR1_2_Pin}};
sca::gpio motor2[] = {{MOTOR2_1_GPIO_Port, MOTOR2_1_Pin}, {MOTOR2_2_GPIO_Port, MOTOR2_2_Pin}};

DriveSystem drive_sys = {motor1[0], motor1[1], motor2[0], motor2[1], StopMotorTimerHandle, &tim3};

sca::gpio hcsr04_trig = {HCSR04_TRIG_GPIO_Port, HCSR04_TRIG_Pin};
sca::gpio hcsr04_echo = {HCSR04_ECHO_GPIO_Port, HCSR04_ECHO_Pin};
HCSR04    hcsr04      = {hcsr04_trig, hcsr04_echo, tim2};

sca::gpio buzzer   = {BUZZER_GPIO_Port, BUZZER_Pin};
sca::gpio debug_ld = {DEBUG_LD_GPIO_Port, DEBUG_LD_Pin};

void hal_init_all()
{
    if (tim2.start() != sca::SCA_RES::OK) error_handle(critical_error);
    if (tim3.start_pwm(TIM_CHANNEL_1) != sca::SCA_RES::OK) error_handle(critical_error);
}
