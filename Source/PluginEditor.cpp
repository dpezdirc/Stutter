#include "PluginProcessor.h"
#include "PluginEditor.h"

constexpr int MARGIN_WIDTH = 10;
constexpr int SLIDER_HEIGHT = 20;

using namespace juce;

//------------------------------------------------------------------------------
StutterAudioProcessorEditor::StutterAudioProcessorEditor(StutterAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p)
{
	CreateAndAddComponent<StutterAudioProcessorEditor, ButtonWithLabel>(this, "Enabled", "");
	CreateAndAddComponent<StutterAudioProcessorEditor, SliderWithLabel>(this, "Repeat Time", "");
	
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
	// get the width of the widest component label
	int maxLabelWidth = 0;
	for (auto& pComponent : m_components)
	{
		const int labelWidth = pComponent->m_label.getFont().getStringWidth(pComponent->m_label.getText()) + pComponent->m_label.getBorderSize().getLeftAndRight();
		maxLabelWidth = std::max(labelWidth, maxLabelWidth);
	}

	// position the components
	for (int i = 0; i < m_components.size(); i++)
	{
		const int x = maxLabelWidth + MARGIN_WIDTH;
		const int y = i * SLIDER_HEIGHT + MARGIN_WIDTH;
		const int w = getWidth() - maxLabelWidth - 2 * MARGIN_WIDTH;
		const int h = SLIDER_HEIGHT;

		m_components[i]->setBounds(x, y, w, h);
	}
}

void StutterAudioProcessorEditor::AddComponent(ComponentWithLabelBase* newComponent)
{
	m_components.emplace_back(newComponent);
}
