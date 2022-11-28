/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayNote.h"
#include "SwapNotes.h"
#include "NoteInfo.h"
#include <array>

//==============================================================================
/**
*/
class TimeStampAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TimeStampAudioProcessor();
    ~TimeStampAudioProcessor() override;

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
    
    Notes notes;
    juce::Array<uint> noteBuffer;
    
private:
    int noteOnCounter = 0;
    int noteOffCounter = 0;
    int noteCounter = 0;
    NoteInfo tmpNote;
    DelayNote melodyNote;
    DelayNote delayNote1;
    DelayNote delayNote2;
    DelayNote delayNote3;
    DelayNote delayNote4;
    SwapNotes swapNotes;
    juce::MidiBuffer midiBuff;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeStampAudioProcessor)
};
