/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <BinaryData.h>

using juce::Slider;
using juce::ToggleButton;

//==============================================================================
/**
*/
class GuitarCrusherAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener, public Button::Listener
{
public:
    GuitarCrusherAudioProcessorEditor (GuitarCrusherAudioProcessor&);
    ~GuitarCrusherAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override;
    void buttonClicked(Button*) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuitarCrusherAudioProcessor& audioProcessor;
    
    Slider gainSlider;
    Slider distSlider;
    Slider bitSlider;
    Slider downSampleSlider;
    Slider drywetSlider;
    
    Slider inputSlider;
    Slider outputSlider;
    
    ToggleButton gainButton;
    ToggleButton distButton;
    ToggleButton crushButton;
    ToggleButton downSampleButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarCrusherAudioProcessorEditor)
};
