/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarCrusherAudioProcessorEditor::GuitarCrusherAudioProcessorEditor (GuitarCrusherAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // knobs
    gainSlider.setName("Gain");
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    gainSlider.setRange(-6.0f, 12.0f);
    gainSlider.setValue(audioProcessor.gainVal);
    addAndMakeVisible(&gainSlider);
    gainSlider.addListener(this);
    
    distSlider.setName("Distortion");
    distSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    distSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    distSlider.setRange(1.0, 500.0f);
    distSlider.setValue(audioProcessor.distVal);
    addAndMakeVisible(&distSlider);
    distSlider.addListener(this);
    
    bitSlider.setName("Bit");
    bitSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    bitSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    bitSlider.setRange(0, 30, 0.1);
    bitSlider.setValue(audioProcessor.bitVal);
    addAndMakeVisible(&bitSlider);
    bitSlider.addListener(this);
    
    downSampleSlider.setName("Downsample");
    downSampleSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    downSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    //downSampleSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    downSampleSlider.setRange(1, 50);
    downSampleSlider.setValue(audioProcessor.downSampleVal);
    addAndMakeVisible(&downSampleSlider);
    downSampleSlider.addListener(this);
    
    drywetSlider.setName("Daring");
    drywetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    drywetSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    drywetSlider.setRange(0.0f, 100.0f);
    drywetSlider.setValue(audioProcessor.drywetPercentageVal);
    addAndMakeVisible(&drywetSlider);
    drywetSlider.addListener(this);
    
    inputSlider.setName("in");
    inputSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    inputSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    inputSlider.setRange(0.0f, 1.0f);
    inputSlider.setValue(audioProcessor.inputVal);
    addAndMakeVisible(&inputSlider);
    inputSlider.addListener(this);
    
    outputSlider.setName("out");
    outputSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    outputSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    outputSlider.setRange(0.0f, 1.0f);
    outputSlider.setValue(audioProcessor.outputVal);
    addAndMakeVisible(&outputSlider);
    outputSlider.addListener(this);
    
    // buttons
    
    gainButton.setName("Gain Switcher");
    gainButton.setToggleState(true, NotificationType::dontSendNotification);
    gainButton.setClickingTogglesState(true);
    addAndMakeVisible(&gainButton);
    gainButton.addListener(this);
    
    distButton.setName("Distortion Switcher");
    distButton.setToggleState(true, NotificationType::dontSendNotification);
    distButton.setClickingTogglesState(true);
    addAndMakeVisible(&distButton);
    distButton.addListener(this);
    
    crushButton.setName("Crush Switcher");
    crushButton.setToggleState(true, NotificationType::dontSendNotification);
    crushButton.setClickingTogglesState(true);
    addAndMakeVisible(&crushButton);
    crushButton.addListener(this);
    
    downSampleButton.setName("DWNSMPL Switcher");
    downSampleButton.setToggleState(true, NotificationType::dontSendNotification);
    downSampleButton.setClickingTogglesState(true);
    addAndMakeVisible(&downSampleButton);
    downSampleButton.addListener(this);
    
    sinPlot.setName("sin plot");
    addAndMakeVisible(sinPlot);
    
    setSize (1000, 600); // put 1000, 600 for full-image size
}

GuitarCrusherAudioProcessorEditor::~GuitarCrusherAudioProcessorEditor()
{
}

//==============================================================================
void GuitarCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto backgroundImage = ImageCache::getFromMemory (BinaryData::backgroundImg_jpg, BinaryData::backgroundImg_jpgSize);
    g.drawImage (backgroundImage, getLocalBounds().toFloat());
    
    g.setColour(Colours::black);
    
    int wText = 100;
    int hText = 20;
    
    // knobs
    g.drawText(gainSlider.getName(),
               gainSlider.getX() + gainSlider.getWidth()/2 - wText/2,
               gainSlider.getY() + gainSlider.getHeight() + 10,
               wText,
               hText,
               Justification::centred);
    
    g.drawText(distSlider.getName(),
               distSlider.getX() + distSlider.getWidth()/2 - wText/2,
               distSlider.getY() + distSlider.getHeight() + 10,
               wText,
               hText,
               Justification::centred);
    
    g.drawText(bitSlider.getName(),
               bitSlider.getX() + bitSlider.getWidth()/2 - wText/2,
               bitSlider.getY() + bitSlider.getHeight() + 10,
               wText,
               hText,
               Justification::centred);
    
    g.drawText(downSampleSlider.getName(),
               downSampleSlider.getX() + downSampleSlider.getWidth()/2- wText/2,
               downSampleSlider.getY() + downSampleSlider.getHeight() + 10,
               wText,
               hText,
               Justification::centred);
    
    g.drawText(drywetSlider.getName(),
               drywetSlider.getX() + drywetSlider.getWidth()/2 - wText/2,
               drywetSlider.getY() + drywetSlider.getHeight() + 10,
               wText,
               hText,
               Justification::centred);
    
    // buttons
    g.drawText(inputSlider.getName(),
               inputSlider.getX() + inputSlider.getWidth()/2 - wText/2,
               inputSlider.getY() + inputSlider.getHeight() - hText/2 + 10,
               wText,
               hText,
               Justification::centred);
    
    g.drawText(outputSlider.getName(),
               outputSlider.getX() + outputSlider.getWidth()/2 - wText/2,
               outputSlider.getY() + outputSlider.getHeight() - hText/2 + 10,
               wText,
               hText,
               Justification::centred);
}

