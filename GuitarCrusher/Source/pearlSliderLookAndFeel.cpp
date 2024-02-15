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
    inactiveFill = ImageCache::getFromMemory(BinaryData::darkPearl_png, BinaryData::darkPearl_pngSize);
    inactiveGear = ImageCache::getFromMemory(BinaryData::darkGear_png, BinaryData::darkGear_pngSize);
    goldThumb = ImageCache::getFromMemory(BinaryData::goldThumb_png, BinaryData::goldThumb_pngSize);
}

void pearlSliderLookAndFeel::drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const float pivotX = outsideGear.getWidth()/2.0;
    const float pivotY = outsideGear.getHeight()/2.0;
    if (slider.isEnabled())
    {
        g.drawImageTransformed(insideFill, AffineTransform::rotation(0, pivotX, pivotY));
        g.drawImageTransformed(outsideGear, AffineTransform::rotation(angle, pivotX, pivotY));
    }
    else
    {
        g.drawImageTransformed(inactiveFill, AffineTransform::rotation(0, pivotX, pivotY));
        g.drawImageTransformed(inactiveGear, AffineTransform::rotation(angle, pivotX, pivotY));
    }
}

void pearlSliderLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, Slider::SliderStyle style, Slider& slider)
{
    auto thumbDim = 16;
    auto thumbX = x + width/2 - thumbDim/2;
    auto thumbY = sliderPos;
    
    drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    g.drawImage(goldThumb, thumbX, sliderPos-thumbDim/2, thumbDim, thumbDim, 0, 0, goldThumb.getWidth(), goldThumb.getHeight());
}
