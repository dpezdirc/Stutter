//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//------------------------------------------------------------------------------
class StutterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	StutterAudioProcessorEditor(StutterAudioProcessor&);
	~StutterAudioProcessorEditor() override {}

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	StutterAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StutterAudioProcessorEditor)
};
