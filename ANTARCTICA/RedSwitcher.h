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
