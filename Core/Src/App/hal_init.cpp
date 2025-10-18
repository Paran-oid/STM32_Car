#include "hal_init.hpp"

#include "drive_sys.hpp"
#include "gpio.hpp"
#include "irremote.hpp"
#include "ptimer.hpp"
#include "rtos.hpp"

PTimer<uint32_t> tim2 {htim2};  // used for precise delays

GPIO     IRcontrol_gpio = {IRcontrol_GPIO_Port, IRcontrol_Pin};
IRRemote remote         = {IRcontrol_gpio, tim2};

GPIO motor1[] = {{MOTOR1_1_GPIO_Port, MOTOR1_1_Pin}, {MOTOR1_2_GPIO_Port, MOTOR1_2_Pin}};
GPIO motor2[] = {{MOTOR2_1_GPIO_Port, MOTOR2_1_Pin}, {MOTOR2_2_GPIO_Port, MOTOR2_2_Pin}};

DriveSystem drive_sys = {motor1[0],           motor1[1],
                         motor2[0], motor2[1],     stopMotorTimerHandle};

void hal_init_all()
{
    tim2.start();

    tim3.pwm_start(TIM_CHANNEL_2);
    IRcontrol_gpio.set_mode(INPUT);
}
