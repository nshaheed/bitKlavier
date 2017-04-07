/*
  ==============================================================================

    PreparationMap.cpp
    Created: 17 Dec 2016 12:35:30pm
    Author:  Daniel Trueman

  ==============================================================================
*/

#include "PreparationMap.h"

PreparationMap::PreparationMap(Keymap::Ptr km,
                               int Id):
isActive(false),
Id(Id),
pKeymap(km)
{

}

PreparationMap::~PreparationMap()
{
    
}

void PreparationMap::prepareToPlay (double sr)
{
    sampleRate = sr;
}

void PreparationMap::setKeymap(Keymap::Ptr km)
{
    pKeymap = km;
}

void PreparationMap::setSynchronic(Synchronic::PtrArr s)
{
    synchronic = s;
    deactivateIfNecessary();
}

void PreparationMap::addSynchronic(Synchronic::Ptr p)
{
    synchronic.add(p);
    deactivateIfNecessary();
}

void PreparationMap::removeSynchronic(Synchronic::Ptr p)
{
    for (int i = synchronic.size(); --i >= 0;)
    {
        if (synchronic[i] == p)
        {
            synchronic.remove(i);
            break;
        }
    }
    deactivateIfNecessary();
}

void PreparationMap::setNostalgic(Nostalgic::PtrArr n)
{
    nostalgic = n;
    deactivateIfNecessary();
}

void PreparationMap::addNostalgic(Nostalgic::Ptr p)
{
    nostalgic.add(p);
    deactivateIfNecessary();
}

void PreparationMap::removeNostalgic(Nostalgic::Ptr p)
{
    for (int i = nostalgic.size(); --i >= 0;)
    {
        if (nostalgic[i] == p)
        {
            nostalgic.remove(i);
            break;
        }
    }
    deactivateIfNecessary();
}

void PreparationMap::setDirect(Direct::PtrArr d)
{
    direct = d;
    deactivateIfNecessary();
}

void PreparationMap::addDirect(Direct::Ptr p)
{
    direct.add(p);
    deactivateIfNecessary();
}

void PreparationMap::removeDirect(Direct::Ptr p)
{
    for (int i = direct.size(); --i >= 0;)
    {
        if (direct[i] == p)
        {
            direct.remove(i);
            break;
        }
    }
    deactivateIfNecessary();
}

void PreparationMap::setTempo(Tempo::PtrArr d)
{
    tempo = d;
    deactivateIfNecessary();
}

void PreparationMap::addTempo(Tempo::Ptr p)
{
    tempo.add(p);
    deactivateIfNecessary();
}

void PreparationMap::removeTempo(Tempo::Ptr p)
{
    for (int i = tempo.size(); --i >= 0;)
    {
        if (tempo[i] == p)
        {
            tempo.remove(i);
            break;
        }
    }
    deactivateIfNecessary();
}

void PreparationMap::setTuning(Tuning::PtrArr d)
{
    tuning = d;
    deactivateIfNecessary();
}

void PreparationMap::addTuning(Tuning::Ptr p)
{
    tuning.add(p);
    deactivateIfNecessary();
}

void PreparationMap::removeTuning(Tuning::Ptr p)
{
    for (int i = tuning.size(); --i >= 0;)
    {
        if (tuning[i] == p)
        {
            tuning.remove(i);
            break;
        }
    }
    deactivateIfNecessary();
}


Synchronic::PtrArr PreparationMap::getSynchronic(void)
{
    return synchronic;
}

Nostalgic::PtrArr PreparationMap::getNostalgic(void)
{
    return nostalgic;
}

Direct::PtrArr PreparationMap::getDirect(void)
{
    return direct;
}

Tuning::PtrArr PreparationMap::getTuning(void)
{
    return tuning;
}


Tempo::PtrArr PreparationMap::getTempo(void)
{
    return tempo;
}


void PreparationMap::removeAllPreparations()
{
    synchronic.clearQuick();
    nostalgic.clearQuick();
    direct.clearQuick();
    tempo.clearQuick();
    tuning.clearQuick();
    isActive = false;
}



void PreparationMap::deactivateIfNecessary()
{
    if(synchronic.size() == 0 &&
       nostalgic.size() == 0 &&
       direct.size() == 0 &&
       tuning.size() == 0 &&
       tempo.size() == 0)
    {
        isActive = false;
    }
    else
    {
        isActive = true;
    }
}


void PreparationMap::processBlock(int numSamples, int midiChannel)
{
    for (int i = tempo.size(); --i >= 0; )
        tempo[i]->processor->processBlock(numSamples, midiChannel);
    
    for (int i = tuning.size(); --i >= 0; )
        tuning[i]->processor->processBlock(numSamples);
  
    for (int i = synchronic.size(); --i >= 0; )
        synchronic[i]->processor->processBlock(numSamples, midiChannel);
    
    for (int i = nostalgic.size(); --i >= 0; )
        nostalgic[i]->processor->processBlock(numSamples, midiChannel);
    
    for (int i = direct.size(); --i >= 0; )
        direct[i]->processor->processBlock(numSamples, midiChannel);

}

//not sure why some of these have Channel and some don't; should rectify?
void PreparationMap::keyPressed(int noteNumber, float velocity, int channel)
{
    for (int i = tempo.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            tempo[i]->processor->keyPressed(noteNumber, velocity);
    }
    
    for (int i = tuning.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            tuning[i]->processor->keyPressed(noteNumber);
    }
  
    for (int i = synchronic.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            synchronic[i]->processor->keyPressed(noteNumber, velocity);
    }
    
    for (int i = nostalgic.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            nostalgic[i]->processor->keyPressed(noteNumber, velocity, channel);
    }
    
    for (int i = direct.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            direct[i]->processor->keyPressed(noteNumber, velocity, channel);
    }
    
}


void PreparationMap::keyReleased(int noteNumber, float velocity, int channel)
{
    for (int i = synchronic.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            synchronic[i]->processor->keyReleased(noteNumber, channel);
    }
    
    for (int i = nostalgic.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            nostalgic[i]->processor->keyReleased(noteNumber, channel);
    }
    
    for (int i = direct.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            direct[i]->processor->keyReleased(noteNumber, velocity, channel);
    }
    
    for (int i = tempo.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            tempo[i]->processor->keyReleased(noteNumber, channel);
    }
}

void PreparationMap::postRelease(int noteNumber, float velocity, int channel)
{
    for (int i = direct.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            direct[i]->processor->keyReleased(noteNumber, velocity, channel);
    }
    
    for (int i = nostalgic.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            nostalgic[i]->processor->keyReleased(noteNumber, channel);
    }
    
    for (int i = tempo.size(); --i >= 0; )
    {
        if (pKeymap->containsNote(noteNumber))
            tempo[i]->processor->keyReleased(noteNumber, channel);
    }
    
}
