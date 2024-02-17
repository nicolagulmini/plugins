/*
  ==============================================================================

    SinPlot.cpp
    Created: 16 Feb 2024 10:59:42pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#include "SinPlot.h"
#include <JuceHeader.h>

//==============================================================================
void SinPlot::paint (Graphics &g)
{
    //g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2, 0.5f);
    
    float firstPoint = wave(0);
    
    for (int i = 1; i < getWidth(); ++i)
    {
        float secondPoint = wave(i);
            
        if (bitCrusher)
        {
            firstPoint -= fmodf(firstPoint, -getHeight()/((32-bit)));
            secondPoint -= fmodf(secondPoint, -getHeight()/((32-bit)));
        }
        
        if (downSample)
            if ((i-1)%int(getWidth()*downSampleValue/100) != 0)
                secondPoint = firstPoint;
            
        
        g.setColour(Colours::white);
        g.setOpacity(percentageDryWet);
        g.drawLine(i-1, firstPoint, i, secondPoint, 2);
        
        g.setColour(Colours::grey);
        g.drawLine(i-1, firstPoint*percentageDryWet+wave(i-1)*(1-percentageDryWet), i, secondPoint*percentageDryWet+wave(i)*(1-percentageDryWet), 2);
        
        g.setColour(Colours::darkgrey);
        g.setOpacity(1-percentageDryWet);
        g.drawLine(i-1, wave(i-1), i, wave(i), 2);
        firstPoint = secondPoint;
    }
}

float SinPlot::wave(int x)
{
    return getHeight()/2*(1-sin(2*M_PI/getWidth()*x));
}
