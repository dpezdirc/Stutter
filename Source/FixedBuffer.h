#pragma once

#include <vector>

class FixedBuffer
{
public:
	FixedBuffer(int size);
	FixedBuffer() : FixedBuffer(0) {}

	void Reset();
	void WriteSample(float sample);
	void WriteSamples(const float* samples, int nSamples);
	float ReadSample();

private:
	bool IsFull() const;

private:
	std::vector<float> m_data;
	int m_size = 0;
	int m_readIdx = 0;
	int m_writeIdx = 0;
};
