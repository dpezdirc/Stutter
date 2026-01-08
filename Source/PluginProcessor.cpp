#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cassert>

constexpr double BUFFER_LENGTH_SECONDS = 1.0;

//------------------------------------------------------------------------------
StutterAudioProcessor::StutterAudioProcessor() :
	AudioProcessor
	(
		BusesProperties()
			.withInput("Input", juce::AudioChannelSet::stereo(), true)
			.withOutput("Output", juce::AudioChannelSet::stereo(), true)
	),
	m_params(*this, nullptr, juce::Identifier("StutterParams"),
		{
			std::make_unique<juce::AudioParameterFloat>("repeatTime", "Repeat Time [ms]", juce::NormalisableRange<float>(0.1f, 1000.f, 0.01f, 1.f), 500.f)
		})
{
	m_pParamRepeatTime = dynamic_cast<juce::AudioParameterFloat*>(m_params.getParameter("repeatTime"));
	assert(m_pParamRepeatTime);
}

//------------------------------------------------------------------------------
bool StutterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	// support mono or stereo output
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// check if the input layout matches the output layout
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;

	return true;
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	m_repeatingBuffer = RepeatingBuffer(static_cast<int>(BUFFER_LENGTH_SECONDS * sampleRate), GetRepeatLengthSamples());
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	const int nSamples = buffer.getNumSamples();

	const bool midiMsgThisFrame = !midiMessages.isEmpty();
	const int midiMsgSamplePos = midiMsgThisFrame ? (*midiMessages.begin()).samplePosition : -1;

	if (!m_enable && !midiMsgThisFrame)
		return;

	m_repeatingBuffer.SetSize(GetRepeatLengthSamples());

	if (midiMsgThisFrame)
	{
		ProcessBlockInternal(buffer, 0, midiMsgSamplePos);

		const juce::MidiMessage msg = (*midiMessages.begin()).getMessage();
		ApplyMidiMessage(msg);

		const int nRemainingSamples = nSamples - midiMsgSamplePos;

		ProcessBlockInternal(buffer, midiMsgSamplePos, nRemainingSamples);
	}
	else
	{
		ProcessBlockInternal(buffer, 0, nSamples);
	}
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::ProcessBlockInternal(juce::AudioBuffer<float>& buffer, int idxStart, int nSamples)
{
	assert(idxStart + nSamples <= buffer.getNumSamples());

	if (!m_enable)
		return;

	const int nOutputChannels = std::min(getTotalNumOutputChannels(), 2);

	for (int iChan = 0; iChan < nOutputChannels; ++iChan)
	{
		float* pDst = buffer.getWritePointer(iChan);

		// TODO: remove after stereo support
		if (iChan == 1)
		{
			const float* pSrc = buffer.getReadPointer(0);
			memcpy(pDst, pSrc, nSamples * sizeof(float));
			break;
		}

		for (int n = 0; n < nSamples; ++n)
		{
			m_repeatingBuffer.WriteSample(pDst[n]);
			pDst[n] = m_repeatingBuffer.ReadSample();
		}
	}
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::ApplyMidiMessage(const juce::MidiMessage& midiMessage)
{
	if (midiMessage.isNoteOn())
	{
		m_enable = true;
	}
	else if (midiMessage.isNoteOff())
	{
		m_enable = false;
		m_repeatingBuffer.Reset();
	}
}

//------------------------------------------------------------------------------
int StutterAudioProcessor::GetRepeatLengthSamples()
{
	return static_cast<int>(m_pParamRepeatTime->get() * 0.001 * getSampleRate());
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

//------------------------------------------------------------------------------
juce::AudioProcessorEditor* StutterAudioProcessor::createEditor()
{
	return new StutterAudioProcessorEditor(*this);
}

//------------------------------------------------------------------------------
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new StutterAudioProcessor();
}