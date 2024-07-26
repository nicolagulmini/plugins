#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ANTARCTICAAudioProcessor::ANTARCTICAAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS TREESTATE", createParameterLayout())
#endif
{
}

ANTARCTICAAudioProcessor::~ANTARCTICAAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout ANTARCTICAAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<RangedAudioParameter>> params;

    // sliders
    auto gainParams = std::make_unique<AudioParameterFloat>(ParameterID{GAIN_ID,1}, GAIN_NAME, 0.0f, 12.0f, local_gain);
    params.push_back(std::move(gainParams));
    
    auto driveParams = std::make_unique<AudioParameterFloat>(ParameterID{DRIVE_ID,1}, DRIVE_NAME, 1.0f, 12.0f, local_drive);
    params.push_back(std::move(driveParams));
    
    auto bitParams = std::make_unique<AudioParameterFloat>(ParameterID{BIT_ID,1}, BIT_NAME, 0.0f, 30.0f, local_bit);
    params.push_back(std::move(bitParams));
    
    auto downsampleParams = std::make_unique<AudioParameterFloat>(ParameterID{DWNSMP_ID,1}, DWNSMP_NAME, 0.0f, 50.0f, local_dwnsmp);
    params.push_back(std::move(downsampleParams));
    
    auto drywetParams = std::make_unique<AudioParameterFloat>(ParameterID{DRYWET_ID,1}, DRYWET_NAME, 0.0f, 100.0f, local_drywet);
    params.push_back(std::move(drywetParams));
    
    auto inputParams = std::make_unique<AudioParameterFloat>(ParameterID{INPUT_ID,1}, INPUT_NAME, 0.0f, 2.0f, local_input);
    params.push_back(std::move(inputParams));
    
    auto outputParams = std::make_unique<AudioParameterFloat>(ParameterID{OUTPUT_ID,1}, OUTPUT_NAME, 0.0f, 2.0f, local_output);
    params.push_back(std::move(outputParams));
    
    auto rndDuration = std::make_unique<AudioParameterFloat>(ParameterID{RND_DURATION_ID,1}, RND_DURATION_NAME, 50.0f, 1000.0f, local_rnd_duration);
    params.push_back(std::move(rndDuration));
    
    auto rndInterval = std::make_unique<AudioParameterFloat>(ParameterID{RND_INTERVAL_ID,1}, RND_INTERVAL_NAME, 50.0f, 1000.0f, local_rnd_interval);
    params.push_back(std::move(rndInterval));
    
    auto delayAmountParams = std::make_unique<AudioParameterFloat>(ParameterID{DELAYAMOUNT_ID,1}, DELAYAMOUNT_NAME, 0.0f, 0.9f, local_delayAmount);
    params.push_back(std::move(delayAmountParams));
    
    auto delayTimeParams = std::make_unique<AudioParameterFloat>(ParameterID{DELAYTIME_ID,1}, DELAYTIME_NAME, 50.0f, 800.0f, local_delayTime);
    params.push_back(std::move(delayTimeParams));

    
    // buttons
    auto gainButtonParams = std::make_unique<AudioParameterBool>(ParameterID{GAIN_BTN_ID,1}, GAIN_BTN_NAME, true);
    params.push_back(std::move(gainButtonParams));
    
    auto driveButtonParams = std::make_unique<AudioParameterBool>(ParameterID{DRIVE_BTN_ID,1}, DRIVE_BTN_NAME, false);
    params.push_back(std::move(driveButtonParams));
    
    auto bitButtonParams = std::make_unique<AudioParameterBool>(ParameterID{BIT_BTN_ID,1}, BIT_BTN_NAME, true);
    params.push_back(std::move(bitButtonParams));
    
    auto dwnsmpButtonParams = std::make_unique<AudioParameterBool>(ParameterID{DWNSMP_BTN_ID,1}, DWNSMP_BTN_NAME, true);
    params.push_back(std::move(dwnsmpButtonParams));
    
    auto bypassButtonParams = std::make_unique<AudioParameterBool>(ParameterID{BYPASS_BTN_ID,1}, BYPASS_BTN_NAME, false);
    params.push_back(std::move(bypassButtonParams));
    
    auto randomButtonParams = std::make_unique<AudioParameterBool>(ParameterID{RANDOM_BTN_ID,1}, RANDOM_BTN_NAME, false);
    params.push_back(std::move(randomButtonParams));
    
    auto flutterButtonParams = std::make_unique<AudioParameterBool>(ParameterID{FLUTTER_BTN_ID,1}, FLUTTER_BTN_NAME, false);
    params.push_back(std::move(flutterButtonParams));
    
    auto reverseButtonParams = std::make_unique<AudioParameterBool>(ParameterID{REV_BTN_ID,1}, REV_BTN_NAME, false);
    params.push_back(std::move(reverseButtonParams));
    
    auto tailButtonParams = std::make_unique<AudioParameterBool>(ParameterID{TAIL_BTN_ID,1}, TAIL_BTN_NAME, false);
    params.push_back(std::move(tailButtonParams));

    // return vector
    return {params.begin(), params.end()};
}

