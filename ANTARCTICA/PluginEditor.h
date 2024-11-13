#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <BinaryData.h>
#include "pearlSlider.h"
#include "RedSwitcher.h"

#define GAIN_ID                  "gain"
#define GAIN_NAME                "Gain"
#define DRIVE_ID                 "drive"
#define DRIVE_NAME               "Drive"
#define BIT_ID                   "bitcrusher"
#define BIT_NAME                 "Bit"
#define DWNSMP_ID                "downsample"
#define DWNSMP_NAME              "Downsample"
#define DRYWET_ID                "drywet"
#define DRYWET_NAME              "Dry/Wet"
#define INPUT_ID                 "input"
#define INPUT_NAME               "Input"
#define OUTPUT_ID                "output"
#define OUTPUT_NAME              "Output"
#define DELAYAMOUNT_ID           "delayamount"
#define DELAYAMOUNT_NAME         "Tail Amount"
#define DELAYTIME_ID             "delaytime"
#define DELAYTIME_NAME           "Tail Length"

#define RND_INTERVAL_ID          "rnd_interval"
#define RND_INTERVAL_NAME        "Random Interval"
#define RND_DURATION_ID          "rnd_duration"
#define RND_DURATION_NAME        "Random Duration"
     
// new sliders
#define CLIPPER_ID               "clipper"
#define CLIPPER_NAME             "Clipper"
#define SATURATION_ID            "saturation"
#define SATURATION_NAME          "Saturation"
#define FADEIN_ID                "fade_in"
#define FADEIN_NAME              "Fade In"
#define FADEOUT_ID               "fade_out"
#define FADEOUT_NAME             "Fade Out"
#define ANTIALIASING_ID          "anti_aliasing"
#define ANTIALIASING_NAME        "Anti Aliasing"

// buttons
#define GAIN_BTN_ID              "gain_btn"
#define GAIN_BTN_NAME            "Gain Button"
#define DRIVE_BTN_ID             "drive_btn"
#define DRIVE_BTN_NAME           "Drive Button"
#define BIT_BTN_ID               "bitcrusher_btn"
#define BIT_BTN_NAME             "Bit Button"
#define DWNSMP_BTN_ID            "downsample_btn"
#define DWNSMP_BTN_NAME          "Downsample Button"

#define BYPASS_BTN_ID            "bypass_btn"
#define BYPASS_BTN_NAME          "Bypass Button"
#define RANDOM_BTN_ID            "random_btn"
#define RANDOM_BTN_NAME          "Random Button"
#define FLUTTER_BTN_ID           "flutter_btn"
#define FLUTTER_BTN_NAME         "Split Channels"
#define REV_BTN_ID               "reverse_btn"
#define REV_BTN_NAME             "Reverse Buffer"
#define TAIL_BTN_ID              "tail_btn"
#define TAIL_BTN_NAME            "Tail Button"

// new buttons
#define FADE_TYPE_BTN_ID         "fade_type_btn"
#define FADE_TYPE_BTN_NAME       "Linear / Cosine Fade"
#define GAIN_MATCHING_BTN_ID     "gain_matching_btn"
#define GAIN_MATCHING_BTN_NAME   "Gain Matching"


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
    void setCustomButtonStyle(Button& s, String name, String id);
    
    pearlSlider gainSlider,
                distSlider,
                bitSlider,
                downSampleSlider,
                drywetSlider,
                delayAmountSlider,
                delayTimeSlider,
                rndIntervalSlider,
                rndDurationSlider,
                inputSlider,
                outputSlider,
                clipperSlider,
                saturationSlider,
                fadeinSlider,
                fadeoutSlider,
                antialiasingSlider;
    
    RedSwitcher gainButton,
                distButton,
                crushButton,
                downSampleButton,
                bypassButton,
                randomButton,
                flutterButton,
                reverseButton,
                tailButton,
                fadeButton,
                gainMatchingButton;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ANTARCTICAAudioProcessor& audioProcessor;
    
public:
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderValue,
                                                                    distSliderValue,
                                                                    bitSliderValue,
                                                                    downSampleSliderValue,
                                                                    drywetSliderValue,
                                                                    inputSliderValue,
                                                                    outputSliderValue,
                                                                    delayAmountSliderValue,
                                                                    delayTimeSliderValue,
                                                                    rndIntervalSliderValue,
                                                                    rndDurationSliderValue,
                                                                    clipperSliderValue,
                                                                    saturationSliderValue,
                                                                    fadeinSliderValue,
                                                                    fadeoutSliderValue,
                                                                    antialiasingSliderValue;
                                                                    
    std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> gainButtonValue,
                                                                    distButtonValue,
                                                                    crushButtonValue,
                                                                    downSampleButtonValue,
                                                                    bypassButtonValue,
                                                                    randomButtonValue,
                                                                    flutterButtonValue,
                                                                    reverseButtonValue,
                                                                    tailButtonValue,
                                                                    fadeButtonValue,
                                                                    gainMatchingButtonValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ANTARCTICAAudioProcessorEditor)
};
