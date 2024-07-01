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
    audioProcessor.treeState.addParameterListener(BIT_ID, this);
    
    downSampleSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DWNSMP_ID, downSampleSlider);
    setCustomSliderStyle(downSampleSlider, 0, DWNSMP_NAME);
    audioProcessor.treeState.addParameterListener(DWNSMP_ID, this);
    
    drywetSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DRYWET_ID, drywetSlider);
    setCustomSliderStyle(drywetSlider, 0, DRYWET_NAME);
    audioProcessor.treeState.addParameterListener(DRYWET_ID, this);
    
    // linear vertical
    inputSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, INPUT_ID, inputSlider);
    setCustomSliderStyle(inputSlider, 1, INPUT_NAME);
    
    outputSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, OUTPUT_ID, outputSlider);
    setCustomSliderStyle(outputSlider, 1, OUTPUT_NAME);
    
    // under sinPlot
    lowPassSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, LOWPASS_ID, lowPassSlider);
    setCustomSliderStyle(lowPassSlider, 0, LOWPASS_NAME);
    
    delayAmountSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DELAYAMOUNT_ID, delayAmountSlider);
    setCustomSliderStyle(delayAmountSlider, 0, DELAYAMOUNT_NAME);
    
    delayTimeSliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DELAYTIME_ID, delayTimeSlider);
    setCustomSliderStyle(delayTimeSlider, 0, DELAYTIME_NAME);
    //delayTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 80, 20);
    
    // buttons
    auto configureButton = [this](String ID, String NAME, RedSwitcher& button) {
        auto attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.treeState, ID, button);
        setCustomButtonStyle(button, NAME, ID);
        audioProcessor.treeState.addParameterListener(ID, this);
        return attachment;
    };
    
    gainButtonValue = configureButton(GAIN_BTN_ID, GAIN_BTN_NAME, gainButton);
    gainSlider.setEnabled(gainButton.getToggleState());
    
    distButtonValue = configureButton(DRIVE_BTN_ID, DRIVE_BTN_NAME, distButton);
    distSlider.setEnabled(distButton.getToggleState());
    
    crushButtonValue = configureButton(BIT_BTN_ID, BIT_BTN_NAME, crushButton);
    bitSlider.setEnabled(crushButton.getToggleState());
    
    downSampleButtonValue = configureButton(DWNSMP_BTN_ID, DWNSMP_BTN_NAME, downSampleButton);
    downSampleSlider.setEnabled(downSampleButton.getToggleState());
    
    tailButtonValue = configureButton(TAIL_BTN_ID, TAIL_BTN_NAME, tailButton);
    delayAmountSlider.setEnabled(tailButton.getToggleState());
    delayTimeSlider.setEnabled(tailButton.getToggleState());
    
    bypassButtonValue = configureButton(BYPASS_BTN_ID, BYPASS_BTN_NAME, bypassButton);
    randomButtonValue = configureButton(RANDOM_BTN_ID, RANDOM_BTN_NAME, randomButton);
    flutterButtonValue = configureButton(FLUTTER_BTN_ID, FLUTTER_BTN_NAME, flutterButton);
    reverseButtonValue = configureButton(REV_BTN_ID, REV_BTN_NAME, reverseButton);
    
    // sin plot
    sinPlot.setName("sin plot");
    addAndMakeVisible(sinPlot);
    
    setSize (1000, 700);
}

ANTARCTICAAudioProcessorEditor::~ANTARCTICAAudioProcessorEditor()
{
    audioProcessor.treeState.removeParameterListener(BIT_ID, this);
    audioProcessor.treeState.removeParameterListener(DWNSMP_ID, this);
    audioProcessor.treeState.removeParameterListener(DRYWET_ID, this);
    
    audioProcessor.treeState.removeParameterListener(GAIN_BTN_ID, this);
    audioProcessor.treeState.removeParameterListener(DRIVE_BTN_ID, this);
    audioProcessor.treeState.removeParameterListener(BIT_BTN_ID, this);
    audioProcessor.treeState.removeParameterListener(DWNSMP_BTN_ID, this);
}

void ANTARCTICAAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // sliders
    
    if (parameterID == DWNSMP_ID)
        sinPlot.setDownSampleValue(newValue);
    else if (parameterID == BIT_ID)
        sinPlot.setBit(int(newValue));
    else if (parameterID == DRYWET_ID)
        sinPlot.setPercentageDryWet(newValue/100);
    
    // buttons
    else if (parameterID == GAIN_BTN_ID)
        gainSlider.setEnabled(gainButton.getToggleState());
    else if (parameterID == DRIVE_BTN_ID)
        distSlider.setEnabled(distButton.getToggleState());
    else if (parameterID == BIT_BTN_ID)
    {
        bitSlider.setEnabled(crushButton.getToggleState());
        //sinPlot.setIsBitCrushing(crushButton.getToggleState());
        sinPlot.setIsBitCrushing(newValue);
    }
    else if (parameterID == DWNSMP_BTN_ID)
    {
        downSampleSlider.setEnabled(downSampleButton.getToggleState());
        //sinPlot.setIsDownSampling(downSampleButton.getToggleState());
        sinPlot.setIsDownSampling(newValue);
    }
    else if (parameterID == TAIL_BTN_ID)
    {
        delayAmountSlider.setEnabled(tailButton.getToggleState());
        delayTimeSlider.setEnabled(tailButton.getToggleState());
    }
}

void ANTARCTICAAudioProcessorEditor::updateSinPlot()
{
    sinPlot.setDownSampleValue(audioProcessor.treeState.getRawParameterValue(DWNSMP_ID)->load());
    sinPlot.setBit(int(audioProcessor.treeState.getRawParameterValue(BIT_ID)->load()));
    sinPlot.setPercentageDryWet(audioProcessor.treeState.getRawParameterValue(DRYWET_ID)->load()/100);
    sinPlot.setIsBitCrushing(crushButton.getToggleState());
    sinPlot.setIsDownSampling(downSampleButton.getToggleState());
}

void ANTARCTICAAudioProcessorEditor::setCustomButtonStyle(Button& b, String name, String id)
{
    b.setName(name);
    b.setToggleState(audioProcessor.treeState.getRawParameterValue(id)->load(), NotificationType::dontSendNotification);
    addAndMakeVisible(&b);
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
    drawTextSlider(lowPassSlider);
    drawTextSlider(delayAmountSlider);
    drawTextSlider(delayTimeSlider);
    
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
    
    updateSinPlot();
}

void ANTARCTICAAudioProcessorEditor::resized()
{
    // dimensions
    
    int dimSwitcher = 25;
    
    int sliderWidth = 50;
    int sliderHeight = 160;
    
    int knobsDim = 120;
    int knobsMargin = 30;
    
    int sinPlotMargin = 500;
    
    // x, y, w, h
    
    // knobs
    gainSlider.setBounds(sliderWidth+knobsMargin, 0+knobsMargin, knobsDim, knobsDim);
    distSlider.setBounds(gainSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    downSampleSlider.setBounds(distSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    bitSlider.setBounds(downSampleSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    drywetSlider.setBounds(bitSlider.getX()+knobsDim+knobsMargin*2, 0+knobsMargin, knobsDim, knobsDim);
    lowPassSlider.setBounds(sliderWidth+knobsMargin, sinPlotMargin+knobsMargin, knobsDim, knobsDim);
    delayAmountSlider.setBounds(lowPassSlider.getX()+knobsDim+knobsMargin*2, sinPlotMargin+knobsMargin, knobsDim, knobsDim);
    delayTimeSlider.setBounds(delayAmountSlider.getX()+knobsDim+knobsMargin*2, sinPlotMargin+knobsMargin, knobsDim, knobsDim);
    
    inputSlider.setBounds(0, 0+knobsMargin, sliderWidth, sliderHeight-knobsMargin);
    outputSlider.setBounds(drywetSlider.getX()+knobsDim+knobsMargin, 0+knobsMargin, sliderWidth, sliderHeight-knobsMargin);
    
    // switchers
    gainButton.setBounds(gainSlider.getX()+knobsDim-dimSwitcher/2, gainSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    distButton.setBounds(distSlider.getX()+knobsDim-dimSwitcher/2, distSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    downSampleButton.setBounds(downSampleSlider.getX()+knobsDim-dimSwitcher/2, downSampleSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    crushButton.setBounds(bitSlider.getX()+knobsDim-dimSwitcher/2, bitSlider.getY()+knobsDim-dimSwitcher/2, dimSwitcher, dimSwitcher);
    
    sinPlot.setBounds(inputSlider.getX()+inputSlider.getWidth()/2, 200, getWidth()-outputSlider.getWidth(), 300);
    
    updateSinPlot();
}
