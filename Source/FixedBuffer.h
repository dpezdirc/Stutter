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
	float GetSample(int idx);
	int GetSize() { return m_size; }

private:
	bool IsFull() const;

private:
	std::vector<float> m_data;
	int m_size = 0;
	int m_writeIdx = 0;
};

class RepeatingBuffer
{
public:
	RepeatingBuffer(int maxSize, int size);
	RepeatingBuffer() : RepeatingBuffer(0, 0) {}

	void Reset();
	void SetSize(int newSize);
	void WriteSample(float sample);
	void WriteSamples(const float* samples, int nSamples);
	float ReadSample();
	bool IsReadPositionAtStart() { return m_readIdx == 0; }

private:
	FixedBuffer m_fixedBuffer;
	int m_size = 0;
	int m_readIdx = 0;
};
