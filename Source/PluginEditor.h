//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UIHelpers.h"

#include <vector>

//------------------------------------------------------------------------------
class StutterAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	StutterAudioProcessorEditor(StutterAudioProcessor&);
	~StutterAudioProcessorEditor() override {}

	void paint(juce::Graphics&) override;
	void resized() override;

	void AddComponent(ComponentWithLabelBase* newComponent);

private:
	StutterAudioProcessor& audioProcessor;

	std::vector<std::unique_ptr<ComponentWithLabelBase>> m_components;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StutterAudioProcessorEditor)
};
