/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicDelayAudioProcessorEditor::GraphicDelayAudioProcessorEditor (GraphicDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    setSize (400, 300);
}

GraphicDelayAudioProcessorEditor::~GraphicDelayAudioProcessorEditor()
{
}

//==============================================================================
void GraphicDelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void GraphicDelayAudioProcessorEditor::resized()
{
}
