/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GraphicDelayAudioProcessor::GraphicDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

GraphicDelayAudioProcessor::~GraphicDelayAudioProcessor()
{
}

//==============================================================================
const juce::String GraphicDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GraphicDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GraphicDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GraphicDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GraphicDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GraphicDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GraphicDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GraphicDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GraphicDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void GraphicDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GraphicDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayBuffer.setSize(getTotalNumInputChannels(), (int) sampleRate * 2); // 2 seconds of circular buffer
    delayBuffer.clear(); // to avoid bad effects
}

void GraphicDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GraphicDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GraphicDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto playhead = getPlayHead();
    if (playhead != nullptr) // may not always exist
    {
        currentPlayHeadState = playhead->getPosition()->getIsPlaying();
        if ((not previousPlayHeadState) and playhead->getPosition()->getIsPlaying())
            delayBuffer.clear();
        previousPlayHeadState = playhead->getPosition()->getIsPlaying();
    }
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        fillBuffer(buffer, channel);
        readFromBuffer(buffer, channel); // reverse implemented inside this method
        fillBuffer(buffer, channel);
    }
    updateBufferPositions(buffer);
    
}

void GraphicDelayAudioProcessor::updateBufferPositions (juce::AudioBuffer<float>& buffer)
{
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    delayBufferWritePosition += bufferSize;
    delayBufferWritePosition %= delayBufferSize;
}

void GraphicDelayAudioProcessor::fillBuffer (juce::AudioBuffer<float>& buffer, int channel)
{
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    float* channelData = buffer.getWritePointer (channel);
    if (delayBufferSize > delayBufferWritePosition + bufferSize)
        delayBuffer.copyFrom(channel, delayBufferWritePosition, channelData, bufferSize);
    else
    {
        int leftSamples = delayBufferSize - delayBufferWritePosition;
        int numSamplesAtStart = bufferSize - leftSamples;
        delayBuffer.copyFrom(channel, delayBufferWritePosition, channelData, leftSamples);
        delayBuffer.copyFrom(channel, 0, channelData, numSamplesAtStart);
    }
}

void GraphicDelayAudioProcessor::readFromBuffer (juce::AudioBuffer<float>& buffer, int channel)
{
    int bufferSize = buffer.getNumSamples();
    int delayBufferSize = delayBuffer.getNumSamples();
    
    int readPosition = static_cast<int> (delayBufferWritePosition - getSampleRate() * delayTime / 1000);
    if (readPosition < 0)
        readPosition += delayBufferSize;
    
    if (reverseDelay) delayBuffer.reverse(channel, 0, delayBufferSize);
    if (readPosition + bufferSize < delayBufferSize)
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, amountDelay, amountDelay);
    else
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), delayBufferSize-readPosition, amountDelay, amountDelay);
        buffer.addFromWithRamp(channel, delayBufferSize-readPosition, delayBuffer.getReadPointer(channel, 0), bufferSize-delayBufferSize+readPosition, amountDelay, amountDelay);
    }
    if (reverseDelay) delayBuffer.reverse(channel, 0, delayBufferSize);
}

//==============================================================================
bool GraphicDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GraphicDelayAudioProcessor::createEditor()
{
    return new GraphicDelayAudioProcessorEditor (*this);
}

//==============================================================================
void GraphicDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GraphicDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GraphicDelayAudioProcessor();
}
