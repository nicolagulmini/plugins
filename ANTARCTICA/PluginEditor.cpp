#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ANTARCTICAAudioProcessorEditor::ANTARCTICAAudioProcessorEditor (ANTARCTICAAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // treeState
    gainSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, GAIN_ID, gainSlider);
    setCustomSliderStyle(gainSlider, 0, GAIN_NAME);

    distSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DRIVE_ID, distSlider);
    setCustomSliderStyle(distSlider, 0, DRIVE_NAME);
    
    bitSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, BIT_ID, bitSlider);
    setCustomSliderStyle(bitSlider, 0, BIT_NAME);
    
    downSampleSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DWNSMP_ID, downSampleSlider);
    setCustomSliderStyle(downSampleSlider, 0, DWNSMP_NAME);
    
    gainSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DRYWET_ID, drywetSlider);
    setCustomSliderStyle(drywetSlider, 0, DRYWET_NAME);
    
    // dev
    //setCustomSliderStyle(preLowPass, 0, "Pre Low Pass Filter", 20.0f, 20000.0f, audioProcessor.filterAfterProcessFreq);
    // no knob for this
    
    // linear vertical
    inputSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, INPUT_ID, inputSlider);
    setCustomSliderStyle(inputSlider, 1, INPUT_NAME);
    
    outputSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, outputSlider);
    setCustomSliderStyle(outputSlider, 1, OUTPUT_NAME);
        
    // buttons
    setCustomButtonStyle(gainButton, "Gain Switcher", true);
    setCustomButtonStyle(distButton, "Distortion Switcher", true);
    setCustomButtonStyle(crushButton, "Crush Switcher", true);
    setCustomButtonStyle(downSampleButton, "DWNSMPL Switcher", true);

    sinPlot.setName("sin plot");
    addAndMakeVisible(sinPlot);
    
    setSize (1000, 550); // put 1000, 600 for full-image size
}

ANTARCTICAAudioProcessorEditor::~ANTARCTICAAudioProcessorEditor()
{
}

void ANTARCTICAAudioProcessorEditor::setCustomButtonStyle(Button& b, String name, bool state)
{
    b.setName(name);
    b.setToggleState(state, NotificationType::dontSendNotification);
    b.setClickingTogglesState(state);
    addAndMakeVisible(&b);
    b.addListener(this);
}

void ANTARCTICAAudioProcessorEditor::setCustomSliderStyle(Slider& s, int type, String name)
{
    s.setName(name);
    
   if (type == 0) // means Rotary
       s.setSliderStyle(Slider::SliderStyle::Rotary);
    else if (type == 1) // means Linear
        s.setSliderStyle(Slider::SliderStyle::LinearVertical);

    s.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    addAndMakeVisible(&s);
}

//==============================================================================
void ANTARCTICAAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto backgroundImage = ImageCache::getFromMemory (BinaryData::backgroundImg_jpg, BinaryData::backgroundImg_jpgSize);
    g.drawImage (backgroundImage, getLocalBounds().toFloat());
    
    g.setColour(Colours::black);
    
    int wText = 100;
    int hText = 20;
    
    auto drawTextSlider = [wText, hText, &g](Slider& s) {
        g.drawText(s.getName(),
                   s.getX() + s.getWidth()/2 - wText/2,
                   s.getY() + s.getHeight() + 10,
                   wText,
                   hText,
                   Justification::centred);
    };
    
    // knobs
    drawTextSlider(gainSlider);
    drawTextSlider(distSlider);
    drawTextSlider(bitSlider);
    drawTextSlider(downSampleSlider);
    drawTextSlider(drywetSlider);
    // dev
    //drawTextSlider(preLowPass);
    
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
    
    // dev
    //preLowPass.setBounds(0, 400, knobsDim, knobsDim);
    
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
    if (slider == &bitSlider)
    {
        sinPlot.setBit(int(bitSlider.getValue()));
    }
    else if (slider == &downSampleSlider)
    {
        sinPlot.setDownSampleValue(downSampleSlider.getValue());
    }
    else if (slider == &drywetSlider)
    {
        sinPlot.setPercentageDryWet(drywetSlider.getValue()/100);
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
