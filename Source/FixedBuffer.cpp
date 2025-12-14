#include "FixedBuffer.h"
#include <cassert>

FixedBuffer::FixedBuffer(int size) :
    m_size(size)
{
    m_data.resize(size);
    Reset();
}

void FixedBuffer::Reset()
{
    m_readIdx = 0;
    m_writeIdx = 0;
}

void FixedBuffer::WriteSample(float sample)
{
    assert(m_writeIdx <= m_size);

    if (IsFull())
        return;

    m_data[m_writeIdx] = sample;
    ++m_writeIdx;
}

void FixedBuffer::WriteSamples(const float* samples, int nSamples)
{
    for (int i = 0; i < nSamples; ++i)
    {
        WriteSample(samples[i]);
    }
}

float FixedBuffer::ReadSample()
{
    const float sample = m_data[m_readIdx];

    m_readIdx = (m_readIdx + 1) % m_size;

    return sample;
}

bool FixedBuffer::IsFull() const
{
    return m_writeIdx == m_size;
}
