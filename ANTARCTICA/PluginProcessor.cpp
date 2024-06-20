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
                       ), treeState(*this, nullptr, "PARAMETERS TREESTATE", createParameterLayout()), afterProcessingLowPassFilter(dsp::IIR::Coefficients<float>::makeLowPass(44100, filterAfterProcessFreq))//, waveViewer(1)
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
    
    auto inputParams = std::make_unique<AudioParameterFloat>(ParameterID{INPUT_ID,1}, INPUT_NAME, -6.0f, 6.0f, local_input);
    params.push_back(std::move(inputParams));
    
    auto outputParams = std::make_unique<AudioParameterFloat>(ParameterID{OUTPUT_ID,1}, OUTPUT_NAME, -6.0f, 6.0f, local_output);
    params.push_back(std::move(outputParams));
    
    // buttons
    auto gainButtonParams = std::make_unique<AudioParameterBool>(ParameterID{GAIN_BTN_ID,1}, GAIN_BTN_NAME, true);
    params.push_back(std::move(gainButtonParams));
    
    auto driveButtonParams = std::make_unique<AudioParameterBool>(ParameterID{DRIVE_BTN_ID,1}, DRIVE_BTN_NAME, false);
    params.push_back(std::move(driveButtonParams));
    
    auto bitButtonParams = std::make_unique<AudioParameterBool>(ParameterID{BIT_BTN_ID,1}, BIT_BTN_NAME, true);
    params.push_back(std::move(bitButtonParams));
    
    auto dwnsmpButtonParams = std::make_unique<AudioParameterBool>(ParameterID{DWNSMP_BTN_ID,1}, DWNSMP_BTN_NAME, true);
    params.push_back(std::move(dwnsmpButtonParams));

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
    lastSampleRate = sampleRate;
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    afterProcessingLowPassFilter.prepare(spec);
    afterProcessingLowPassFilter.reset();
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

void ANTARCTICAAudioProcessor::updateLowPassFilter(float freq)
{
    *afterProcessingLowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freq);
}

void ANTARCTICAAudioProcessor::updateParam(float& localParam, String ID_PARAM, String ID_BTN)
{ // let's suppose that ID_PARAM and ID_BTN exist...
    // smooth update
    float treeStateParam = treeState.getRawParameterValue(ID_PARAM)->load();
    
    if(ID_BTN.length()>0 && !treeState.getRawParameterValue(ID_BTN)->load())
        treeStateParam = 0; // not default, just to prevent sharpness
    
    if (abs(localParam-treeStateParam) < EPSILON)
        localParam = treeStateParam;
    else if (localParam > treeStateParam)
        localParam -= EPSILON;
    else
        localParam += EPSILON;
}

void ANTARCTICAAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float toProcessVal, backupVal, finalVal, toProcessValBeforeProcessing;
            
            updateParam(local_input, INPUT_ID, "");
            channelData[sample] *= Decibels::decibelsToGain(local_input);
            
            toProcessVal = backupVal = finalVal = toProcessValBeforeProcessing = channelData[sample];
            
            updateParam(local_gain, GAIN_ID, GAIN_BTN_ID);
            if (treeState.getRawParameterValue(GAIN_BTN_ID)->load())
                toProcessVal *= Decibels::decibelsToGain(local_gain);
            
            toProcessValBeforeProcessing = toProcessVal;
            
            updateParam(local_drive, DRIVE_ID, DRIVE_BTN_ID);
            if (treeState.getRawParameterValue(DRIVE_BTN_ID)->load())
            {
                toProcessVal = tanh(local_drive * toProcessVal);
            }
            
            updateParam(local_dwnsmp, DWNSMP_ID, DWNSMP_BTN_ID);
            if (treeState.getRawParameterValue(DWNSMP_BTN_ID)->load())
            {
                int step = int(buffer.getNumSamples()*pow(1.08, local_dwnsmp)/100);
                int stepIndex = sample%step; // from 0 to step-1
                if (stepIndex)
                    toProcessVal = channelData[sample - stepIndex];
            }
            
            updateParam(local_bit, BIT_ID, BIT_BTN_ID);
            if (treeState.getRawParameterValue(BIT_BTN_ID)->load())
                toProcessVal -= fmodf(toProcessVal, pow(2, -(pow(1.1117,32-local_bit)+1)));
            
            // lil compression (not parametrized)
            if (abs(toProcessVal) > 1.5*abs(toProcessValBeforeProcessing))
            {
                float compressedSample = 1.5*toProcessValBeforeProcessing + (toProcessVal - 1.5*toProcessValBeforeProcessing) / 3.5;
                toProcessVal = (toProcessVal < 0.0f) ? -compressedSample : compressedSample;
            }
            // lil saturation (not parametrized)
            float saturationAmount = 2.0f;
            finalVal = toProcessVal * (1.0f + saturationAmount) / (1.0f + saturationAmount * abs(toProcessVal));
            
            updateParam(local_output, OUTPUT_ID, "");
            updateParam(local_drywet, DRYWET_ID, "");
            channelData[sample] = Decibels::decibelsToGain(local_output)*(finalVal*(local_drywet/100)+backupVal*(1-local_drywet/100));
             
        }
    }
             
    dsp::AudioBlock<float> block (buffer);
    updateLowPassFilter(filterAfterProcessFreq);
    afterProcessingLowPassFilter.process(dsp::ProcessContextReplacing<float>(block));
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
