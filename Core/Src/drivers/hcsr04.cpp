#include "hcsr04.hpp"

#include "rtos.hpp"

#define SPEED_OF_SOUND 340

int16_t HCSR04::retrieve()
{
    osMutexAcquire(HTIM3MutexHandle, osWaitForever);

    this->m_trig.state_set(HIGH);
    m_htim.delay_us(10);
    this->m_trig.state_set(LOW);

    if (!m_htim.delay_until(m_echo, HIGH, 5000))
    {
        osMutexRelease(HTIM3MutexHandle);
        return -1;
    }
    m_htim.reset();

    if (!m_htim.delay_until(m_echo, LOW, 5000))
    {
        osMutexRelease(HTIM3MutexHandle);
        return -1;
    }

    uint32_t elapsed_us = m_htim.elapsed_us();

    float distance_cm = (elapsed_us * 0.017f);  // 340 / (2 * 1000)

    osMutexRelease(HTIM3MutexHandle);

    return (int16_t) distance_cm;
}
