/*
  ==============================================================================

    RedSwitcher.cpp
    Created: 14 Feb 2024 10:35:13pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#include "RedSwitcher.h"
#include <JuceHeader.h>

//==============================================================================
RedSwitcher::RedSwitcher() : ToggleButton()
{
    setLookAndFeel(&buttonLookAndFeel);
}

RedSwitcher::~RedSwitcher()
{
    setLookAndFeel(nullptr);
}
