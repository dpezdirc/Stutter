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
#if PROVIDE_DEBUG_ENABLE_PARAM
			,std::make_unique<juce::AudioParameterBool>("enable", "Enable", true)
#endif
		})
{
	m_pParamRepeatTime = dynamic_cast<juce::AudioParameterFloat*>(m_params.getParameter("repeatTime"));
	assert(m_pParamRepeatTime);

#if PROVIDE_DEBUG_ENABLE_PARAM
	m_pParamDebugEnable = dynamic_cast<juce::AudioParameterBool*>(m_params.getParameter("enable"));
	assert(m_pParamDebugEnable);
#endif
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
	for (RepeatingBuffer& repeatingBuffer : m_repeatingBuffersPerChannel)
	{
		repeatingBuffer = RepeatingBuffer(static_cast<int>(BUFFER_LENGTH_SECONDS * sampleRate), GetRepeatLengthSamples());
	}
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

#if PROVIDE_DEBUG_ENABLE_PARAM
	const bool debugEnable = m_pParamDebugEnable->get();
	const bool debugEnableLast = m_debugEnableLast;
	m_debugEnableLast = debugEnable;

	if (debugEnable != debugEnableLast)
	{
		juce::MidiMessage debugMidiMsg;
		const int channel = 1;
		const int noteNumber = 1;
		const juce::uint8 velocity = 1;
		if (debugEnable)
			debugMidiMsg = juce::MidiMessage::noteOn(channel, noteNumber, velocity);
		else
			debugMidiMsg = juce::MidiMessage::noteOff(channel, noteNumber);
		midiMessages.addEvent(debugMidiMsg, 0);
	}
#endif

	const int nSamples = buffer.getNumSamples();

	const bool midiMsgThisFrame = !midiMessages.isEmpty();
	const int midiMsgSamplePos = midiMsgThisFrame ? (*midiMessages.begin()).samplePosition : -1;

	if (!m_enable && !midiMsgThisFrame)
		return;

	for (RepeatingBuffer& repeatingBuffer : m_repeatingBuffersPerChannel)
	{
		repeatingBuffer.SetSize(GetRepeatLengthSamples());
	}

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

		for (int n = 0; n < nSamples; ++n)
		{
			m_repeatingBuffersPerChannel[iChan].WriteSample(pDst[n]);
			pDst[n] = m_repeatingBuffersPerChannel[iChan].ReadSample();
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

		for (RepeatingBuffer& repeatingBuffer : m_repeatingBuffersPerChannel)
		{
			repeatingBuffer.Reset();
		}
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
	juce::ValueTree state = m_params.copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (!xmlState.get())
		return;

	if (!xmlState->hasTagName(m_params.state.getType()))
		return;

	m_params.replaceState(juce::ValueTree::fromXml(*xmlState));
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