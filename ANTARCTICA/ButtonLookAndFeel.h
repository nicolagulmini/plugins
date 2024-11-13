#pragma once

#include <JuceHeader.h>
class ButtonLookAndFeel  : public LookAndFeel_V4
{
public:
    ButtonLookAndFeel();
    void drawToggleButton(Graphics &, ToggleButton &, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    Image onButton;
    Image offButton;
    //Image clickedOn;
    //Image clickedOff;
};
