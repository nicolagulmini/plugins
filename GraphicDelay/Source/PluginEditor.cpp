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
    delayTimeSlider.setName("Time");
    delayTimeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    delayTimeSlider.setRange(0.0f, 1000.0f); // from 0 to 1 second
    delayTimeSlider.setValue(audioProcessor.delayTime);
    addAndMakeVisible(&delayTimeSlider);
    delayTimeSlider.addListener(this);
    
    amountDelaySlider.setName("Amount");
    amountDelaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    amountDelaySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    amountDelaySlider.setRange(0.0f, 1.0f); // from 0 to 1 second
    amountDelaySlider.setValue(audioProcessor.delayTime);
    addAndMakeVisible(&amountDelaySlider);
    amountDelaySlider.addListener(this);
    
    setSize (400, 300);
}

GraphicDelayAudioProcessorEditor::~GraphicDelayAudioProcessorEditor()
{
}

//==============================================================================
void GraphicDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void GraphicDelayAudioProcessorEditor::resized()
{
    delayTimeSlider.setBounds(0, 0, 100, 100);
    amountDelaySlider.setBounds(100, 0, 100, 100);
}

void GraphicDelayAudioProcessorEditor::sliderValueChanged (Slider *slider)
{
    if (slider == &delayTimeSlider)
    {
        audioProcessor.delayTime = delayTimeSlider.getValue();
    }
    else if (slider == &amountDelaySlider)
    {
        audioProcessor.amountDelay = amountDelaySlider.getValue();
    }
}
