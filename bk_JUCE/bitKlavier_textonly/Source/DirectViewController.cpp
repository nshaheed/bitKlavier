/*
  ==============================================================================

    DirectViewController.cpp
    Created: 30 Nov 2016 5:00:19pm
    Author:  Michael R Mulshine

  ==============================================================================
*/

#include "DirectViewController.h"

#include "Preparation.h"

//==============================================================================
DirectViewController::DirectViewController(BKAudioProcessor& p):
processor(p),
currentDirectId(0),
currentModDirectId(0)
{
    DirectPreparation::Ptr layer = processor.gallery->getStaticDirectPreparation(currentDirectId);
    
    // Labels
    directL = OwnedArray<BKLabel>();
    directL.ensureStorageAllocated(cDirectParameterTypes.size());
    
    for (int i = 0; i < cDirectParameterTypes.size(); i++)
    {
        directL.set(i, new BKLabel());
        addAndMakeVisible(directL[i]);
        directL[i]->setName(cDirectParameterTypes[i]);
        directL[i]->setText(cDirectParameterTypes[i], NotificationType::dontSendNotification);
    }
    
    // Text Fields
    directTF = OwnedArray<BKTextField>();
    directTF.ensureStorageAllocated(cDirectParameterTypes.size());
    
    for (int i = 0; i < cDirectParameterTypes.size(); i++)
    {
        directTF.set(i, new BKTextField());
        addAndMakeVisible(directTF[i]);
        directTF[i]->addListener(this);
        directTF[i]->setName(cDirectParameterTypes[i]);
    }
    
    modDirectTF = OwnedArray<BKTextField>();
    modDirectTF.ensureStorageAllocated(cDirectParameterTypes.size());
    
    for (int i = 0; i < cDirectParameterTypes.size(); i++)
    {
        modDirectTF.set(i, new BKTextField());
        addAndMakeVisible(modDirectTF[i]);
        modDirectTF[i]->addListener(this);
        modDirectTF[i]->setName("M"+cDirectParameterTypes[i]);
    }
    
    updateModFields();
    updateFields();
}

DirectViewController::~DirectViewController()
{
}

void DirectViewController::paint (Graphics& g)
{
    g.setColour(Colours::goldenrod);
    g.drawRect(getLocalBounds(), 1);
}

void DirectViewController::resized()
{
    // Labels
    int lY = gComponentLabelHeight + gYSpacing;
    
    float width = getWidth() * 0.25 - gXSpacing;
    
    for (int n = 0; n < cDirectParameterTypes.size(); n++)
    {
        directL[n]->setBounds(0, gYSpacing + lY * n, width, directL[0]->getHeight());
    }
    
    // Text fields
    int tfY = gComponentTextFieldHeight + gYSpacing;
    
    float height = directTF[0]->getHeight();
    width *= 1.5;
    
    for (int n = 0; n < cDirectParameterTypes.size(); n++)
    {
        directTF[n]->setBounds(directL[0]->getRight()+gXSpacing, gYSpacing + tfY * n, width, height);
        modDirectTF[n]->setBounds(directTF[0]->getRight()+gXSpacing, gYSpacing + tfY * n, width, height);
    }
}

