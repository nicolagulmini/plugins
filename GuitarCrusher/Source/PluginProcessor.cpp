/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuitarCrusherAudioProcessor::GuitarCrusherAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), waveViewer(getTotalNumOutputChannels())
#endif
{
    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(256);
}

GuitarCrusherAudioProcessor::~GuitarCrusherAudioProcessor()
{
}

//==============================================================================
const juce::String GuitarCrusherAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GuitarCrusherAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GuitarCrusherAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GuitarCrusherAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GuitarCrusherAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GuitarCrusherAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GuitarCrusherAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GuitarCrusherAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GuitarCrusherAudioProcessor::getProgramName (int index)
{
    return {};
}

void GuitarCrusherAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GuitarCrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    waveViewer.clear();
}

void GuitarCrusherAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    waveViewer.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GuitarCrusherAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GuitarCrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float toProcessVal, backupVal, finalVal;
            channelData[sample] *= inputVal;
            toProcessVal = backupVal = finalVal = channelData[sample];

            if (gainSwitch)
            {
                toProcessVal *= Decibels::decibelsToGain(gainVal);
            }
            
            if (distSwitch)
            {
                toProcessVal = tanh(2/M_PI * atan(distVal*toProcessVal));
            }
            
            if (bitSwitch)
            {
                toProcessVal -= fmodf(toProcessVal, pow(2, -(pow(1.1117,32-bitVal)+1)));
            }
            
            finalVal = toProcessVal;
            
            if (downSampleSwitch)
            {
                int stepIndex = sample%int(buffer.getNumSamples()*pow(1.08, downSampleVal)/100);
                if (stepIndex != 0)
                {
                    finalVal = channelData[sample - stepIndex];
                }
            }
                        
            channelData[sample] = outputVal*(finalVal*(drywetPercentageVal/100)+backupVal*(1-drywetPercentageVal/100));
        }
    }
    
    waveViewer.pushBuffer(buffer);
}

//==============================================================================
bool GuitarCrusherAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GuitarCrusherAudioProcessor::createEditor()
{
    return new GuitarCrusherAudioProcessorEditor (*this);
}

//==============================================================================
void GuitarCrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GuitarCrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GuitarCrusherAudioProcessor();
}
