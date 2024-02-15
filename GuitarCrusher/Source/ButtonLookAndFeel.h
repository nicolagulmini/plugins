/*
  ==============================================================================

    ButtonLookAndFeel.h
    Created: 14 Feb 2024 10:35:39pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ButtonLookAndFeel  : public LookAndFeel_V4
{
public:
    ButtonLookAndFeel();

    //void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    void drawToggleButton(Graphics &, ToggleButton &, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    Image onButton;
    Image offButton;
};
