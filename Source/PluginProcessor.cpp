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
			std::make_unique<juce::AudioParameterFloat>("repeatTime", "Repeat Time [ms]", juce::NormalisableRange<float>(0.1f, 1000.f, 0.01f, 1.f), 500.f),
			std::make_unique<juce::AudioParameterBool>("enable", "Enable", true)
		})
{
	m_pParamRepeatTime = dynamic_cast<juce::AudioParameterFloat*>(m_params.getParameter("repeatTime"));
	assert(m_pParamRepeatTime);
	m_pParamEnable = dynamic_cast<juce::AudioParameterBool*>(m_params.getParameter("enable"));
	assert(m_pParamEnable);
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

	const int nInputChannels = getTotalNumInputChannels();
	const int nOutputChannels = getTotalNumOutputChannels();
	const int nSamples = buffer.getNumSamples();

	const bool enable = m_pParamEnable->get();
	const bool enableLast = m_enableLast;
	m_enableLast = enable;

	if (!enable)
	{
		if (enableLast)
		{
			m_repeatingBuffer.Reset();
		}
		return;
	}

	m_repeatingBuffer.SetSize(GetRepeatLengthSamples());

	for (int iChan = 0; iChan < std::min(nOutputChannels, 2); ++iChan)
	{
		float* pDst = buffer.getWritePointer(iChan);

		// TODO: remove after stereo support
		if (iChan == 1)
		{
			const float* pSrc = buffer.getReadPointer(0);
			memcpy(pDst, pSrc, nSamples * sizeof(float));
			break;
		}

		m_repeatingBuffer.WriteSamples(pDst, nSamples);

		for (int n = 0; n < nSamples; ++n)
		{
			pDst[n] = m_repeatingBuffer.ReadSample();
		}
	}
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
int StutterAudioProcessor::GetRepeatLengthSamples()
{
	return static_cast<int>(m_pParamRepeatTime->get() * 0.001 * getSampleRate());
}

//------------------------------------------------------------------------------
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new StutterAudioProcessor();
}