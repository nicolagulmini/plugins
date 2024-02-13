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
    bitSlider.setRange(0, 30, 1);
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
    gainButton.setColour(0x1006502, Colours::red);
    gainButton.setColour(0x1006503, Colours::black);
    addAndMakeVisible(&gainButton);
    gainButton.addListener(this);
    
    distButton.setName("Distortion Switcher");
    distButton.setToggleState(true, NotificationType::dontSendNotification);
    distButton.setClickingTogglesState(true);
    distButton.setColour(0x1006502, Colours::red);
    distButton.setColour(0x1006503, Colours::black);
    addAndMakeVisible(&distButton);
    distButton.addListener(this);
    
    crushButton.setName("Crush Switcher");
    crushButton.setToggleState(true, NotificationType::dontSendNotification);
    crushButton.setClickingTogglesState(true);
    crushButton.setColour(0x1006502, Colours::red);
    crushButton.setColour(0x1006503, Colours::black);
    addAndMakeVisible(&crushButton);
    crushButton.addListener(this);
    
    downSampleButton.setName("DWNSMPL Switcher");
    downSampleButton.setToggleState(true, NotificationType::dontSendNotification);
    downSampleButton.setClickingTogglesState(true);
    downSampleButton.setColour(0x1006502, Colours::red);
    downSampleButton.setColour(0x1006503, Colours::black);
    addAndMakeVisible(&downSampleButton);
    downSampleButton.addListener(this);
    
    setSize (1000, 200); // put 1000, 600 for full-image size
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
    g.drawText(gainSlider.getName(), gainSlider.getX() + gainSlider.getWidth()/2 - wText/2, gainSlider.getY() + gainSlider.getHeight()/2 - hText/2 + gainButton.getY()/2, wText, hText, Justification::centred);
    g.drawText(distSlider.getName(), distSlider.getX() + distSlider.getWidth()/2 - wText/2, distSlider.getY() + distSlider.getHeight()/2 - hText/2 + distButton.getY()/2, wText, hText, Justification::centred);
    g.drawText(bitSlider.getName(), bitSlider.getX() + bitSlider.getWidth()/2 - wText/2, bitSlider.getY() + bitSlider.getHeight()/2 - hText/2 + crushButton.getY()/2, wText, hText, Justification::centred);
    g.drawText(downSampleSlider.getName(), downSampleSlider.getX() + downSampleSlider.getWidth()/2- wText/2, downSampleSlider.getY() + downSampleSlider.getHeight()/2 - hText/2 + downSampleButton.getY()/2, wText, hText, Justification::centred);
    g.drawText(drywetSlider.getName(), drywetSlider.getX() + drywetSlider.getWidth()/2 - wText/2, drywetSlider.getY() + drywetSlider.getHeight()/2 - hText/2 + distButton.getY()/2, wText, hText, Justification::centred);
    
    g.drawText(inputSlider.getName(), inputSlider.getX() + inputSlider.getWidth()/2 - wText/2, inputSlider.getY() + inputSlider.getHeight() - hText/2, wText, hText, Justification::centred);
    g.drawText(outputSlider.getName(), outputSlider.getX() + outputSlider.getWidth()/2 - wText/2, outputSlider.getY() + outputSlider.getHeight() - hText/2, wText, hText, Justification::centred);
}

void GuitarCrusherAudioProcessorEditor::resized()
{
    // dimensions
    
    int dimSwitcher = 25;
    int sliderWidth = 50;
    int sliderHeight = 160;
    int knobsDim = 180;
    
    // x, y, w, h
    
    // knobs
    gainSlider.setBounds(sliderWidth, 0, knobsDim, knobsDim);
    distSlider.setBounds(knobsDim+sliderWidth, 0, knobsDim, knobsDim);
    bitSlider.setBounds(knobsDim*2+sliderWidth, 0, knobsDim, knobsDim);
    downSampleSlider.setBounds(knobsDim*3+sliderWidth, 0, knobsDim, knobsDim);
    drywetSlider.setBounds(knobsDim*4+sliderWidth, 0, knobsDim, knobsDim);
    
    inputSlider.setBounds(0, 0, sliderWidth, sliderHeight);
    outputSlider.setBounds(knobsDim*5+sliderWidth, 0, sliderWidth, sliderHeight);
    
    // switcher
    
    gainButton.setBounds(knobsDim/2-dimSwitcher/2+sliderWidth, knobsDim/2-dimSwitcher/2, dimSwitcher, dimSwitcher);
    distButton.setBounds(knobsDim/2*3-dimSwitcher/2+sliderWidth, knobsDim/2-dimSwitcher/2, dimSwitcher, dimSwitcher);
    crushButton.setBounds(knobsDim/2*5-dimSwitcher/2+sliderWidth, knobsDim/2-dimSwitcher/2, dimSwitcher, dimSwitcher);
    downSampleButton.setBounds(knobsDim/2*7-dimSwitcher/2+sliderWidth, knobsDim/2-dimSwitcher/2, dimSwitcher, dimSwitcher);
}

void GuitarCrusherAudioProcessorEditor::sliderValueChanged (Slider *slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.gainVal = gainSlider.getValue();
    }
    else if (slider == &bitSlider)
    {
        audioProcessor.bitVal = bitSlider.getValue();
    }
    else if (slider == &downSampleSlider)
    {
        audioProcessor.downSampleVal = downSampleSlider.getValue();
    }
    else if (slider == &distSlider)
    {
        audioProcessor.distVal = distSlider.getValue();
    }
    else if (slider == &drywetSlider)
    {
        audioProcessor.drywetPercentageVal = drywetSlider.getValue();
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
    }
    else if (button == &distButton)
    {
        audioProcessor.distSwitch = distButton.getToggleState();
    }
    else if (button == &crushButton)
    {
        audioProcessor.bitSwitch = crushButton.getToggleState();
    }
    else if (button == &downSampleButton)
    {
        audioProcessor.downSampleSwitch = downSampleButton.getToggleState();
    }
}
