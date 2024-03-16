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
class GraphicDelayAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener
{
public:
    GraphicDelayAudioProcessorEditor (GraphicDelayAudioProcessor&);
    ~GraphicDelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GraphicDelayAudioProcessor& audioProcessor;
    
    Slider delayTimeSlider;
    Slider amountDelaySlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphicDelayAudioProcessorEditor)
};
