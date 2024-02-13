/*
  ==============================================================================

    pearlSlider.cpp
    Created: 13 Feb 2024 3:32:04pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#include <JuceHeader.h>
#include "pearlSlider.h"

//==============================================================================
pearlSlider::pearlSlider() : Slider()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setLookAndFeel(&pearlSliderLookAndFeel);
}

pearlSlider::~pearlSlider()
{
    setLookAndFeel(nullptr);
}

void pearlSlider::mouseDown(const MouseEvent& event)
{
    Slider::mouseDown(event);
    setMouseCursor(MouseCursor::NoCursor);
    mousePosition = Desktop::getMousePosition();
}

void pearlSlider::mouseUp(const MouseEvent& event)
{
    Slider::mouseUp(event);
    Desktop::setMousePosition(mousePosition);
    setMouseCursor(MouseCursor::NormalCursor);
}
