/*
  ==============================================================================

    NoteInfo.h
    Created: 25 Nov 2022 9:18:28pm
    Author:  Mouldy Soul

  ==============================================================================
*/
#pragma once
#include "JuceHeader.h"
// Note Info is the main struct for note information
struct NoteInfo
{
    int velocity = 0;
    int noteNum = 0;
    int position = 0;
    int delay = -1;
    int delayOff = -1;
    bool noteOnMsg = true;
    bool finishedPlaying = false;
};

using Notes = juce::Array<NoteInfo>;
