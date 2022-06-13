#pragma once

#include <array>

template <typename Ty, std::size_t N = 8, std::enable_if_t<std::is_arithmetic<Ty>::value, bool> = true>
class Average
{
private:
    std::array<Ty, N> m_data;
    Ty                m_ave;

    void update()
    {
        Ty sum = 0;
        for (auto &e : m_data)
        {
            sum += e;
        }
        m_ave = sum / N;
    }
protected:
public:
    int index = 0;
    Average(){};

    void push(const Ty &e)
    {
        if (index >= N) index = 0;
        m_data[index++] = e;
        update();
    }

    Ty get() { return m_ave; }
};