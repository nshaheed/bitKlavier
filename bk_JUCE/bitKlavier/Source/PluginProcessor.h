
#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "BKUtilities.h"

#include "BKGalleryLoader.h"

#include "BKSampleLoader.h"

#include "BKSynthesiser.h"

#include "BKUpdateState.h"

#include "Keymap.h"

#include "Tuning.h"

#include "Tempo.h"

#include "General.h"

#include "Piano.h"

#include "Gallery.h"

#include "ItemMapper.h"
//==============================================================================
/**
*/
class BKAudioProcessor  : public AudioProcessor,
                           public ChangeListener
{
    
public:
    //==============================================================================
    BKAudioProcessor();
    ~BKAudioProcessor();
    
    void loadGalleryDialog(void);
    void loadJsonGalleryDialog(void);
    void loadGalleryFromPath(String path);
    void loadGalleryFromXml(ScopedPointer<XmlElement> xml);
    void loadJsonGalleryFromPath(String path);
    void saveGalleryAs(void);
    void saveGallery(void);
    void createGalleryWithName(String name);
    void createNewGallery(String name);
    void renameGallery(String name);
    void deleteGallery(void);
    void deleteGalleryWithName(String name);
    
    String firstGallery(void);
    void initializeGallery(void);
    
    BKSampleLoadType currentSampleType;
    
    Gallery::Ptr                        gallery;
    
    BKUpdateState::Ptr                  updateState;

    // Synthesisers.
    BKSynthesiser                       mainPianoSynth;
    BKSynthesiser                       hammerReleaseSynth;
    BKSynthesiser                       resonanceReleaseSynth;
    
    Piano::Ptr                          prevPiano;
    Piano::Ptr                          currentPiano;
    Piano::PtrArr                       prevPianos;
    
    StringArray mikroetudes, ns_etudes, bk_examples;
    
    StringArray                         galleryNames;
    String                              currentGallery;
    String                              currentGalleryPath;
    
    bool                                defaultLoaded;
    String                              defaultName;
    
#if TRY_UNDO
    Piano::PtrArr                       history;
    int epoch;
    
    void updateHistory(void);
    
    void timeTravel(bool forward);
#endif
    
    void updateGalleries(void);
    
    void collectGalleries(void);
    void collectGalleriesFromFolder(File folder);
    
    void updateUI(void);
    
    Array<bool>                         noteOn; //which notes are on, for the UI
    Array<bool>                         getNoteOns() { return noteOn; }
    
    void                                noteOnUI (int noteNumber) { if(didLoadMainPianoSamples) notesOnUI.add(noteNumber); }
    void                                noteOffUI(int noteNumber) { if(didLoadMainPianoSamples) notesOffUI.add(noteNumber); }
    
    int                                 noteOnCount;
    bool                                allNotesOff;

    int channel;

    // Change listener callback implementation
    void changeListenerCallback(ChangeBroadcaster *source) override;
    
    
    //==============================================================================
    void loadPianoSamples(BKSampleLoadType type);
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    
    void  setCurrentPiano(int which);
    void  performModifications(int noteNumber);
    void  performResets(int noteNumber);
    
    void handleNoteOn(int noteNumber, float velocity, int channel);
    void handleNoteOff(int noteNumber, float velocity, int channel);

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    double getLevelL();
    double getLevelR();

    /*
    void saveOnClose() override
    {
        DBG("SAVE ON CLOSE CALLED");
        saveGallery();
    }
    

    bool isDirty() override
    {
        return gallery->isGalleryDirty();
    }
     */


    BKItem::PtrArr clipboard;
    
    inline void setClipboard(BKItem::PtrArr items)
    {
        clipboard = items;
    }
    
    inline BKItem::PtrArr getClipboard(void)
    {
        return clipboard;
    }

    
    inline bool clipboardContains(BKItem::Ptr thisItem)
    {
        for (auto item : clipboard)
        {
            if (item->getType() == thisItem->getType() && item->getId() == thisItem->getId()) return true;
        }
        return false;
    }
    
    void reset(BKPreparationType type, int Id);
    void clear(BKPreparationType type, int Id);
    
    BKPlatform      platform;

    float uiScaleFactor;
    
    
    float paddingScalarX;
    float paddingScalarY;
    
    int screenWidth;
    int screenHeight;
    
    inline String getCurrentGalleryPath(void) { return currentGalleryPath;}
    
    double progress;
    double progressInc;
    bool didLoadHammersAndRes, didLoadMainPianoSamples;
    
private:
    
    int  currentPianoId;
    
    bool sustainIsDown;
    
    double bkSampleRate;
    
    BKSampleLoader loader;
    
    AudioSampleBuffer levelBuf; //for storing samples for metering/RMS calculation
    
    Array<float> tempoAlreadyLoaded;
    bool galleryDidLoad;
    
    Array<int> notesOnUI;
    Array<int> notesOffUI;
    
    File lastGalleryPath;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
