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

float FixedBuffer::GetSample(int idx)
{
    assert(idx >= 0 && idx < m_size);

    return m_data[idx];
}

bool FixedBuffer::IsFull() const
{
    return m_writeIdx == m_size;
}

RepeatingBuffer::RepeatingBuffer(int maxSize, int size) :
	m_fixedBuffer(maxSize),
	m_size(size)
{
}

void RepeatingBuffer::Reset()
{
	m_fixedBuffer.Reset();
	m_readIdx = 0;
}

void RepeatingBuffer::SetSize(int newSize)
{
    assert(newSize > 0 && newSize <= m_fixedBuffer.GetSize());

    m_size = newSize;

    if (m_readIdx >= m_size)
        m_readIdx = 0;
}

void RepeatingBuffer::WriteSample(float sample)
{
	m_fixedBuffer.WriteSample(sample);
}

void RepeatingBuffer::WriteSamples(const float* samples, int nSamples)
{
	m_fixedBuffer.WriteSamples(samples, nSamples);
}

float RepeatingBuffer::ReadSample()
{
	const float sample = m_fixedBuffer.GetSample(m_readIdx);

	m_readIdx = (m_readIdx + 1) % m_size;

	return sample;
}
