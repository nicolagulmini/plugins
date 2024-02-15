/*
  ==============================================================================

    ButtonLookAndFeel.cpp
    Created: 14 Feb 2024 10:35:39pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ButtonLookAndFeel.h"

ButtonLookAndFeel::ButtonLookAndFeel() : LookAndFeel_V4()
{
    onButton = ImageCache::getFromMemory(BinaryData::onButton_png, BinaryData::onButton_pngSize);
    offButton = ImageCache::getFromMemory(BinaryData::offButton_png, BinaryData::offButton_pngSize);
}

void ButtonLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& b, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (shouldDrawButtonAsHighlighted)
        g.drawImage(onButton, Rectangle<float>(0.0f, 0.0f, 25.0f, 25.0f));
    else
        g.drawImage(offButton, Rectangle<float>(0.0f, 0.0f, 25.0f, 25.0f));
}
