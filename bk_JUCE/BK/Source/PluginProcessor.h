
#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ReferenceCountedBuffer.h"

#include "Synchronic.h"
#include "Nostalgic.h"

#include "AudioConstants.h"

#include "BKSynthesiser.h"

#include "BKTextField.h"

#define USE_SYNCHRONIC_TWO 0

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
    
    
    int numSynchronicLayers;
    int currentSynchronicLayer;
    
    int numNostalgicLayers;
    int currentNostalgicLayer;

    // Sample loading.
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReader> sampleReader;
    ScopedPointer<AudioSampleBuffer> sampleBuffer;
    
    // Synthesisers.
    BKSynthesiser mainPianoSynth;
    BKSynthesiser hammerReleaseSynth;
    BKSynthesiser resonanceReleaseSynth;

    // Preparations.
    SynchronicPreparation::Ptr currentSynchronicPreparation;
    NostalgicPreparation::Ptr currentNostalgicPreparation;
    
    OwnedArray<SynchronicProcessor, CriticalSection>    sProcessor;
    OwnedArray<NostalgicProcessor, CriticalSection>     nProcessor;
    
    Array<SynchronicPreparation::Ptr, CriticalSection> sPreparation;
    Array<NostalgicPreparation::Ptr, CriticalSection> nPreparation;
    
    int channel;
    
    // Change listener callback implementation
    void changeListenerCallback(ChangeBroadcaster *source) override;
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

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
    
    

private:
    void loadMainPianoSamples(BKSynthesiser *synth, int numLayers);
    void loadResonanceRelaseSamples(BKSynthesiser *synth);
    void loadHammerReleaseSamples(BKSynthesiser *synth);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BKAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED