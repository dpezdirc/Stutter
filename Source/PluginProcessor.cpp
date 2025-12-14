#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr double BUFFER_LENGTH_SECONDS = 1.0;

//------------------------------------------------------------------------------
StutterAudioProcessor::StutterAudioProcessor() :
	AudioProcessor
	(
		BusesProperties()
			.withInput("Input", juce::AudioChannelSet::stereo(), true)
			.withOutput("Output", juce::AudioChannelSet::stereo(), true)
	)
{
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
	m_buffer = FixedBuffer(static_cast<int>(BUFFER_LENGTH_SECONDS * sampleRate));
}

//------------------------------------------------------------------------------
void StutterAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	const int nInputChannels = getTotalNumInputChannels();
	const int nOutputChannels = getTotalNumOutputChannels();

	for (int iChan = 0; iChan < nInputChannels; ++iChan)
	{
		auto* channelData = buffer.getWritePointer(iChan);

		// ..do something to the data...
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
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new StutterAudioProcessor();
}