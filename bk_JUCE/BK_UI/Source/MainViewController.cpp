/*
  ==============================================================================

    MainViewController.cpp
    Created: 27 Mar 2017 2:04:26pm
    Author:  Michael R Mulshine

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainViewController.h"

//==============================================================================
MainViewController::MainViewController (BKAudioProcessor& p):
processor (p),
galvc(p),
kvc(p),
tvc(p),
gvc(p),
svc(p),
nvc(p),
dvc(p),
ovc(p),
timerCallbackCount(0)
{
    for (int i = 0; i < cDisplayNames.size(); i++)
    {
        buttons.add(new TextButton());
        
        buttons[i]->setName(cDisplayNames[i]);
        buttons[i]->setButtonText(cDisplayNames[i]);
        
        buttons[i]->changeWidthToFitText();
        buttons[i]->addListener(this);
        addAndMakeVisible(buttons[i]);
        
    }

    addAndMakeVisible(galvc);
    
    addAndMakeVisible(gvc);
    addAndMakeVisible(svc);
    addAndMakeVisible(nvc);
    addAndMakeVisible(dvc);
    addAndMakeVisible(kvc);
    addAndMakeVisible(tvc);
    addAndMakeVisible(ovc);
    
    addAndMakeVisible (levelMeterComponentL = new BKLevelMeterComponent());
    addAndMakeVisible (levelMeterComponentR = new BKLevelMeterComponent());
    
    
    setCurrentDisplay(DisplayDirect);
    
    
    startTimerHz (50);
    
}

MainViewController::~MainViewController()
{
    
}

void MainViewController::bkButtonClicked            (Button* b)
{
    String name = b->getName();
    if (name == cDisplayNames[DisplayKeymap])
    {
        setCurrentDisplay(DisplayKeymap);
    }
    else if (name == cDisplayNames[DisplayDirect])
    {
        setCurrentDisplay(DisplayDirect);
    }
    else if (name == cDisplayNames[DisplaySynchronic])
    {
        setCurrentDisplay(DisplaySynchronic);
    }
    else if (name == cDisplayNames[DisplayNostalgic])
    {
        setCurrentDisplay(DisplayNostalgic);
    }
    else if (name == cDisplayNames[DisplayTempo])
    {
        setCurrentDisplay(DisplayTempo);
    }
    else if (name == cDisplayNames[DisplayTuning])
    {
        setCurrentDisplay(DisplayTuning);
    }
    else if (name == cDisplayNames[DisplayGeneral])
    {
        setCurrentDisplay(DisplayGeneral);
    }
}

void MainViewController::timerCallback()
{
    
    if (++timerCallbackCount >= 100)
    {
        timerCallbackCount = 0;
        processor.collectGalleries();
        galvc.fillGalleryCB();
    }
    
    galvc.update();
    
    if (processor.updateState->generalSettingsDidChange)
    {
        processor.updateState->generalSettingsDidChange = false;
        gvc.updateFields();
    }
    
    if (processor.updateState->directPreparationDidChange)
    {
        processor.updateState->directPreparationDidChange = false;
        dvc.updateFields();
    }
    
    if (processor.updateState->nostalgicPreparationDidChange)
    {
        processor.updateState->nostalgicPreparationDidChange = false;
        nvc.updateFields();
    }
    
    if (processor.updateState->synchronicPreparationDidChange)
    {
        processor.updateState->synchronicPreparationDidChange = false;
        svc.updateFields();
    }
    
    if (processor.updateState->tuningPreparationDidChange)
    {
        processor.updateState->tuningPreparationDidChange = false;
        tvc.updateFields();
    }
    
    if (processor.updateState->tempoPreparationDidChange)
    {
        processor.updateState->tempoPreparationDidChange = false;
        ovc.updateFields();
    }
    
    if (processor.updateState->keymapDidChange)
    {
        processor.updateState->keymapDidChange = false;
        kvc.reset();
    }
    
    if (processor.updateState->directDidChange)
    {
        processor.updateState->directDidChange = false;
        dvc.reset();
    }
    
    levelMeterComponentL->updateLevel(processor.getLevelL());
    levelMeterComponentR->updateLevel(processor.getLevelL());
    
}

void MainViewController::paint (Graphics& g)
{
    g.fillAll(Colours::dimgrey);
}

void MainViewController::setCurrentDisplay(BKPreparationDisplay type)
{
    currentDisplay = type;
    
    kvc.setVisible(false);
    gvc.setVisible(false);
    tvc.setVisible(false);
    dvc.setVisible(false);
    svc.setVisible(false);
    nvc.setVisible(false);
    ovc.setVisible(false);
    
    if (type == DisplayKeymap)
    {
        kvc.setVisible(true);
    }
    else if (type == DisplayTuning)
    {
        tvc.setVisible(true);
    }
    else if (type == DisplayTempo)
    {
        ovc.setVisible(true);
    }
    else if (type == DisplaySynchronic)
    {
        svc.setVisible(true);
    }
    else if (type == DisplayNostalgic)
    {
        nvc.setVisible(true);
    }
    else if (type == DisplayDirect)
    {
        dvc.setVisible(true);
    }
    else if (type == DisplayGeneral)
    {
        gvc.setVisible(true);
    }
        
}
#define SOME_PADDING 100
void MainViewController::resized()
{
    galvc.setBounds(0, 0, gVCWidth+2*gXSpacing, getHeight());
    
    // Place buttons.
    float buttonWidth = ((getRight()-galvc.getRight() - 7 * gXSpacing - SOME_PADDING)/7.0f);
    float buttonHeight = 30;
    buttons[0]->setBounds(galvc.getRight()+gXSpacing, gYSpacing, buttonWidth, buttonHeight);
    for (int i = 1; i < cDisplayNames.size(); i++)
    {
        buttons[i]->setBounds(buttons[i-1]->getRight()+gXSpacing, gYSpacing, buttonWidth, buttonHeight);
    }
    
    /*
    float kvcH = cKeymapParameterTypes.size() * (gComponentTextFieldHeight + gYSpacing) + 1.5 * gYSpacing;
    float gvcH = cGeneralParameterTypes.size() * (gComponentTextFieldHeight + gYSpacing) + 1.5 * gYSpacing;
    float svcH = cSynchronicParameterTypes.size() * (gComponentTextFieldHeight + gYSpacing) + 1.5 * gYSpacing;
    float nvcH = cNostalgicParameterTypes.size()  * (gComponentTextFieldHeight + gYSpacing) + 1.5 *  gYSpacing;
    float dvcH = cDirectParameterTypes.size()  * (gComponentTextFieldHeight + gYSpacing) + 1.5 * gYSpacing;
    float tvcH = cTuningParameterTypes.size()  * (gComponentTextFieldHeight + gYSpacing) + gYSpacing;
    float ovcH = cTempoParameterTypes.size()  * (gComponentTextFieldHeight + gYSpacing) + gYSpacing;
    */
    
    kvc.setBounds(galvc.getRight() + gXSpacing,
                  buttons[0]->getBottom() + gYSpacing,
                  getRight() - galvc.getRight() - 2*gXSpacing - SOME_PADDING,
                  getHeight() - 200);
    
    gvc.setBounds(kvc.getBounds());
    tvc.setBounds(kvc.getBounds());
    dvc.setBounds(kvc.getBounds());
    svc.setBounds(kvc.getBounds());
    nvc.setBounds(kvc.getBounds());
    ovc.setBounds(kvc.getBounds());
    
    float levelMeterHeight = 200;
    float levelMeterWidth = 20;
    //levelMeterComponent->setBounds(galvc.getRight() + gXSpacing, getBottom()-1.5*levelMeterHeight, levelMeterWidth, levelMeterHeight);
    levelMeterComponentL->setBounds(kvc.getRight() + gXSpacing,
                                    kvc.getBottom() - levelMeterHeight,
                                    levelMeterWidth,
                                    levelMeterHeight);
    
    levelMeterComponentR->setBounds(kvc.getRight() + gXSpacing + levelMeterWidth,
                                    kvc.getBottom() - levelMeterHeight,
                                    levelMeterWidth,
                                    levelMeterHeight);
    
}