#include <JuceHeader.h>
#include "ButtonLookAndFeel.h"

ButtonLookAndFeel::ButtonLookAndFeel() : LookAndFeel_V4()
{
    onButton = ImageCache::getFromMemory(BinaryData::onButton_png, BinaryData::onButton_pngSize);
    offButton = ImageCache::getFromMemory(BinaryData::offButton_png, BinaryData::offButton_pngSize);
    //clickedOn = ImageCache::getFromMemory(BinaryData::clickedButton_png, BinaryData::clickedButton_pngSize);
    //clickedOff = ImageCache::getFromMemory(BinaryData::clickedOff_png, BinaryData::clickedOff_pngSize);
}

void ButtonLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& b, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (shouldDrawButtonAsHighlighted or b.getToggleState())
        g.drawImage(onButton, Rectangle<float>(0.0f, 0.0f, 25.0f, 25.0f));
    else
        g.drawImage(offButton, Rectangle<float>(0.0f, 0.0f, 25.0f, 25.0f));
}
