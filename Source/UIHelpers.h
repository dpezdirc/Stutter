#pragma once

#include <JuceHeader.h>

#include <memory>

//------------------------------------------------------------------------------
struct ComponentWithLabelBase
{
	virtual ~ComponentWithLabelBase() {}
	virtual void setBounds(int x, int y, int w, int h) = 0;

	juce::Label m_label;
};

//------------------------------------------------------------------------------
template <typename TComponent, typename TAttachment>
struct ComponentWithLabel : ComponentWithLabelBase
{
	ComponentWithLabel()
	{
		m_label.attachToComponent(&m_component, true);
	}

	void setBounds(int x, int y, int w, int h) override
	{
		m_component.setBounds(x, y, w, h);
	}

	TComponent m_component;
	std::unique_ptr<TAttachment> attachment;
};

//------------------------------------------------------------------------------
using ButtonWithLabel = ComponentWithLabel<juce::ToggleButton, juce::AudioProcessorValueTreeState::SliderAttachment>;
using SliderWithLabel = ComponentWithLabel<juce::Slider, juce::AudioProcessorValueTreeState::SliderAttachment>;

//------------------------------------------------------------------------------
template <typename TOwner, typename TComponent>
void CreateAndAddComponent(TOwner* owner, const juce::String& label, const juce::String& tooltip)
{
	TComponent* newComponent = new TComponent();
	newComponent->m_label.setText(label, dontSendNotification);
	newComponent->m_label.setTooltip(tooltip);
	owner->AddComponent(newComponent);
	owner->addAndMakeVisible(newComponent->m_component);
}
