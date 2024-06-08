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
                       ), treeState(*this, nullptr), afterProcessingLowPassFilter(dsp::IIR::Coefficients<float>::makeLowPass(44100, filterAfterProcessFreq))//, waveViewer(1)
#endif
{
    NormalisableRange<float> gainRange (-6.0f, 12.0f);
}

ANTARCTICAAudioProcessor::~ANTARCTICAAudioProcessor()
{
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
            float toProcessVal, backupVal, finalVal, toProcessValBeforeDistortion;
            channelData[sample] *= inputVal;
            toProcessVal = backupVal = finalVal = toProcessValBeforeDistortion = channelData[sample];

            if (gainSwitch)
                toProcessVal *= Decibels::decibelsToGain(gainVal);
            
            toProcessValBeforeDistortion = toProcessVal;
            if (distSwitch)
                toProcessVal = tanh(distVal * toProcessVal);
            
            if (abs(toProcessVal) > 1.3*abs(toProcessValBeforeDistortion))
            {
                float compressedSample = 1.3*toProcessValBeforeDistortion + (toProcessVal - 1.3*toProcessValBeforeDistortion) / 3.5;
                toProcessVal = (toProcessVal < 0.0f) ? -compressedSample : compressedSample;
            }
            
            if (bitSwitch)
                toProcessVal -= fmodf(toProcessVal, pow(2, -(pow(1.1117,32-bitVal)+1)));

            finalVal = toProcessVal;
            
            if (downSampleSwitch)
            {
                int step = int(buffer.getNumSamples()*pow(1.08, downSampleVal)/100);
                int stepIndex = sample%step; // from 0 to step-1
                finalVal = channelData[sample - stepIndex]*(1-stepIndex/(step-1))+toProcessVal*stepIndex/(step-1);
            }
            
            // lil saturation
            float saturationAmount = 2.0f;
            toProcessVal = toProcessVal * (1.0f + saturationAmount) / (1.0f + saturationAmount * abs(toProcessVal));
                        
            channelData[sample] = outputVal*(finalVal*(drywetPercentageVal/100)+backupVal*(1-drywetPercentageVal/100));
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
