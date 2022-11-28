/*
  ==============================================================================

    DelayNote.h
    Created: 25 Nov 2022 5:51:25pm
    Author:  Mouldy Soul

  ==============================================================================
*/
#pragma once
#include "NoteInfo.h"
//#include "NoteHarmony.h"
class DelayNote
{
    public :
    
    void prepare()
    {
        for (int i = 0; i < 127; i ++)
        {
            noteKeys[i] = 0; // Clear note keys
        }
    }
    
    void process(juce::MidiBuffer& midiMessages, int transposeAmount,
                 Notes& notesInfo, int delayValue, int& noteOnCount, int interval, bool harmony)
    {
        for (auto message : midiMessages)
        {
            auto currentMessage = message.getMessage();
            auto noteNum = currentMessage.getNoteNumber();
            int velocity = currentMessage.getVelocity();
            int position = message.samplePosition;
            
            if (currentMessage.isNoteOn())
            {
                int interNote = (noteNum) % 12; // Check the pitch class, what interval in the scale are we on
                
                int iv = returnVoiceAddition(interNote,false,interval); // returns distance to harmony note
                tempOnNote.velocity = velocity;
                if (harmony)
                {
                    tempOnNote.noteNum = noteNum + iv; // Add harmony value to note number
                    noteKeys[noteNum] = noteNum + iv;
                }else
                {
                    tempOnNote.noteNum = noteNum; // or don't if we want original notes through
                    noteKeys[noteNum] = noteNum;
                }
                
                tempOnNote.position = position; // not actually needed
                tempOnNote.noteOnMsg = true; // set a note on message in struct
                tempOnNote.delay = position + delayValue; // This is our time to play in the future
                tempOnNote.delayOff = -1; // Note off has't happened here, set to -1
                tempOnNote.finishedPlaying = false; // not used
                notesInfo.add(tempOnNote); // add note to stack
            }
            
            if (currentMessage.isNoteOff())
            {
                tempOffNote.noteNum = noteKeys[noteNum]; // the harmony note value is stored in an array corresponding to it's key value. (if user changes harmony value during held note, the last value will be remembered)
                tempOffNote.position = position;
                tempOffNote.noteOnMsg = false;
                tempOffNote.delayOff = position + delayValue; // set note off for the future.
                notesInfo.add(tempOffNote);
            }
        }
    }
    int noteKeys[127];
    juce::Array<int> notePairs;
    NoteInfo tempOnNote;
    NoteInfo tempOffNote;

