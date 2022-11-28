/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <boost/range/adaptor/indexed.hpp>
//==============================================================================
TimeStampAudioProcessor::TimeStampAudioProcessor()
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

TimeStampAudioProcessor::~TimeStampAudioProcessor()
{
}

//==============================================================================
const juce::String TimeStampAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TimeStampAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TimeStampAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TimeStampAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TimeStampAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TimeStampAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TimeStampAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TimeStampAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TimeStampAudioProcessor::getProgramName (int index)
{
    return {};
}

void TimeStampAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TimeStampAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayNote1.prepare(); // this prepares the transport function in delay voice
    delayNote2.prepare();
    delayNote3.prepare();
    delayNote4.prepare();
    melodyNote.prepare();
    noteBuffer.resize(1);
    notes.resize(10000);
    notes.clearQuick();
    //noteBuffer.fill(0);
}

void TimeStampAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TimeStampAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void TimeStampAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    jassert (buffer.getNumChannels() == 0);
    midiBuff.clear();
    //
    melodyNote.process(midiMessages, 0,  notes, 0, noteOnCounter,1, false);
    delayNote1.process(midiMessages, 12, notes, 44100/2, noteOnCounter,4, true);
    //delayNote2.process(midiMessages, 19, notes, 44100, noteOnCounter,2, true);
    //delayNote3.process(midiMessages, 24, notes, 44100*2, noteOnCounter,3, true);
    //delayNote4.process(midiMessages, 0,  notes, 44100*4, noteOnCounter,4, true);
    //
    for (int j = 0; j < buffer.getNumSamples(); ++j)
    {
        //for (auto& note : notes)
        for (int i = 0; i < notes.size(); ++i)
        {
            auto& note = notes.getReference(i);
            
            if (!note.noteOnMsg && note.delayOff > -1)
            {
               note.delayOff -= 1; // This is the note off delay counter, delay is set, and for every sample subtracted from untill it is time to play.
            }
            if (note.delay > -1)
            {
                note.delay -= 1; // This is the note on counter for the delay
            }

            if (note.delay == 0 && note.noteOnMsg) // note delay == 0 means it's time to play
            {
                auto message = juce::MidiMessage::noteOn(1, note.noteNum, (juce::uint8)note.velocity);
                midiBuff.addEvent(message, j);
                notes.remove(i);
                ++noteOnCounter;
                ++noteCounter;
            }
            
            if (note.delayOff == 0 && !note.noteOnMsg)
            {
                if (note.noteNum > 0 && note.noteNum < 127)
                {
                    auto message = juce::MidiMessage::noteOff(1, note.noteNum);
                    midiBuff.addEvent(message, j);
                    ++noteOffCounter;
                    --noteCounter;
                    notes.remove(i);
                }
                DBG("note Off Counter = " + juce::String(noteOffCounter));
                DBG("note On Counter = " + juce::String(noteOnCounter));
                DBG("note Counter = " + juce::String(noteCounter));
            }
            
            if (!notes.isEmpty() && note.delayOff == 0)
            {
                notes.clearQuick();
                DBG(notes.size());
            }
        }
        if (noteOnCounter <= -1)
        {
            DBG("note is hung");
            auto message = juce::MidiMessage::allNotesOff(1);//
            midiBuff.addEvent(message, j);
            noteOffCounter = 0; noteOnCounter = 0;
        }
    }

   
    midiMessages.swapWith(midiBuff);
}

//==============================================================================
bool TimeStampAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TimeStampAudioProcessor::createEditor()
{
    return new TimeStampAudioProcessorEditor (*this);
}

//==============================================================================
void TimeStampAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TimeStampAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TimeStampAudioProcessor();
}