//==============================================================================
const juce::String ANTARCTICAAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ANTARCTICAAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ANTARCTICAAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ANTARCTICAAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ANTARCTICAAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ANTARCTICAAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ANTARCTICAAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ANTARCTICAAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ANTARCTICAAudioProcessor::getProgramName (int index)
{
    return {};
}

void ANTARCTICAAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ANTARCTICAAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayBuffer.setSize(getTotalNumInputChannels(), (int) sampleRate * 2); // 4 seconds of circular buffer
    delayBuffer.clear(); // to avoid bad effects
}

void ANTARCTICAAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    //waveViewer.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ANTARCTICAAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

/* DEPRECATED: performance issues
void ANTARCTICAAudioProcessor::updateParam(float& localParam, String ID_PARAM, String ID_BTN, float velocity)
{ // let's suppose that ID_PARAM and ID_BTN exist...
    // smooth update
    float treeStateParam = treeState.getRawParameterValue(ID_PARAM)->load();
    
    if(ID_BTN.length()>0 && !treeState.getRawParameterValue(ID_BTN)->load())
        treeStateParam = 0; // not default, just to prevent sharpness
    
    if (abs(localParam-treeStateParam) < EPSILON*velocity)
        localParam = treeStateParam;
    else if (localParam > treeStateParam)
        localParam -= EPSILON*velocity;
    else
        localParam += EPSILON*velocity;
}
 */

void ANTARCTICAAudioProcessor::updateParams()
{
    local_gain          = treeState.getRawParameterValue(GAIN_ID)->load();
    local_drive         = treeState.getRawParameterValue(DRIVE_ID)->load();
    local_bit           = treeState.getRawParameterValue(BIT_ID)->load();
    local_dwnsmp        = treeState.getRawParameterValue(DWNSMP_ID)->load();
    local_drywet        = treeState.getRawParameterValue(DRYWET_ID)->load();
    local_input         = treeState.getRawParameterValue(INPUT_ID)->load();
    local_output        = treeState.getRawParameterValue(OUTPUT_ID)->load();
    local_delayTime     = treeState.getRawParameterValue(DELAYTIME_ID)->load();
    local_delayAmount   = treeState.getRawParameterValue(DELAYAMOUNT_ID)->load();
    local_rnd_duration  = treeState.getRawParameterValue(RND_DURATION_ID)->load();
    local_rnd_interval  = treeState.getRawParameterValue(RND_INTERVAL_ID)->load();
}

void ANTARCTICAAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto playhead = getPlayHead();
    if (playhead != nullptr)
    {
        currentPlayHeadState = playhead->getPosition()->getIsPlaying();
        if ((not previousPlayHeadState) and playhead->getPosition()->getIsPlaying())
            delayBuffer.clear();
        previousPlayHeadState = playhead->getPosition()->getIsPlaying();
    }
    
    bypassBuffer.makeCopyOf(buffer, false);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    if (!treeState.getRawParameterValue(BYPASS_BTN_ID)->load())
    {
        updateParams();
        
        if(treeState.getRawParameterValue(RANDOM_BTN_ID)->load())
        {
            rndIntervalCounter += 2*buffer.getNumSamples();
            if (rndIsInterval && rndIntervalCounter > int(getSampleRate() * local_rnd_interval / 1000))
            {
                assignRandomValues();
                rndIntervalCounter = 0;
                rndIsInterval = false;
            }
            if ((!rndIsInterval) && rndIntervalCounter > int(getSampleRate() * local_rnd_duration / 1000))
            {
                undoRandomAssignment();
                rndIntervalCounter = 0;
                rndIsInterval = true;
            }
        }
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            // delay
            if (treeState.getRawParameterValue(TAIL_BTN_ID)->load())
            {
                fillBuffer(buffer, channel);
                
                channelPingPongCounter += buffer.getNumSamples();
                channelPingPongCounter %= int(2*(getSampleRate() * local_delayTime / 1000));
                
                if (!
                    (
                     treeState.getRawParameterValue(FLUTTER_BTN_ID)->load()
                        &&
                     !(channel == int(channelPingPongCounter > (getSampleRate() * local_delayTime / 1000)))
                     )
                    )
                    readFromBuffer(buffer, channel); // reverse implemented inside this method
                
                fillBuffer(buffer, channel);
            }
            
            auto* channelData = buffer.getWritePointer (channel);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float toProcessVal, finalVal;
                
                //updateParam(local_input, INPUT_ID);
                channelData[sample] *= local_input;
                
                toProcessVal = finalVal = channelData[sample];
            
                //updateParam(local_gain, GAIN_ID, GAIN_BTN_ID);
                if (treeState.getRawParameterValue(GAIN_BTN_ID)->load())
                    toProcessVal *= Decibels::decibelsToGain(local_gain);
                
                //updateParam(local_drive, DRIVE_ID, DRIVE_BTN_ID);
                if (treeState.getRawParameterValue(DRIVE_BTN_ID)->load())
                    toProcessVal = tanh(local_drive * toProcessVal);
                
                //updateParam(local_dwnsmp, DWNSMP_ID, DWNSMP_BTN_ID);
                if (treeState.getRawParameterValue(DWNSMP_BTN_ID)->load())
                {
                    int step = int(buffer.getNumSamples()*(pow(1.08, local_dwnsmp)/100-0.01));
                    if (step)
                    {
                        int stepIndex = sample%step;
                        if (stepIndex)
                            toProcessVal = channelData[sample - stepIndex];
                    }
                }
                
                //updateParam(local_bit, BIT_ID, BIT_BTN_ID);
                if (treeState.getRawParameterValue(BIT_BTN_ID)->load())
                    toProcessVal -= fmodf(toProcessVal, pow(2, -(pow(1.1117,32-local_bit)+1)));
                
                // lil saturation (not parametrized)
                float saturationAmount = 2.0f;
                finalVal = toProcessVal * (1.0f + saturationAmount) / (1.0f + saturationAmount * abs(toProcessVal));
                
                channelData[sample] = finalVal;
            }
        }

        buffer.applyGain(local_drywet/100);
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
            buffer.addFrom(channel, 0, bypassBuffer, channel, 0, buffer.getNumSamples(), 1-local_drywet/100);
        
        buffer.applyGain(local_output);
        
        updateBufferPositions(buffer);
    }
}

// delay methods
void ANTARCTICAAudioProcessor::updateBufferPositions (juce::AudioBuffer<float>& buffer)
{
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    delayBufferWritePosition += bufferSize;
    delayBufferWritePosition %= delayBufferSize;
}

void ANTARCTICAAudioProcessor::fillBuffer (juce::AudioBuffer<float>& buffer, int channel, bool alternate)
{
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    float* channelData = buffer.getWritePointer ((channel+int(alternate))%2);
    if (delayBufferSize > delayBufferWritePosition + bufferSize)
        delayBuffer.copyFromWithRamp(channel, delayBufferWritePosition, channelData, bufferSize, 0.8f, 0.8f);
    else
    {
        int leftSamples = delayBufferSize - delayBufferWritePosition;
        int numSamplesAtStart = bufferSize - leftSamples;
        delayBuffer.copyFromWithRamp(channel, delayBufferWritePosition, channelData, leftSamples, 0.8f, 0.8f);
        delayBuffer.copyFromWithRamp(channel, 0, channelData, numSamplesAtStart, 0.8f, 0.8f);
    }
}

void ANTARCTICAAudioProcessor::readFromBuffer (juce::AudioBuffer<float>& buffer, int channel)
{
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    int readPosition = static_cast<int> (delayBufferWritePosition - getSampleRate() * local_delayTime / 1000);
    if (readPosition < 0)
        readPosition += delayBufferSize;
    
    if (treeState.getRawParameterValue(REV_BTN_ID)->load()) delayBuffer.reverse(channel, 0, delayBufferSize);
    if (readPosition + bufferSize < delayBufferSize)
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, local_delayAmount, local_delayAmount);
    else
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), delayBufferSize-readPosition, local_delayAmount, local_delayAmount);
        buffer.addFromWithRamp(channel, delayBufferSize-readPosition, delayBuffer.getReadPointer(channel, 0), bufferSize-delayBufferSize+readPosition, local_delayAmount, local_delayAmount);
    }
    if (treeState.getRawParameterValue(REV_BTN_ID)->load()) delayBuffer.reverse(channel, 0, delayBufferSize);
}

