/*
  ==============================================================================

    SinPlot.h
    Created: 16 Feb 2024 10:59:42pm
    Author:  Nicola Gulmini

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SinPlot : public Component, public Timer
{
public:
    SinPlot() {startTimer(30);};
    
    void timerCallback() override
    { repaint();};
    void paint(Graphics &g) override;

    float wave(int x);
    
    void setIsBitCrushing(bool cond) {bitCrusher = cond;}
    bool isBitCrushing() {return bitCrusher;}
    void setIsDownSampling(bool cond) {downSample = cond;}
    bool isDownSampling() {return downSample;}
    void setBit(int newBit) {if (newBit >= 0 and newBit < 33) bit = newBit;}
    int getBit() {return bit;}
    void setDownSampleValue(float dwnsmpl) {if (dwnsmpl >= 1 and dwnsmpl <= 50) downSampleValue = dwnsmpl;}
    bool getDownSampleValue() {return downSampleValue;}
    void setPercentageDryWet(float percentage) {if (percentage >= 0.0f and percentage <= 1.0f) percentageDryWet = percentage;}
    
private:
    
    int s = 100;
    bool bitCrusher = true;
    bool downSample = true;
    int bit = 0;
    float downSampleValue = 1.0f;
    float percentageDryWet = 1.0f;
};
