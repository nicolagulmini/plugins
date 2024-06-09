#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <BinaryData.h>
#include "pearlSlider.h"
#include "RedSwitcher.h"
#include "SinPlot.h"

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define DRIVE_ID "drive"
#define DRIVE_NAME "Drive"
#define BIT_ID "bitcrusher"
#define BIT_NAME "Bit"
#define DWNSMP_ID "downsample"
#define DWNSMP_NAME "Downsample"
#define DRYWET_ID "drywet"
#define DRYWET_NAME "Dry/Wet"
#define INPUT_ID "input"
#define INPUT_NAME "Input"
#define OUTPUT_ID "output"
#define OUTPUT_NAME "Output"

#define GAIN_BTN_ID "gain_btn"
#define GAIN_BTN_NAME "Gain Button"
#define DRIVE_BTN_ID "drive_btn"
#define DRIVE_BTN_NAME "Drive Button"
#define BIT_BTN_ID "bitcrusher_btn"
#define BIT_BTN_NAME "Bit Button"
#define DWNSMP_BTN_ID "downsample_btn"
#define DWNSMP_BTN_NAME "Downsample Button"

using juce::Slider;
using juce::ToggleButton;

//==============================================================================
/**
*/
class ANTARCTICAAudioProcessorEditor  : public juce::AudioProcessorEditor, public AudioProcessorValueTreeState::Listener
{
public:
    ANTARCTICAAudioProcessorEditor (ANTARCTICAAudioProcessor&);
    ~ANTARCTICAAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setCustomSliderStyle(Slider& s, int type, String name);
    void setCustomButtonStyle(Button& s, String name);
    
    pearlSlider gainSlider;
    pearlSlider distSlider;
    pearlSlider bitSlider;
    pearlSlider downSampleSlider;
    pearlSlider drywetSlider;
    // dev
    pearlSlider preLowPass;
    
    pearlSlider inputSlider;
    pearlSlider outputSlider;
    
    RedSwitcher gainButton;
    RedSwitcher distButton;
    RedSwitcher crushButton;
    RedSwitcher downSampleButton;
    
    SinPlot sinPlot;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ANTARCTICAAudioProcessor& audioProcessor;
    
public:
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> distSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> bitSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> downSampleSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> drywetSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> inputSliderValue;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> outputSliderValue;
    
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> gainButtonValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> distButtonValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> crushButtonValue;
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> downSampleButtonValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ANTARCTICAAudioProcessorEditor)
};
