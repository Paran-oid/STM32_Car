#include "irremote.hpp"

#include "rtos.hpp"

uint8_t IRRemote::read_byte()
{
    uint8_t res = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        while (m_gpio.get_state() != sca::HIGH);

        m_htim.reset();

        while (m_gpio.get_state() != sca::LOW)
        {
            if (m_htim.elapsed_us() >= 2000) break;
        }

        uint32_t elapsed = m_htim.elapsed_us();

        if (elapsed > 1000)
        {
            res |= (1 << i);
        }
    }

    return res;
}

IRRemoteEntry IRRemote::retrieve()
{
    if (osMutexAcquire(HTIM3MutexHandle, osWaitForever) != osOK) return {0, 0, false};

    m_htim.reset();

    if (!(m_htim.delay_until(m_gpio, sca::HIGH, 9500)))
    {
        osMutexRelease(HTIM3MutexHandle);
        return {0, 0, false};  // Leader mark (~9ms)
    }
    if (!(m_htim.delay_until(m_gpio, sca::LOW, 4600)))
    {
        osMutexRelease(HTIM3MutexHandle);
        return {0, 0, false};  // Leader space (~4.5ms)
    }

    IRRemoteEntry entry = {0, 0, 0, 0, true};

    entry.addr     = read_byte();
    entry.addr_bar = read_byte();

    entry.data     = read_byte();
    entry.data_bar = read_byte();

    if (IR_repeat_check(entry))
    {
        osMutexRelease(HTIM3MutexHandle);
        return entry;
    }

    if ((entry.addr ^ entry.addr_bar) != 0xFF || (entry.data ^ entry.data_bar) != 0xFF)
    {
        osMutexRelease(HTIM3MutexHandle);
        return {0, 0, 0, 0, false};
    }

    osMutexRelease(HTIM3MutexHandle);
    return entry;
}
