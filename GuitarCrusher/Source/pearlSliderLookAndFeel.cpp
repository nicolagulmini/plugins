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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    knobRelease = ImageCache::getFromMemory(BinaryData::knobs_png, BinaryData::knobs_pngSize);
}

