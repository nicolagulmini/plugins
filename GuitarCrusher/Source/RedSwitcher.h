/*
  ==============================================================================

    RedSwitcher.h
    Created: 14 Feb 2024 10:35:13pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ButtonLookAndFeel.h"

//==============================================================================
/*
*/
class RedSwitcher  : public juce::ToggleButton
{
public:
    RedSwitcher();
    ~RedSwitcher();

private:
    
    ButtonLookAndFeel buttonLookAndFeel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RedSwitcher);
};
