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
    g.drawRect(0, 0, s, s);
    g.drawLine(0, getHeight()/2, getWidth(), getHeight()/2);
    for (int i = 0; i < getWidth()-1; ++i)
        g.drawLine(i, wave(i), i+1, wave(i+1));
}

float SinPlot::wave(int x)
{
    return getHeight()/2-(getHeight()/2-1)*sin(2*M_PI/getWidth()*x);
    
    /*
    if (bitSwitch)
        toProcessVal -= fmodf(toProcessVal, pow(2, -(pow(1.1117,32-bitVal)+1)));
    
    finalVal = toProcessVal;
    
    if (downSampleSwitch)
    {
        int stepIndex = sample%int(buffer.getNumSamples()*pow(1.08, downSampleVal)/100);
        if (stepIndex != 0)
            finalVal = channelData[sample - stepIndex];
    }
                
    channelData[sample] = outputVal*(finalVal*(drywetPercentageVal/100)+backupVal*(1-drywetPercentageVal/100));
     */
}
