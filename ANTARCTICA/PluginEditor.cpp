#include "PluginProcessor.h"
#include "PluginEditor.h"

ANTARCTICAAudioProcessorEditor::ANTARCTICAAudioProcessorEditor (ANTARCTICAAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    // lambda sliders
    auto configureSlider = [this](String ID, String NAME, pearlSlider& slider, bool listener, int style) {
        auto sliderValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, ID, slider);
        setCustomSliderStyle(slider, style, NAME);
        if (listener) audioProcessor.treeState.addParameterListener(ID, this);
        return sliderValue;
    };
    
    gainSliderValue = configureSlider(GAIN_ID, GAIN_NAME, gainSlider, false, 2);
    
    clipperSliderValue = configureSlider(CLIPPER_ID, CLIPPER_NAME, clipperSlider, false, 2);
    saturationSliderValue = configureSlider(SATURATION_ID, SATURATION_NAME, saturationSlider, false, 2);
    distSliderValue = configureSlider(DRIVE_ID, DRIVE_NAME, distSlider, false, 2);
    bitSliderValue = configureSlider(BIT_ID, BIT_NAME, bitSlider, true, 2);
    downSampleSliderValue = configureSlider(DWNSMP_ID, DWNSMP_NAME, downSampleSlider, true, 2);
    
    drywetSliderValue = configureSlider(DRYWET_ID, DRYWET_NAME, drywetSlider, true, 2);
    antialiasingSliderValue = configureSlider(ANTIALIASING_ID, ANTIALIASING_NAME, antialiasingSlider, false, 2);
    inputSliderValue = configureSlider(INPUT_ID, INPUT_NAME, inputSlider, false, 1);
    outputSliderValue = configureSlider(OUTPUT_ID, OUTPUT_NAME, outputSlider, false, 1);
    
    delayAmountSliderValue = configureSlider(DELAYAMOUNT_ID, DELAYAMOUNT_NAME, delayAmountSlider, false, 2);
    delayTimeSliderValue = configureSlider(DELAYTIME_ID, DELAYTIME_NAME, delayTimeSlider, false, 2);
    fadeinSliderValue = configureSlider(FADEIN_ID, FADEIN_NAME, fadeinSlider, false, 2);
    fadeoutSliderValue = configureSlider(FADEOUT_ID, FADEOUT_NAME, fadeoutSlider, false, 2);
    
    rndIntervalSliderValue = configureSlider(RND_INTERVAL_ID, RND_INTERVAL_NAME, rndIntervalSlider, false, 2);
    
    rndDurationSliderValue = configureSlider(RND_DURATION_ID, RND_DURATION_NAME, rndDurationSlider, false, 2);
    
    
    // lambda buttons
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
    rndIntervalSlider.setEnabled(randomButton.getToggleState());
    rndDurationSlider.setEnabled(randomButton.getToggleState());
    
    flutterButtonValue = configureButton(FLUTTER_BTN_ID, FLUTTER_BTN_NAME, flutterButton);
    reverseButtonValue = configureButton(REV_BTN_ID, REV_BTN_NAME, reverseButton);
    
    setSize (1000, 600);
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
    // buttons
    if (parameterID == GAIN_BTN_ID)
        gainSlider.setEnabled(gainButton.getToggleState());
    else if (parameterID == DRIVE_BTN_ID)
        distSlider.setEnabled(distButton.getToggleState());
    else if (parameterID == BIT_BTN_ID)
        bitSlider.setEnabled(crushButton.getToggleState());
    else if (parameterID == DWNSMP_BTN_ID)
        downSampleSlider.setEnabled(downSampleButton.getToggleState());
    else if (parameterID == TAIL_BTN_ID)
    {
        delayAmountSlider.setEnabled(tailButton.getToggleState());
        delayTimeSlider.setEnabled(tailButton.getToggleState());
    }
    else if (parameterID == RANDOM_BTN_ID)
    {
        rndIntervalSlider.setEnabled(randomButton.getToggleState());
        rndDurationSlider.setEnabled(randomButton.getToggleState());
    }
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
    else if (type == 1) // Linear Vertical
        s.setSliderStyle(Slider::SliderStyle::LinearVertical);
    else if (type == 2) // Linear Horizontal
        s.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    
    s.setSkewFactor(0.5);

    s.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 50, 20);
    addAndMakeVisible(&s);
}

