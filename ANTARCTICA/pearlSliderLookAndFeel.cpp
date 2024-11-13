#include <JuceHeader.h>
#include "pearlSliderLookAndFeel.h"

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
    
    auto thumbX = 0;
    auto thumbY = 0;
    
    if (style == 0)
    {
        thumbX = sliderPos - thumbDim/2;
        thumbY = y + height/2 - thumbDim/2;
    }
    
    else if (style == 1)
    {
        thumbX = x + width/2 - thumbDim/2;
        thumbY = sliderPos - thumbDim/2;
    }
    
    
    drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    g.drawImage(goldThumb, thumbX, thumbY, thumbDim, thumbDim, 0, 0, goldThumb.getWidth(), goldThumb.getHeight());
}

Label* pearlSliderLookAndFeel::createSliderTextBox(Slider& s)
{
    Label* sliderPointer = LookAndFeel_V4::createSliderTextBox(s);
    sliderPointer->setColour(Label::textColourId, Colours::black);
    sliderPointer->setColour(Label::backgroundColourId, Colours::transparentWhite);
    sliderPointer->setColour(Label::outlineColourId, Colours::transparentWhite);
    return sliderPointer;
}
