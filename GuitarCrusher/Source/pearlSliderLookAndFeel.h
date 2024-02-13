/*
  ==============================================================================

    pearlSliderLookAndFeel.h
    Created: 13 Feb 2024 3:32:46pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class pearlSliderLookAndFeel  : public LookAndFeel_V4
{
public:
    pearlSliderLookAndFeel();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    void drawLabel(Graphics& g, Label& label) override;

private:
    Image knobRelease;
};
