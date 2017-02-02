/*
  ==============================================================================

    PianoMap.cpp
    Created: 31 Jan 2017 3:15:28pm
    Author:  Michael R Mulshine

  ==============================================================================
*/

#include "PianoMap.h"


PianoMap::PianoMap()
{
    keymapPianoMap.ensureStorageAllocated(aMaxNumKeymaps);
    keyPianoMap.ensureStorageAllocated(128);
    
}

PianoMap::~PianoMap()
{
    
}
