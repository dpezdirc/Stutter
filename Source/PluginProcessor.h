//------------------------------------------------------------------------------
//  Code for the plugin's audio processing logic
//------------------------------------------------------------------------------

#pragma once

#include "FixedBuffer.h"

#include <JuceHeader.h>
#include <array>

#define PROVIDE_DEBUG_ENABLE_PARAM 1

//------------------------------------------------------------------------------
class StutterAudioProcessor : public juce::AudioProcessor
{
public:
	StutterAudioProcessor();
	~StutterAudioProcessor() override {};

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
	void releaseResources() override {};

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	juce::AudioProcessorEditor* createEditor() override;

	//--------------------------------------
	// Mandatory, one-liner overrides
	//--------------------------------------
	bool hasEditor() const override { return false; }
	bool acceptsMidi() const override { return true; }
	bool producesMidi() const override { return false; }
	bool isMidiEffect() const override { return false; }
	double getTailLengthSeconds() const override { return 0.0; }
	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	const juce::String getName() const override { return JucePlugin_Name; }
	const juce::String getProgramName(int index) override { return {}; }
	void changeProgramName(int index, const juce::String& newName) override {}
	void setCurrentProgram(int index) override {}

private:
	void ProcessBlockInternal(juce::AudioBuffer<float>& buffer, int idxStart, int nSamples);
	void ApplyMidiMessage(const juce::MidiMessage& midiMessage);
	int GetRepeatLengthSamples();

private:
	juce::AudioProcessorValueTreeState m_params;
	juce::AudioParameterFloat* m_pParamRepeatTime;
	std::array<RepeatingBuffer, 2> m_repeatingBuffersPerChannel;
	bool m_enable = false;

#if PROVIDE_DEBUG_ENABLE_PARAM
	juce::AudioParameterBool* m_pParamDebugEnable;
	bool m_debugEnableLast = false;
#endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StutterAudioProcessor)
};
