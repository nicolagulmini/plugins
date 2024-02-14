/*
  ==============================================================================

    pearlSliderLookAndFeel.cpp
    Created: 13 Feb 2024 3:32:46pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#include <JuceHeader.h>
#include "pearlSliderLookAndFeel.h"

//==============================================================================
pearlSliderLookAndFeel::pearlSliderLookAndFeel() : LookAndFeel_V4()
{
    insideFill = ImageCache::getFromMemory(BinaryData::pearl_png, BinaryData::pearl_pngSize);
    outsideGear = ImageCache::getFromMemory(BinaryData::knobs_png, BinaryData::knobs_pngSize);
}

void pearlSliderLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float pivotX = outsideGear.getWidth()/2.0;
    const float pivotY = outsideGear.getHeight()/2.0;
    g.drawImageTransformed(insideFill, AffineTransform::rotation(0, pivotX, pivotY));
    g.drawImageTransformed(outsideGear, AffineTransform::rotation(angle, pivotX, pivotY));
}
