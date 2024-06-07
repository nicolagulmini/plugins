/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

/*
 
 There are a couple of approaches to make delay time changes smoother :

 Fade the audio out when the delay time starts changing and fade it back when its settled.
 Interpolate through the delay buffer at a different speed to reach the new read head position corresponding to the new delay time. This is often used in “vintage” delay emulations. The interpolation should be high quality, plain linear interpolation probably isn’t going to work great.
 Just using a SmoothedValue for the delay time parameter likely isn’t going to work.
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
