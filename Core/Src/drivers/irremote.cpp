#include "irremote.hpp"

uint8_t IRRemote::read_byte()
{
    uint8_t res, bit;
    res = bit = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        while (m_gpio.get() != HIGH);

        m_htim.reset();
        while (m_gpio.get() != LOW)
        {
            if (m_htim.elapsed_us() > 2000)
            {
                break;
            }
        }

        uint32_t elapsed = m_htim.elapsed_us();

        if (elapsed > 1000)
        {
            res |= (1 << i);
        }
    }

    return res;
}

IRRemoteEntry IRRemote::receive()
{
    // transmission start
    if (!m_htim.delay_until(m_gpio, LOW, 100)) return {0, 0, false};

    m_htim.reset();

    if (!(m_htim.delay_until(m_gpio, HIGH, 9500))) return {0, 0, false};  // Leader mark (~9ms)
    if (!(m_htim.delay_until(m_gpio, LOW, 4600))) return {0, 0, false};   // Leader space (~4.5ms)

    // interrpret data bits
    uint8_t addr, addr_bar;
    uint8_t data, data_bar;

    addr     = read_byte();
    addr_bar = read_byte();
    if ((addr ^ addr_bar) != 0xFF) return {0, 0, false};

    data     = read_byte();
    data_bar = read_byte();
    if ((data ^ data_bar) != 0xFF) return {0, 0, false};

    return {addr, data, true};
}

bool IRRemote::refresh()
{
    return m_htim.delay_until(m_gpio, LOW, 100);
}
