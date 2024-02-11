/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using juce::Slider;

//==============================================================================
/**
*/
class GuitarCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener
{
public:
    GuitarCrusherAudioProcessorEditor (GuitarCrusherAudioProcessor&);
    ~GuitarCrusherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider *slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarCrusherAudioProcessor& audioProcessor;
    
    Slider gainSlider;
    Slider bitSlider;
    Slider downSampleSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarCrusherAudioProcessorEditor)
};
