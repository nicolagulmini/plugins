#pragma once

#include <JuceHeader.h>
#include "pearlSliderLookAndFeel.h"

class pearlSlider  : public juce::Slider
{
public:
    pearlSlider();
    ~pearlSlider();

    //void mouseDown(const MouseEvent& event) override;
    //void mouseUp(const MouseEvent& event) override;

private:
    
    pearlSliderLookAndFeel pearlSliderLookAndFeel;
    //Point<int> mousePosition;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pearlSlider);
};
