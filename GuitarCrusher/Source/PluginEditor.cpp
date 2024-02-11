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
    gainSlider.setName("Gain");
    gainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    gainSlider.setRange(-60.0f, 12.0f);
    gainSlider.setValue(0.0f);
    addAndMakeVisible(&gainSlider);
    gainSlider.addListener(this);
    
    bitSlider.setName("Bit");
    bitSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    bitSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    bitSlider.setRange(2, 32, 1);
    bitSlider.setValue(32);
    addAndMakeVisible(&bitSlider);
    bitSlider.addListener(this);
    
    downSampleSlider.setName("Downsample");
    downSampleSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    downSampleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    //downSampleSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 200, 20);
    downSampleSlider.setRange(1, 50);
    downSampleSlider.setValue(1);
    addAndMakeVisible(&downSampleSlider);
    downSampleSlider.addListener(this);
    
    setSize (600, 200);
}

GuitarCrusherAudioProcessorEditor::~GuitarCrusherAudioProcessorEditor()
{
}

//==============================================================================
void GuitarCrusherAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.setColour(Colours::white);
    int wText = 100;
    int hText = 20;
    g.drawText("Gain", gainSlider.getX() + gainSlider.getWidth()/2 - wText/2, gainSlider.getY() + gainSlider.getHeight()/2 - hText/2, wText, hText, Justification::centred);
    g.drawText("Bit", bitSlider.getX() + bitSlider.getWidth()/2 - wText/2, bitSlider.getY() + bitSlider.getHeight()/2 - hText/2, wText, hText, Justification::centred);
    g.drawText("Downsample", downSampleSlider.getX() + downSampleSlider.getWidth()/2- wText/2, downSampleSlider.getY() + downSampleSlider.getHeight()/2 - hText/2, wText, hText, Justification::centred);
}

void GuitarCrusherAudioProcessorEditor::resized()
{
    int margin = 10;
    int dim = getHeight()-margin;
    int height = dim;
    int width = getWidth();
    // x, y, w, h
    gainSlider.setBounds(width/3-dim, height-dim, dim, dim);
    bitSlider.setBounds(width/3*2-dim, height-dim, dim, dim);
    downSampleSlider.setBounds(width/3*3-dim, height-dim, dim, dim);
}

void GuitarCrusherAudioProcessorEditor::sliderValueChanged (juce::Slider *slider)
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
}
