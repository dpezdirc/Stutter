//------------------------------------------------------------------------------
//  Code for the plugin's audio processing logic
//------------------------------------------------------------------------------

#pragma once

#include "FixedBuffer.h"

#include <JuceHeader.h>

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
	bool hasEditor() const override { return true; }
	bool acceptsMidi() const override { return false; }
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
	FixedBuffer m_buffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StutterAudioProcessor)
};
