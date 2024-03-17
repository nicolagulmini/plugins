/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GraphicDelayAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener, public ToggleButton::Listener
{
public:
    GraphicDelayAudioProcessorEditor (GraphicDelayAudioProcessor&);
    ~GraphicDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override;
    void buttonClicked(Button*) override;

private:
    void setRotarySliderStyle(Slider& s, String name, double rangeStart, double rangeEnd);
    void setRotarySliderText (Graphics& g, Slider& s, int wText, int hText);
    
    GraphicDelayAudioProcessor& audioProcessor;
    
    Slider delayTimeSlider;
    Slider amountDelaySlider;
    
    ToggleButton reverseButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicDelayAudioProcessorEditor)
};
