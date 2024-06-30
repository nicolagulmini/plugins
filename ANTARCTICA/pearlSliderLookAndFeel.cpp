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
    
    drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    g.drawImage(goldThumb, thumbX, sliderPos-thumbDim/2, thumbDim, thumbDim, 0, 0, goldThumb.getWidth(), goldThumb.getHeight());
}

Label* pearlSliderLookAndFeel::createSliderTextBox(Slider& s)
{
    Label* sliderPointer = LookAndFeel_V4::createSliderTextBox(s);
    sliderPointer->setColour(Label::textColourId, Colours::black);
    sliderPointer->setColour(Label::backgroundColourId, Colours::transparentWhite);
    sliderPointer->setColour(Label::outlineColourId, Colours::transparentWhite);
    return sliderPointer;
}

/*
Slider::SliderLayout pearlSliderLookAndFeel::getSliderLayout(Slider& slider)
{

    // almost the same method from LookAndFeel_V2
    // https://github.com/juce-framework/JUCE/blob/master/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V2.cpp
    
    int minXSpace = 0;
    int minYSpace = 0;

    auto textBoxPos = slider.getTextBoxPosition();

    if (textBoxPos == Slider::TextBoxLeft || textBoxPos == Slider::TextBoxRight)
        minXSpace = 30;
    else
        minYSpace = 15;

    auto localBounds = slider.getLocalBounds();

    auto textBoxWidth  = jmax (0, jmin (slider.getTextBoxWidth(),  localBounds.getWidth() - minXSpace));
    auto textBoxHeight = jmax (0, jmin (slider.getTextBoxHeight(), localBounds.getHeight() - minYSpace));

    Slider::SliderLayout layout;

    // 2. set the textBox bounds

    if (textBoxPos != Slider::NoTextBox)
    {
        if (slider.isBar())
        {
            layout.textBoxBounds = localBounds;
        }
        else
        {
            layout.textBoxBounds.setWidth (textBoxWidth);
            layout.textBoxBounds.setHeight (textBoxHeight);

            if (textBoxPos == Slider::TextBoxLeft)           layout.textBoxBounds.setX (0);
            else if (textBoxPos == Slider::TextBoxRight)     layout.textBoxBounds.setX (localBounds.getWidth() - textBoxWidth);
            else                                             layout.textBoxBounds.setX ((localBounds.getWidth() - textBoxWidth) / 2);

            if (textBoxPos == Slider::TextBoxAbove)          layout.textBoxBounds.setY (-2*textBoxHeight); //outside local bounds!
            else if (textBoxPos == Slider::TextBoxBelow)     layout.textBoxBounds.setY (localBounds.getHeight() - 0.5*textBoxHeight); // my change // outside local bounds!
            else                                             layout.textBoxBounds.setY ((localBounds.getHeight() - textBoxHeight) / 2);
        }
    }

    // 3. set the slider bounds

    layout.sliderBounds = localBounds;

    if (slider.isBar())
    {
        layout.sliderBounds.reduce (1, 1);   // bar border
    }
    else
    {
        if (textBoxPos == Slider::TextBoxLeft)       layout.sliderBounds.removeFromLeft (textBoxWidth);
        else if (textBoxPos == Slider::TextBoxRight) layout.sliderBounds.removeFromRight (textBoxWidth);
        else if (textBoxPos == Slider::TextBoxAbove) layout.sliderBounds.removeFromTop (textBoxHeight);
        else if (textBoxPos == Slider::TextBoxBelow) layout.sliderBounds.removeFromBottom (textBoxHeight);

        const int thumbIndent = getSliderThumbRadius (slider);

        if (slider.isHorizontal())    layout.sliderBounds.reduce (thumbIndent, 0);
        else if (slider.isVertical()) layout.sliderBounds.reduce (0, thumbIndent);
    }

    return layout;
}
*/
