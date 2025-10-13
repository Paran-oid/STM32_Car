#include "irremote.hpp"

uint8_t IRRemote::read_byte()
{
    uint8_t res = 0;
    uint8_t bit;
    for (uint8_t i = 0; i < 8; i++)
    {
        while (m_gpio.get() != HIGH);

        if (m_htim.delay_until(m_gpio, LOW, 620))
            bit = 0;
        else
            bit = 1;

        res |= (bit << i);
    }

    return res;
}

IRRemoteEntry IRRemote::receive()
{
    // transmission start
    m_gpio.set_mode(INPUT);
    while (m_gpio.get() == HIGH);

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
