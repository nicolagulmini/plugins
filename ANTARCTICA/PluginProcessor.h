#pragma once

#include <JuceHeader.h>
using juce::Decibels;
using juce::AudioVisualiserComponent;

//==============================================================================
/**
*/
class ANTARCTICAAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
    
private:
    float EPSILON {0.001f}; // should make it const
    float local_gain {0.0f};
    float local_drive {1.0f};
    float local_bit {0.0f};
    float local_dwnsmp {0.0f};
    float local_drywet {100.0f};
    float local_input {0.0f};
    float local_output {0.0f};
    float local_lowPass {7500.0f};
    
public:
    //==============================================================================
    ANTARCTICAAudioProcessor();
    ~ANTARCTICAAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState treeState;
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
private:
    void updateLowPassFilter(float freq);
    void updateParam(float& localParam, String ID_PARAM, String ID_BTN);

    float lastSampleRate;
    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> afterProcessingLowPassFilter;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ANTARCTICAAudioProcessor)
};
