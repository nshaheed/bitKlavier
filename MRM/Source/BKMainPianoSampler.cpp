/*
  ==============================================================================

    BKMainPianoSampler.cpp
    Created: 18 Oct 2016 11:52:19am
    Author:  Michael R Mulshine

  ==============================================================================
*/

#include "BKMainPianoSampler.h"

#include "AudioConstants.h"

BKMainPianoSamplerSound::BKMainPianoSamplerSound (const String& soundName,
                                AudioFormatReader& reader,
                                const BigInteger& notes,
                                const int rootMidiNote,
                                const BigInteger& velocities,
                                const double maxSampleLengthSeconds)
: name (soundName),
midiNotes (notes),
midiVelocities(velocities),
midiRootNote (rootMidiNote)
{
    rampOnSamples = 0;
    rampOffSamples = 0;
    
    sourceSampleRate = reader.sampleRate;
    
    if (sourceSampleRate <= 0 || reader.lengthInSamples <= 0)
    {
        length = 0;
        
    }
    else
    {
        length = jmin((int) reader.lengthInSamples,
                      (int) (maxSampleLengthSeconds * sourceSampleRate));
        
        data = new AudioSampleBuffer (jmin (2, (int) reader.numChannels), length + 4);
        
        reader.read(data, 0, length + 4, 0, true, true);
        
        rampOnSamples = roundToInt (aRampOnTimeSec* sourceSampleRate);
        rampOffSamples = roundToInt (aRampOffTimeSec * sourceSampleRate);
    }
}

BKMainPianoSamplerSound::~BKMainPianoSamplerSound()
{
}

bool BKMainPianoSamplerSound::appliesToNote (int midiNoteNumber)
{
    return midiNotes [midiNoteNumber];
}

bool BKMainPianoSamplerSound::appliesToVelocity(int midiNoteVelocity)
{
    return midiVelocities [midiNoteVelocity];
}

bool BKMainPianoSamplerSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

//==============================================================================
BKMainPianoSamplerVoice::BKMainPianoSamplerVoice() : pitchRatio (0.0),
sourceSamplePosition (0.0),
lgain (0.0f), rgain (0.0f),
rampOnOffLevel (0),
rampOnDelta (0),
rampOffDelta (0),
isInRampOn (false), isInRampOff (false)
{
}

BKMainPianoSamplerVoice::~BKMainPianoSamplerVoice()
{
}

bool BKMainPianoSamplerVoice::canPlaySound (BKSynthesiserSound* sound)
{
    return dynamic_cast<const BKMainPianoSamplerSound*> (sound) != nullptr;
}

void BKMainPianoSamplerVoice::startNote (const int midiNoteNumber,
                                const float velocity,
                                BKSynthesiserSound* s,
                                const int /*currentPitchWheelPosition*/)
{
    if (const BKMainPianoSamplerSound* const sound = dynamic_cast<const BKMainPianoSamplerSound*> (s))
    {
        pitchRatio = pow (2.0, (midiNoteNumber - sound->midiRootNote) / 12.0)
        * sound->sourceSampleRate / getSampleRate();
        
        sourceSamplePosition = 0.0;
        lgain = velocity;
        rgain = velocity;
        
        isInRampOn = (sound->rampOnSamples > 0);
        isInRampOff = false;
        
        if (isInRampOn)
        {
            rampOnOffLevel = 0.0f;
            rampOnDelta = (float) (pitchRatio / sound->rampOnSamples);
        }
        else
        {
            rampOnOffLevel = 1.0f;
            rampOnDelta = 0.0f;
        }
        
        if (sound->rampOffSamples > 0)
        {
            rampOffDelta = (float) (-pitchRatio / sound->rampOffSamples);
        }
        else
        {
            rampOffDelta = -1.0f;
        }
    }
    else
    {
        jassertfalse; // this object can only play BKSamplerSounds!
    }
}



void BKMainPianoSamplerVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        isInRampOn = false;
        isInRampOff = true;
    }
    else
    {
        clearCurrentNote();
    }
}

void BKMainPianoSamplerVoice::pitchWheelMoved (const int /*newValue*/)
{
}

void BKMainPianoSamplerVoice::controllerMoved (const int /*controllerNumber*/,
                                      const int /*newValue*/)
{
}

//==============================================================================
void BKMainPianoSamplerVoice::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (const BKMainPianoSamplerSound* const playingSound = static_cast<BKMainPianoSamplerSound*> (getCurrentlyPlayingSound().get()))
    {
        
        const float* const inL = playingSound->data->getReadPointer (0);
        const float* const inR = playingSound->data->getNumChannels() > 1
        ? playingSound->data->getReadPointer (1) : nullptr;
        
        float* outL = outputBuffer.getWritePointer (0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;
        
        while (--numSamples >= 0)
        {
            const int pos = (int) sourceSamplePosition;
            const float alpha = (float) (sourceSamplePosition - pos);
            const float invAlpha = 1.0f - alpha;
            
            // just using a very simple linear interpolation here..
            float l = (inL [pos] * invAlpha + inL [pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR [pos] * invAlpha + inR [pos + 1] * alpha)
            : l;
            
            l *= lgain;
            r *= rgain;
            
            if (isInRampOn)
            {
                l *= rampOnOffLevel;
                r *= rampOnOffLevel;
                
                rampOnOffLevel += rampOnDelta;
                
                if (rampOnOffLevel >= 1.0f)
                {
                    rampOnOffLevel = 1.0f;
                    isInRampOff = false;
                }
            }
            else if (isInRampOff)
            {
                l *= rampOnOffLevel;
                r *= rampOnOffLevel;
                
                rampOnOffLevel += rampOffDelta;
                
                if (rampOnOffLevel <= 0.0f)
                {
                    stopNote (0.0f, false);
                    break;
                }
            }
            
            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                *outL++ += (l + r) * 0.5f;
            }
            
            sourceSamplePosition += pitchRatio;
            
            if (sourceSamplePosition > playingSound->length)
            {
                stopNote (0.0f, false);
                break;
            }
        }
    }
}