    // this switch case is fair long, bullshit, could be a look up table instead, but not important for now.
    int returnHarmonyAddition(int interNote, bool majorOn, int intervalSpacing)
    {
        int intervalAddition;
        
        switch(intervalSpacing)
        {
            case 3:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 4;
                            break;
                        case 1:
                            intervalAddition = 3;
                            break;
                        case 2:
                            intervalAddition = 3;
                            break;
                        case 3:
                            intervalAddition = 3;
                            break;
                        case 4:
                            intervalAddition = 3;
                            break;
                        case 5:
                            intervalAddition = 4;
                        case 6:
                            intervalAddition = 4;
                            break;
                        case 7:
                            intervalAddition = 4; // or 2?
                            break;
                        case 8:
                            intervalAddition = 4; // or 2?
                            break;
                        case 9:
                            intervalAddition = 3;
                            break;
                        case 10:
                            intervalAddition = 3;
                            break;
                        case 11:
                            intervalAddition = 3;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 3;
                            break;
                        case 1:
                            intervalAddition = 3;
                            break;
                        case 2:
                            intervalAddition = 3;
                            break;
                        case 3:
                            intervalAddition = 4;
                            break;
                        case 4:
                            intervalAddition = 4;
                            break;
                        case 5:
                            intervalAddition = 3;
                            break;
                        case 6:
                            intervalAddition = 4;
                            break;
                        case 7:
                            intervalAddition = 3; // or 2?
                            break;
                        case 8:
                            intervalAddition = 4; // or 2?
                            break;
                        case 9:
                            intervalAddition = 3;
                            break;
                        case 10:
                            intervalAddition = 4;
                            break;
                        case 11:
                            intervalAddition = 3;
                            break;
                    }
                }
                break;
            case 4:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 7;
                            break;
                        case 1:
                            intervalAddition = 7;
                            break;
                        case 2:
                            intervalAddition = 7;
                            break;
                        case 3:
                            intervalAddition = 7;
                            break;
                        case 4:
                            intervalAddition = 7;
                            break;
                        case 5:
                            intervalAddition = 7;
                            break;
                        case 6:
                            intervalAddition = 7;
                            break;
                        case 7:
                            intervalAddition = 7; // or 2?
                            break;
                        case 8:
                            intervalAddition = 7;
                            break;
                        case 9:
                            intervalAddition = 7;
                            break;
                        case 10:
                            intervalAddition = 7;
                            break;
                        case 11:
                            intervalAddition = 6;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 7;
                            break;
                        case 1:
                            intervalAddition = 7;
                            break;
                        case 2:
                            intervalAddition = 6;
                            break;
                        case 3:
                            intervalAddition = 7;
                            break;
                        case 4:
                            intervalAddition = 7;
                            break;
                        case 5:
                            intervalAddition = 7;
                            break;
                        case 6:
                            intervalAddition = 7;
                            break;
                        case 7:
                            intervalAddition = 7; // or 2?
                            break;
                        case 8:
                            intervalAddition = 7; // or 2?
                            break;
                        case 9:
                            intervalAddition = 6;
                            break;
                        case 10:
                            intervalAddition = 7;
                            break;
                        case 11:
                            intervalAddition = 6;
                            break;
                    }
                }
                break;
            case 2:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -3;
                            break;
                        case 1:
                            intervalAddition = -3;
                            break;
                        case 2:
                            intervalAddition = -3;
                            break;
                        case 3:
                            intervalAddition = -3;
                            break;
                        case 4:
                            intervalAddition = -4;
                            break;
                        case 5:
                            intervalAddition = -3;
                            break;
                        case 6:
                            intervalAddition = -3;
                            break;
                        case 7:
                            intervalAddition = -3; // or 2?
                            break;
                        case 8:
                            intervalAddition = -3; // or 2?
                            break;
                        case 9:
                            intervalAddition = -4;
                            break;
                        case 10:
                            intervalAddition = -3;
                            break;
                        case 11:
                            intervalAddition = -4;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -4;
                            break;
                        case 1:
                            intervalAddition = -3;
                            break;
                        case 2:
                            intervalAddition = -4;
                            break;
                        case 3:
                            intervalAddition = -3;
                            break;
                        case 4:
                            intervalAddition = -3;
                            break;
                        case 5:
                            intervalAddition = -3;
                            break;
                        case 6:
                            intervalAddition = -3;
                            break;
                        case 7:
                            intervalAddition = -4; // or 2?
                            break;
                        case 8:
                            intervalAddition = -3; // or 2?
                            break;
                        case 9:
                            intervalAddition = -3;
                            break;
                        case 10:
                            intervalAddition = -3;
                            break;
                        case 11:
                            intervalAddition = -3;
                            break;
                    }
                }
                break;
            case 1:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -7;
                            break;
                        case 1:
                            intervalAddition = -7;
                            break;
                        case 2:
                            intervalAddition = -7;
                            break;
                        case 3:
                            intervalAddition = -7;
                            break;
                        case 4:
                            intervalAddition = -7;
                            break;
                        case 5:
                            intervalAddition = -6;
                            break;
                        case 6:
                            intervalAddition = -7;
                            break;
                        case 7:
                            intervalAddition = -7; // or 2?
                            break;
                        case 8:
                            intervalAddition = -7; // or 2?
                            break;
                        case 9:
                            intervalAddition = -7;
                            break;
                        case 10:
                            intervalAddition = -7;
                            break;
                        case 11:
                            intervalAddition = -7;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -7;
                            break;
                        case 1:
                            intervalAddition = -7;
                            break;
                        case 2:
                            intervalAddition = -7;
                            break;
                        case 3:
                            intervalAddition = -7;
                            break;
                        case 4:
                            intervalAddition = -7;
                            break;
                        case 5:
                            intervalAddition = -7;
                            break;
                        case 6:
                            intervalAddition = -7;
                            break;
                        case 7:
                            intervalAddition = -7; // or 2?
                            break;
                        case 8:
                            intervalAddition = -6; // or 2?
                            break;
                        case 9:
                            intervalAddition = -7;
                            break;
                        case 10:
                            intervalAddition = -7;
                            break;
                        case 11:
                            intervalAddition = -6;
                            break;
                    }
                }
                break;
        }
        
        
        
        return intervalAddition;
    }
    
    int returnVoiceAddition(int interNote, bool majorOn, int intervalSpacing)
    {
        int intervalAddition;
        
        switch(intervalSpacing)
        {
            case 3:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 4;
                            break;
                        case 1:
                            intervalAddition = 3;
                            break;
                        case 2:
                            intervalAddition = 2;
                            break;
                        case 3:
                            intervalAddition = 2;
                            break;
                        case 4:
                            intervalAddition = 3;
                            break;
                        case 5:
                            intervalAddition = 2;
                        case 6:
                            intervalAddition = 4;
                            break;
                        case 7:
                            intervalAddition = 5; // or 2?
                            break;
                        case 8:
                            intervalAddition = 4; // or 2?
                            break;
                        case 9:
                            intervalAddition = 3;
                            break;
                        case 10:
                            intervalAddition = 2;
                            break;
                        case 11:
                            intervalAddition = 1;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 4;
                            break;
                        case 1:
                            intervalAddition = 3;
                            break;
                        case 2:
                            intervalAddition = 2;
                            break;
                        case 3:
                            intervalAddition = 1;
                            break;
                        case 4:
                            intervalAddition = 5;
                            break;
                        case 5:
                            intervalAddition = 4;
                            break;
                        case 6:
                            intervalAddition = 3;
                            break;
                        case 7:
                            intervalAddition = 2; // or 2?
                            break;
                        case 8:
                            intervalAddition = 1; // or 2?
                            break;
                        case 9:
                            intervalAddition = 3;
                            break;
                        case 10:
                            intervalAddition = 2;
                            break;
                        case 11:
                            intervalAddition = 1;
                            break;
                    }
                }
                break;
            case 4:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 7;
                            break;
                        case 1:
                            intervalAddition = 6;
                            break;
                        case 2:
                            intervalAddition = 5;
                            break;
                        case 3:
                            intervalAddition = 9;
                            break;
                        case 4:
                            intervalAddition = 8;
                            break;
                        case 5:
                            intervalAddition = 7;
                            break;
                        case 7:
                            intervalAddition = 9; // or 2?
                            break;
                        case 9:
                            intervalAddition = 7;
                            break;
                        case 11:
                            intervalAddition = 5;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = 9;
                            break;
                        case 1:
                            intervalAddition = 8;
                            break;
                        case 2:
                            intervalAddition = 7;
                            break;
                        case 3:
                            intervalAddition = 6;
                            break;
                        case 4:
                            intervalAddition = 8;
                            break;
                        case 5:
                            intervalAddition = 7;
                            break;
                        case 6:
                            intervalAddition = 6;
                            break;
                        case 7:
                            intervalAddition = 5; // or 2?
                            break;
                        case 8:
                            intervalAddition = 4; // or 2?
                            break;
                        case 9:
                            intervalAddition = 7;
                            break;
                        case 10:
                            intervalAddition = 6;
                            break;
                        case 11:
                            intervalAddition = 5;
                            break;
                    }
                }
                break;
            case 2:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -5;
                            break;
                        case 1:
                            intervalAddition = -6;
                            break;
                        case 2:
                            intervalAddition = -2;
                            break;
                        case 3:
                            intervalAddition = -3;
                            break;
                        case 4:
                            intervalAddition = -4;
                            break;
                        case 5:
                            intervalAddition = -1;
                            break;
                        case 7:
                            intervalAddition = -3; // or 2?
                            break;
                        case 9:
                            intervalAddition = -2;
                            break;
                        case 11:
                            intervalAddition = -4;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -3;
                            break;
                        case 1:
                            intervalAddition = -1;
                            break;
                        case 2:
                            intervalAddition = -2;
                            break;
                        case 3:
                            intervalAddition = -3;
                            break;
                        case 4:
                            intervalAddition = -4;
                            break;
                        case 5:
                            intervalAddition = -1;
                            break;
                        case 6:
                            intervalAddition = -2;
                            break;
                        case 7:
                            intervalAddition = -3; // or 2?
                            break;
                        case 8:
                            intervalAddition = -4; // or 2?
                            break;
                        case 9:
                            intervalAddition = -5;
                            break;
                        case 10:
                            intervalAddition = -1;
                            break;
                        case 11:
                            intervalAddition = -2;
                            break;
                    }
                }
                break;
            case 1:
                if (majorOn)
                {
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -8;
                            break;
                        case 1:
                            intervalAddition = -8;
                            break;
                        case 2:
                            intervalAddition = -7;
                            break;
                        case 3:
                            intervalAddition = -8;
                            break;
                        case 4:
                            intervalAddition = -9;
                            break;
                        case 5:
                            intervalAddition = -5;
                            break;
                        case 7:
                            intervalAddition = -7; // or 2?
                            break;
                        case 9:
                            intervalAddition = -5;
                            break;
                        case 11:
                            intervalAddition = -7;
                            break;
                    }
                }
                
                if (!majorOn)
                {
                    
                    switch(interNote)
                    {
                        case 0:
                            intervalAddition = -8;
                            break;
                        case 1:
                            intervalAddition = -4;
                            break;
                        case 2:
                            intervalAddition = -5;
                            break;
                        case 3:
                            intervalAddition = -6;
                            break;
                        case 4:
                            intervalAddition = -7;
                            break;
                        case 5:
                            intervalAddition = -5;
                            break;
                        case 6:
                            intervalAddition = -6;
                            break;
                        case 7:
                            intervalAddition = -7; // or 2?
                            break;
                        case 8:
                            intervalAddition = -8; // or 2?
                            break;
                        case 9:
                            intervalAddition = -9;
                            break;
                        case 10:
                            intervalAddition = -6;
                            break;
                        case 11:
                            intervalAddition = -7;
                            break;
                    }
                }
                break;
        }
        
        
        
        return intervalAddition;
    }

};
