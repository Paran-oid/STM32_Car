#include "hcsr04.hpp"

#include "rtos.hpp"

#define SPEED_OF_SOUND 340

int16_t HCSR04::retrieve()
{
    osMutexAcquire(ptimerMutexHandle, osWaitForever);

    m_trig.set_mode(OUTPUT);
    this->m_trig.set(HIGH);
    m_htim.delay_us(10);
    this->m_trig.set(LOW);

    if (!m_htim.delay_until(m_echo, HIGH, 500))
    {
        osMutexRelease(ptimerMutexHandle);
        return -1;
    }
    m_htim.reset();
    if (!m_htim.delay_until(m_echo, LOW, 500))
    {
        osMutexRelease(ptimerMutexHandle);
        return -1;
    }

    uint32_t elapsed_ms = m_htim.elapsed_us() / 1e3;
    int16_t  distance   = (int16_t) ((SPEED_OF_SOUND * elapsed_ms) / 2);

    osMutexRelease(ptimerMutexHandle);

    return distance;
}