// random methods
void ANTARCTICAAudioProcessor::assignRandomValues()
{
    float newValue;
    
    // do a lambda function instead of this
    
    if (treeState.getRawParameterValue(DRIVE_BTN_ID)->load())
    {
        auto* param = treeState.getParameter(DRIVE_ID);
        bkpDrive = local_drive;
        newValue = rndGenerator.nextFloat()*param->convertTo0to1(local_drive);
        treeState.getParameter(DRIVE_ID)->setValueNotifyingHost(newValue);
        local_drive = param->convertFrom0to1(newValue);
    }
    if (treeState.getRawParameterValue(BIT_BTN_ID)->load())
    {
        auto* param = treeState.getParameter(BIT_ID);
        bkpBit = local_bit;
        newValue = rndGenerator.nextFloat()*param->convertTo0to1(local_bit);
        treeState.getParameter(BIT_ID)->setValueNotifyingHost(newValue);
        local_bit = param->convertFrom0to1(newValue);
    }
    if (treeState.getRawParameterValue(DWNSMP_BTN_ID)->load())
    {
        auto* param = treeState.getParameter(DWNSMP_ID);
        bkpDwnsp = local_dwnsmp;
        newValue = rndGenerator.nextFloat()*param->convertTo0to1(local_dwnsmp);
        treeState.getParameter(DWNSMP_ID)->setValueNotifyingHost(newValue);
        local_dwnsmp = param->convertFrom0to1(newValue);
    }
    if (treeState.getRawParameterValue(TAIL_BTN_ID)->load())
    {
        auto* param = treeState.getParameter(DELAYTIME_ID);
        bkpDelayTime = local_delayTime;
        newValue = rndGenerator.nextFloat()*param->convertTo0to1(local_delayTime);
        treeState.getParameter(DELAYTIME_ID)->setValueNotifyingHost(newValue);
        local_delayTime = param->convertFrom0to1(newValue);
        
        param = treeState.getParameter(DELAYAMOUNT_ID);
        bkpDelayAmount = local_delayAmount;
        newValue = rndGenerator.nextFloat()*param->convertTo0to1(local_delayAmount);
        treeState.getParameter(DELAYAMOUNT_ID)->setValueNotifyingHost(newValue);
        local_delayAmount = param->convertFrom0to1(newValue);
    }
}

void ANTARCTICAAudioProcessor::undoRandomAssignment()
{
    auto* param = treeState.getParameter(DRIVE_ID);
    local_drive = bkpDrive;
    //treeState.getParameter(DRIVE_ID)->beginChangeGesture();
    treeState.getParameter(DRIVE_ID)->setValueNotifyingHost(param->convertTo0to1(local_drive));
    //treeState.getParameter(DRIVE_ID)->endChangeGesture();
      
    param = treeState.getParameter(BIT_ID);
    local_bit = bkpBit;
    treeState.getParameter(BIT_ID)->setValueNotifyingHost(param->convertTo0to1(local_bit));
    
    param = treeState.getParameter(DWNSMP_ID);
    local_dwnsmp = bkpDwnsp;
    treeState.getParameter(DWNSMP_ID)->setValueNotifyingHost(param->convertTo0to1(local_dwnsmp));
    
    param = treeState.getParameter(DELAYTIME_ID);
    local_delayTime = bkpDelayTime;
    treeState.getParameter(DELAYTIME_ID)->setValueNotifyingHost(param->convertTo0to1(local_delayTime));
    
    param = treeState.getParameter(DELAYAMOUNT_ID);
    local_delayAmount = bkpDelayAmount;
    treeState.getParameter(DELAYAMOUNT_ID)->setValueNotifyingHost(param->convertTo0to1(local_delayAmount));
}

//==============================================================================
bool ANTARCTICAAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ANTARCTICAAudioProcessor::createEditor()
{
    return new ANTARCTICAAudioProcessorEditor (*this);
}

//==============================================================================
void ANTARCTICAAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ANTARCTICAAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ANTARCTICAAudioProcessor();
}
