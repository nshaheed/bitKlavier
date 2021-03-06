/*
  ==============================================================================

    PluginConfig.cpp
    Created: 22 Mar 2017 12:40:02pm
    Author:  Michael R Mulshine

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void BKAudioProcessor::releaseResources() {
    
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    //fileBuffer.setSize (0, 0);
    
}


#ifndef JucePlugin_PreferredChannelConfigurations
bool BKAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) {
    
    // Reject any bus arrangements that are not compatible with your plugin
    
    const int numChannels = preferredSet.size();
    
#if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
#elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
#else
    if (numChannels != 1 && numChannels != 2)
        return false;
    
    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
#endif
    
    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif


//==============================================================================
void BKAudioProcessor::changeListenerCallback(ChangeBroadcaster *source)
{
    
}

//==============================================================================
bool BKAudioProcessor::hasEditor() const
{
    
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BKAudioProcessor::createEditor()
{
    return new BKAudioProcessorEditor (*this);
}

//==============================================================================
void BKAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    if (galleryDidLoad)
    {
        
        ValueTree galleryVT(vtagGalleryPath);
        
        galleryVT.setProperty("defaultLoaded", (int)defaultLoaded, 0);
        galleryVT.setProperty("defaultName", defaultName, 0);
        
        galleryVT.setProperty("galleryPath", gallery->getURL(), 0);
        
        galleryVT.setProperty("defaultPiano", currentPiano->getId(), 0);
        
        galleryVT.setProperty("invertSustain", getSustainInversion(), 0);
        
        DBG("sustain inversion saved: " + String((int)getSustainInversion()));
        
        DBG("saving gallery and piano to plugin state: getStateInformation() "
            +  gallery->getURL() + " "
            + String(currentPiano->getId()));
        
        ScopedPointer<XmlElement> galleryXML = galleryVT.createXml();
        copyXmlToBinary (*galleryXML, destData);
        
        
        /* //this will save entire state, rather than just which Gallery/Piano is being used
        ValueTree galleryVT = gallery->getState();
    
        ScopedPointer<XmlElement> galleryXML = galleryVT.createXml();
        
        copyXmlToBinary (*galleryXML, destData);
         */
        
    }
}

void BKAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    //loadGallery
    if (galleryDidLoad)
    {
        DBG("BKAudioProcessor::setStateInformation");
        
        ScopedPointer<XmlElement> galleryXML (getXmlFromBinary (data, sizeInBytes));
        if (galleryXML != nullptr)
        {
            defaultLoaded = (bool) galleryXML->getStringAttribute("defaultLoaded").getIntValue();
            
            if (defaultLoaded)
            {
                defaultName = galleryXML->getStringAttribute("defaultName");
                
                int size;
                
                String xmlData = BinaryData::getNamedResource(defaultName.toUTF8(), size);
                
                loadGalleryFromXml(XmlDocument::parse(xmlData));
            }
            else
            {
                String currentGalleryPath = galleryXML->getStringAttribute("galleryPath");
                currentGallery = galleryXML->getStringAttribute("name");
                
                DBG("loading gallery and piano from plugin state: setStateInformation() "
                    + currentGalleryPath + " "
                    + String(galleryXML->getStringAttribute("defaultPiano").getIntValue()));
                
                loadGalleryFromPath(currentGalleryPath);
            }
            
            bool invertSustain = (bool)galleryXML->getStringAttribute("invertSustain").getIntValue();
            
            setSustainInversion(invertSustain);
            
            //override gallery-saved defaultPiano with pluginHost-saved defaultPiano
            setCurrentPiano(galleryXML->getStringAttribute("defaultPiano").getIntValue());
            
            initializeGallery();
            
        }
    
        
        /* //will load entire state rather than just galleryPath, assuming it has been saved
        ScopedPointer<XmlElement> galleryXML (getXmlFromBinary (data, sizeInBytes));
        if (galleryXML != nullptr)
        {
            gallery = new Gallery(galleryXML, *this);
            
            initializeGallery();

        }
         */
    }
}

//==============================================================================
const String BKAudioProcessor::getName() const {
    
    return JucePlugin_Name;
}

bool BKAudioProcessor::acceptsMidi() const {
    
    return true;
}

bool BKAudioProcessor::producesMidi() const {
    
    return false;
}

double BKAudioProcessor::getTailLengthSeconds() const {
    
    return 0.0;
}

int BKAudioProcessor::getNumPrograms() {
    
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int BKAudioProcessor::getCurrentProgram() {
    
    return 0;
}

void BKAudioProcessor::setCurrentProgram (int index) {
    
}

const String BKAudioProcessor::getProgramName (int index) {
    
    return String("bitKlavier");
}

void BKAudioProcessor::changeProgramName (int index, const String& newName) {
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    AudioProcessor* processor = new BKAudioProcessor();

    
    return  processor;
}