void GuitarCrusherAudioProcessorEditor::resized()
{
    // dimensions
    
    int dimSwitcher = 25;
    
    int sliderWidth = 50;
    int sliderHeight = 160;
    
    int knobsDim = 120;
    int knobsMargin = 30;
    
    // x, y, w, h
    
    // knobs
    gainSlider.setBounds(sliderWidth+knobsMargin, 0+knobsMargin, knobsDim, knobsDim);
    distSlider.setBounds(gainSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    bitSlider.setBounds(distSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    downSampleSlider.setBounds(bitSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    drywetSlider.setBounds(downSampleSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    
    inputSlider.setBounds(0, 0+knobsMargin, sliderWidth, sliderHeight-knobsMargin);
    outputSlider.setBounds(drywetSlider.getX()+knobsDim+knobsMargin, 0+knobsMargin, sliderWidth, sliderHeight-knobsMargin);
    
    // switchers
    gainButton.setBounds(gainSlider.getX()+knobsDim-dimSwitcher/2, gainSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    distButton.setBounds(distSlider.getX()+knobsDim-dimSwitcher/2, distSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    crushButton.setBounds(bitSlider.getX()+knobsDim-dimSwitcher/2, bitSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    downSampleButton.setBounds(downSampleSlider.getX()+knobsDim-dimSwitcher/2, downSampleSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    
    //audioProcessor.waveViewer.setBounds(sliderWidth/2, 200, 400, 200);
    sinPlot.setBounds(inputSlider.getX()*gainSlider.getValue()+inputSlider.getWidth()/2, 200, getWidth()-outputSlider.getWidth(), 300);
}

void GuitarCrusherAudioProcessorEditor::sliderValueChanged (Slider *slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.gainVal = gainSlider.getValue();
    }
    else if (slider == &bitSlider)
    {
        audioProcessor.bitVal = int(bitSlider.getValue());
        sinPlot.setBit(int(bitSlider.getValue()));
    }
    else if (slider == &downSampleSlider)
    {
        audioProcessor.downSampleVal = downSampleSlider.getValue();
        sinPlot.setDownSampleValue(downSampleSlider.getValue());
    }
    else if (slider == &distSlider)
    {
        audioProcessor.distVal = distSlider.getValue();
    }
    else if (slider == &drywetSlider)
    {
        audioProcessor.drywetPercentageVal = drywetSlider.getValue();
        sinPlot.setPercentageDryWet(drywetSlider.getValue()/100);
    }
    else if (slider == &inputSlider)
    {
        audioProcessor.inputVal = inputSlider.getValue();
    }
    else if (slider == &outputSlider)
    {
        audioProcessor.outputVal = outputSlider.getValue();
    }
}

void GuitarCrusherAudioProcessorEditor::buttonClicked (Button *button)
{
    if (button == &gainButton)
    {
        audioProcessor.gainSwitch = gainButton.getToggleState();
        //DBG(int(gainButton.getToggleState()));
        gainSlider.setEnabled(gainButton.getToggleState());
    }
    else if (button == &distButton)
    {
        audioProcessor.distSwitch = distButton.getToggleState();
        distSlider.setEnabled(distButton.getToggleState());
    }
    else if (button == &crushButton)
    {
        audioProcessor.bitSwitch = crushButton.getToggleState();
        bitSlider.setEnabled(crushButton.getToggleState());
        sinPlot.setIsBitCrushing(crushButton.getToggleState());
    }
    else if (button == &downSampleButton)
    {
        audioProcessor.downSampleSwitch = downSampleButton.getToggleState();
        downSampleSlider.setEnabled(downSampleButton.getToggleState());
        sinPlot.setIsDownSampling(downSampleButton.getToggleState());
    }
}
