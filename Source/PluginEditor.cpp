#include "PluginProcessor.h"
#include "PluginEditor.h"

//------------------------------------------------------------------------------
StutterAudioProcessorEditor::StutterAudioProcessorEditor (StutterAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

//------------------------------------------------------------------------------
void StutterAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

//------------------------------------------------------------------------------
void StutterAudioProcessorEditor::resized()
{
}
