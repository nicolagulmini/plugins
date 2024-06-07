#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ANTARCTICAAudioProcessorEditor::ANTARCTICAAudioProcessorEditor (ANTARCTICAAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // knobs
    setCustomSliderStyle(gainSlider, 0, "Gain", -6.0f, 12.0f, audioProcessor.gainVal);
    setCustomSliderStyle(distSlider, 0, "Drive", 0.1f, 11.0f, audioProcessor.distVal);
    setCustomSliderStyle(bitSlider, 0, "Bit", 0.0f, 30.0f, audioProcessor.bitVal);
    setCustomSliderStyle(downSampleSlider, 0, "Downsample", 1.0f, 50.0f, audioProcessor.downSampleVal);
    setCustomSliderStyle(drywetSlider, 0, "Dry/Wet", 0.0f, 100.0f, audioProcessor.drywetPercentageVal);
    
    // linear vertical
    setCustomSliderStyle(inputSlider, 1, "in", 0.0f, 1.0f, audioProcessor.inputVal);
    setCustomSliderStyle(outputSlider, 1, "out", 0.0f, 1.0f, audioProcessor.outputVal);
        
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
    
    setSize (1000, 550); // put 1000, 600 for full-image size
}

ANTARCTICAAudioProcessorEditor::~ANTARCTICAAudioProcessorEditor()
{
}

void ANTARCTICAAudioProcessorEditor::setCustomSliderStyle(Slider& s, int type, String name, double rangeStart, double rangeEnd, double value)
{
    s.setName(name);
    
   if (type == 0) // means Rotary
       s.setSliderStyle(Slider::SliderStyle::Rotary);
    else if (type == 1) // means Linear
        s.setSliderStyle(Slider::SliderStyle::LinearVertical);

    s.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    s.setRange(rangeStart, rangeEnd);
    s.setValue(value);
    addAndMakeVisible(&s);
    s.addListener(this);
}

//==============================================================================
void ANTARCTICAAudioProcessorEditor::paint (juce::Graphics& g)
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
    
    
    g.setColour(Colours::white);
    g.setOpacity(0.2);
    g.fillRoundedRectangle(inputSlider.getX()+inputSlider.getWidth()/2-1, 200-1, getWidth()-outputSlider.getWidth()+1, 300+1, 10);

    g.setColour(Colours::black);
    g.setOpacity(0.1);
    g.drawVerticalLine(inputSlider.getX()+inputSlider.getWidth()/2-1+190, 200, 500);
    g.drawVerticalLine(inputSlider.getX()+inputSlider.getWidth()/2-1+190*2, 200, 500);
    g.drawVerticalLine(inputSlider.getX()+inputSlider.getWidth()/2-1+190*3, 200, 500);
    g.drawVerticalLine(inputSlider.getX()+inputSlider.getWidth()/2-1+190*4, 200, 500);
    
    g.drawHorizontalLine(199+60, inputSlider.getX()+inputSlider.getWidth()/2-1, inputSlider.getX()-inputSlider.getWidth()/2+getWidth());
    g.drawHorizontalLine(199+60*2, inputSlider.getX()+inputSlider.getWidth()/2-1, inputSlider.getX()-inputSlider.getWidth()/2+getWidth());
    g.drawHorizontalLine(199+60*3, inputSlider.getX()+inputSlider.getWidth()/2-1, inputSlider.getX()-inputSlider.getWidth()/2+getWidth());
    g.drawHorizontalLine(199+60*4, inputSlider.getX()+inputSlider.getWidth()/2-1, inputSlider.getX()-inputSlider.getWidth()/2+getWidth());
    
    g.setColour(Colours::black);
    g.setOpacity(1);
    g.drawRoundedRectangle(inputSlider.getX()+inputSlider.getWidth()/2-1, 200-1, getWidth()-outputSlider.getWidth()+1, 300+1, 10, 3);
}

void ANTARCTICAAudioProcessorEditor::resized()
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
    sinPlot.setBounds(inputSlider.getX()+inputSlider.getWidth()/2, 200, getWidth()-outputSlider.getWidth(), 300);
}

void ANTARCTICAAudioProcessorEditor::sliderValueChanged (Slider *slider)
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

void ANTARCTICAAudioProcessorEditor::buttonClicked (Button *button)
{
    if (button == &gainButton)
    {
        audioProcessor.gainSwitch = gainButton.getToggleState();
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
