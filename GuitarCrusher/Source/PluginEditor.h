/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <BinaryData.h>
#include "pearlSlider.h"
#include "RedSwitcher.h"

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
    
    pearlSlider gainSlider;
    pearlSlider distSlider;
    pearlSlider bitSlider;
    pearlSlider downSampleSlider;
    pearlSlider drywetSlider;
    
    pearlSlider inputSlider;
    pearlSlider outputSlider;
    
    RedSwitcher gainButton;
    RedSwitcher distButton;
    RedSwitcher crushButton;
    RedSwitcher downSampleButton;
    
    Rectangle<int> localRectangle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuitarCrusherAudioProcessorEditor)
};
