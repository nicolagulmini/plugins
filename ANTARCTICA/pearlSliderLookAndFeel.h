#pragma once

#include <JuceHeader.h>

class pearlSliderLookAndFeel  : public LookAndFeel_V4
{
public:
    pearlSliderLookAndFeel();

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle, Slider& slider) override;
    Label* createSliderTextBox(Slider&) override;

private:
    Image outsideGear;
    Image insideFill;
    Image inactiveFill;
    Image inactiveGear;
    Image goldThumb;
};