//==============================================================================
void ANTARCTICAAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto backgroundImage = ImageCache::getFromMemory (BinaryData::backgroundImg_jpg, BinaryData::backgroundImg_jpgSize);
    g.drawImage (backgroundImage, getLocalBounds().toFloat());
    
    g.setColour(Colours::black);
    
    int wText = 120;
    int hText = 20;
    
    
    
    auto drawTextSlider = [wText, hText, &g](Slider& s, int style) {
        if (style == 0) // means rotary
            g.drawText(s.getName(),
                   s.getX() + s.getWidth()/2 - wText/2,
                   s.getY() + s.getHeight() + 10,
                   wText,
                   hText,
                   Justification::centred);
        else if (style == 1) // means linear vertical
            g.drawText(s.getName(),
                       s.getX() + s.getWidth()/2 - wText/2,
                       s.getY() + s.getHeight() - hText/2 + 10,
                       wText,
                       hText,
                       Justification::centred);
        else if (style == 2) // means linear horizontal
            g.drawText(s.getName(),
                       s.getX() + s.getWidth(), // -30?
                       s.getY() + s.getHeight()/2 - hText/2,
                       wText,
                       hText,
                       Justification::centred);
    };
    
    // knobs
    drawTextSlider(gainSlider, 2);
    drawTextSlider(saturationSlider, 2);
    drawTextSlider(distSlider, 2);
    drawTextSlider(clipperSlider, 2);
    drawTextSlider(bitSlider, 2);
    drawTextSlider(downSampleSlider, 2);
    drawTextSlider(drywetSlider, 2);
    drawTextSlider(delayAmountSlider, 2);
    drawTextSlider(delayTimeSlider, 2);
    drawTextSlider(rndDurationSlider, 2);
    drawTextSlider(rndIntervalSlider, 2);
    
    // vertical sliders
    //drawTextSlider(inputSlider, 1);
    //drawTextSlider(outputSlider, 1);
    
    // buttons // to generalize
    auto drawTextButton = [wText, hText, &g](Button& b) {
        g.drawText(b.getName(),
                   b.getX() + b.getWidth(),
                   b.getY() + b.getHeight()/2 - hText/2,
                   wText,
                   hText,
                   Justification::centred);
    };
    
    // buttons
    drawTextButton(flutterButton);
    drawTextButton(reverseButton);
    
    
}

void ANTARCTICAAudioProcessorEditor::resized()
{
    // x, y, w, h
    
    int horizontalSliderWidth = 320;
    int horizontalSliderHeight = 50;
    int verticalSliderWidth = 50;
    int verticalSliderHeight = 160;
    
    int knobsDim = 120;
    int switcherDim = 25;
    
    int buttonLeftMargin = 50;
    int sliderLeftMargin = 100;
    int interComponentTextMargin = 100;
    int interComponentMargin = 30;
    
    
    // horizontal sliders
    gainSlider.setBounds(verticalSliderWidth+sliderLeftMargin, 0, horizontalSliderWidth, horizontalSliderHeight);
    saturationSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*1, horizontalSliderWidth, horizontalSliderHeight);
    distSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*2, horizontalSliderWidth, horizontalSliderHeight);
    clipperSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*3, horizontalSliderWidth, horizontalSliderHeight);
    downSampleSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*4, horizontalSliderWidth, horizontalSliderHeight);
    bitSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*5, horizontalSliderWidth, horizontalSliderHeight);
    
    delayAmountSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*6, horizontalSliderWidth, horizontalSliderHeight);
    delayTimeSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*7, horizontalSliderWidth, horizontalSliderHeight);
    
    rndIntervalSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*8, horizontalSliderWidth, horizontalSliderHeight);
    rndDurationSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*9, horizontalSliderWidth, horizontalSliderHeight);

    drywetSlider.setBounds(verticalSliderWidth+sliderLeftMargin, horizontalSliderHeight*10, horizontalSliderWidth, horizontalSliderHeight);

    // left switchers
    gainButton.setBounds(gainSlider.getX()-switcherDim-interComponentMargin, gainSlider.getY()+switcherDim/2, switcherDim, switcherDim);
    
    distButton.setBounds(distSlider.getX()-switcherDim-interComponentMargin, distSlider.getY()+switcherDim/2, switcherDim, switcherDim);
    downSampleButton.setBounds(downSampleSlider.getX()-switcherDim-interComponentMargin, downSampleSlider.getY()+switcherDim/2, switcherDim, switcherDim);
    crushButton.setBounds(bitSlider.getX()-switcherDim-interComponentMargin, bitSlider.getY()+switcherDim/2, switcherDim, switcherDim);
    
    tailButton.setBounds(delayAmountSlider.getX()-switcherDim-interComponentMargin, (delayAmountSlider.getY()+delayTimeSlider.getY())/2+switcherDim/2, switcherDim, switcherDim); // between the two delay sliders
    randomButton.setBounds(rndIntervalSlider.getX()-switcherDim-interComponentMargin, (rndIntervalSlider.getY()+rndDurationSlider.getY())/2+switcherDim/2, switcherDim, switcherDim); // same
    
    // right switchers
    flutterButton.setBounds(delayAmountSlider.getX()+horizontalSliderWidth+interComponentMargin+interComponentTextMargin, tailButton.getY(), switcherDim, switcherDim);
    reverseButton.setBounds(delayAmountSlider.getX()+horizontalSliderWidth+interComponentMargin*2+switcherDim+interComponentTextMargin*2, tailButton.getY(), switcherDim, switcherDim);
    
    // only switchers section
    bypassButton.setBounds(verticalSliderWidth+buttonLeftMargin, 550, switcherDim, switcherDim);
    
    /*
    
    // vertical sliders
    inputSlider.setBounds(0, 0+knobsMargin, sliderWidth, sliderHeight-knobsMargin);
    outputSlider.setBounds(drywetSlider.getX()+knobsDim+knobsMargin, 0+knobsMargin, sliderWidth, sliderHeight-knobsMargin);
    
    */
}
