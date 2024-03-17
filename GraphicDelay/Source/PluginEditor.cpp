/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicDelayAudioProcessorEditor::GraphicDelayAudioProcessorEditor (GraphicDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setRotarySliderStyle(delayTimeSlider, "Time", 0.0f, 1000.0f);
    delayTimeSlider.setValue(audioProcessor.delayTime);
    
    setRotarySliderStyle(amountDelaySlider, "Amount", 0.0f, 1.0f);
    amountDelaySlider.setValue(audioProcessor.amountDelay);
    
    reverseButton.setName("Reverse");
    reverseButton.setToggleState(false, NotificationType::dontSendNotification);
    reverseButton.setClickingTogglesState(true);
    reverseButton.setColour(0x1006502, Colours::red);
    reverseButton.setColour(0x1006503, Colours::black);
    addAndMakeVisible(&reverseButton);
    reverseButton.addListener(this);
    
    setSize (400, 300);
}

GraphicDelayAudioProcessorEditor::~GraphicDelayAudioProcessorEditor()
{}

void GraphicDelayAudioProcessorEditor::setRotarySliderStyle(Slider& s, String name, double rangeStart, double rangeEnd)
{
    s.setName(name);
    s.setSliderStyle(Slider::SliderStyle::Rotary);
    
    s.setColour(0x1001400, Colours::black);
    s.setColour(0x1001700, Colours::transparentWhite);
    
    s.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 100, 20);
    
    s.setRange(rangeStart, rangeEnd);
    
    s.addListener(this);
    
    addAndMakeVisible(&s);
}

//==============================================================================
void GraphicDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::white);
    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    
    int wText = 100;
    int hText = 20;
    
    // knobs
    setRotarySliderText(g, delayTimeSlider, wText, hText);
    setRotarySliderText(g, amountDelaySlider, wText, hText);
}

void GraphicDelayAudioProcessorEditor::setRotarySliderText (Graphics& g, Slider& s, int wText, int hText)
{
    g.drawText(s.getName(),
               s.getX() + s.getWidth()/2 - wText/2,
               s.getY() + s.getHeight(),
               wText,
               hText,
               Justification::centred);
}

void GraphicDelayAudioProcessorEditor::resized()
{
    delayTimeSlider.setBounds(0, 0, 100, 100);
    amountDelaySlider.setBounds(100, 0, 100, 100);
    reverseButton.setBounds(235, 35, 30, 30);
}

void GraphicDelayAudioProcessorEditor::sliderValueChanged (Slider *slider)
{
    if (slider == &delayTimeSlider)
        audioProcessor.delayTime = delayTimeSlider.getValue();
    else if (slider == &amountDelaySlider)
        audioProcessor.amountDelay = amountDelaySlider.getValue();
}

void GraphicDelayAudioProcessorEditor::buttonClicked (Button *button)
{
    if (button == &reverseButton)
    {
        audioProcessor.reverseDelay = reverseButton.getToggleState();
    }
}