void DirectViewController::bkTextFieldDidChange(TextEditor& tf)
{
    String text = tf.getText();
    String name = tf.getName();
    
    BKTextFieldType type = BKParameter;
    
    if (name.startsWithChar('M'))
    {
        type = BKModification;
        name = name.substring(1);
    }
    
    
    float f = text.getFloatValue();
    int i = text.getIntValue();
    
    DBG(name + ": |" + text + "|");
    
    DirectPreparation::Ptr prep = processor.gallery->getStaticDirectPreparation(currentDirectId);
    DirectPreparation::Ptr active = processor.gallery->getActiveDirectPreparation(currentDirectId);
    DirectModPreparation::Ptr mod = processor.gallery->getDirectModPreparation(currentModDirectId);
    
    if (name == cDirectParameterTypes[DirectId])
    {
        if (type == BKParameter)
        {
            int numDirect = processor.gallery->getNumDirect();
            
            if ((i+1) > numDirect)
            {
                processor.gallery->addDirect();
                currentDirectId = numDirect;
            }
            else if (i >= 0)
            {
                currentDirectId = i;
            }
            
            directTF[DirectId]->setText(String(currentDirectId), false);
            
            updateFields();
        }
        else // BKModification
        {
            int numDMod = processor.gallery->getNumDirectMod();
            
            if ((i+1) > numDMod)
            {
                processor.gallery->addDirectMod();
                currentModDirectId = numDMod;
            }
            else if (i >= 0)
            {
                currentModDirectId = i;
            }
            
            modDirectTF[DirectId]->setText(String(currentModDirectId), false);
            
            updateModFields();
        }
    }
    else if (name == cDirectParameterTypes[DirectTransposition])
    {
        if (type == BKParameter)
        {
            Array<float> transp = stringToFloatArray(text);
            prep    ->setTransposition(transp);
            active  ->setTransposition(transp);
        }
        else    //BKModification
        {
            mod     ->setParam(DirectTransposition, text);
        }
    }
    else if (name == cDirectParameterTypes[DirectGain])
    {
        if (type == BKParameter)
        {
            prep    ->setGain(f);
            active  ->setGain(f);
        }
        else    //BKModification
        {
            mod     ->setParam(DirectGain, text);
        }
    }
    else if (name == cDirectParameterTypes[DirectHammerGain])
    {
        if (type == BKParameter)
        {
            prep    ->setHammerGain(f);
            active  ->setHammerGain(f);
        }
        else    //BKModification
        {
            mod     ->setParam(DirectHammerGain, text);
        }
    }
    else if (name == cDirectParameterTypes[DirectResGain])
    {
        if (type == BKParameter)
        {
            prep    ->setResonanceGain(f);
            active  ->setResonanceGain(f);
        }
        else    //BKModification
        {
            mod     ->setParam(DirectResGain, text);
        }
    }
    else if (name == cDirectParameterTypes[DirectTuning])
    {
        if (type == BKParameter)
        {
            if (i < processor.gallery->getNumTuning())
            {
                prep    ->setTuning(processor.gallery->getTuning(i));
                active  ->setTuning(processor.gallery->getTuning(i));
            }
            else
                tf.setText("0", false);
            
            
        }
        else    //BKModification
        {
            if (i < processor.gallery->getNumTuning())
                mod     ->setParam(DirectTuning, text);
            else
                tf.setText("0", false);
        }
    }
    else
    {
        DBG("Unregistered text field entered input.");
    }
}


void DirectViewController::updateFields(void)
{
    
    DirectPreparation::Ptr prep = processor.gallery->getActiveDirectPreparation(currentDirectId);
    
    directTF[DirectTransposition]       ->setText( floatArrayToString( prep->getTransposition()), false);
    directTF[DirectGain]                ->setText( String( prep->getGain()), false);
    directTF[DirectHammerGain]          ->setText( String( prep->getHammerGain()), false);
    directTF[DirectResGain]             ->setText( String( prep->getResonanceGain()), false);
    directTF[DirectTuning]              ->setText( String( prep->getTuning()->getId()), false);

}

void DirectViewController::updateModFields(void)
{
    DirectModPreparation::Ptr prep = processor.gallery->getDirectModPreparation(currentModDirectId);
    
    modDirectTF[DirectTransposition]       ->setText( prep->getParam(DirectTransposition), false);
    modDirectTF[DirectGain]                ->setText( prep->getParam(DirectGain), false);
    modDirectTF[DirectHammerGain]          ->setText( prep->getParam(DirectHammerGain), false);
    modDirectTF[DirectResGain]             ->setText( prep->getParam(DirectResGain), false);
    modDirectTF[DirectTuning]              ->setText( prep->getParam(DirectTuning), false);
    
}

void DirectViewController::bkMessageReceived (const String& message)
{
    if (message == "direct/update")
    {
        
        updateFields();
    }
}


