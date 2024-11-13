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
