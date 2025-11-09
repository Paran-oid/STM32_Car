#include "hcsr04.hpp"

#include "rtos.hpp"

int16_t HCSR04::retrieve()
{
    static int16_t last_valid = 0;

    if (osMutexAcquire(HTIM3MutexHandle, osWaitForever) != osOK) return -1;

    this->m_trig.set_state(sca::HIGH);
    m_htim.delay_us(10);
    this->m_trig.set_state(sca::LOW);

    if (!m_htim.delay_until(m_echo, sca::HIGH, 3000))
    {
        osMutexRelease(HTIM3MutexHandle);
        return -1;
    }

    m_htim.reset();

    if (!m_htim.delay_until(m_echo, sca::LOW, 24000))
    {
        osMutexRelease(HTIM3MutexHandle);
        return -1;
    }

    uint32_t elapsed_us = m_htim.elapsed_us();

    if (elapsed_us < 100 || elapsed_us > 25000)
    {
        osMutexRelease(HTIM3MutexHandle);
        return -1;  // Ignore impossible readings (<2cm or >400cm)
    }

    int16_t distance_cm = (int16_t) (elapsed_us * 0.017f);  // 340 / (2 * 1000)

    if (distance_cm < 2 || distance_cm > 400)
    {
        distance_cm = last_valid;
    }

    osMutexRelease(HTIM3MutexHandle);

    return distance_cm;
}
