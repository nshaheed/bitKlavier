/*
  ==============================================================================

    GraphicsConstants.h
    Created: 23 Sep 2016 1:50:37pm
    Author:  Michael R Mulshine

  ==============================================================================
*/

#ifndef GRAPHICSCONSTANTS_H_INCLUDED
#define GRAPHICSCONSTANTS_H_INCLUDED

static const float gMainComponentFontSize = 12.0f;

#define DEFAULT_WIDTH 933
#define DEFAULT_HEIGHT 700

#define DEFAULT_MIN_WIDTH 466
#define DEFAULT_MIN_HEIGHT 290


static const int gComponentLeftOffset = 5;
static const int gComponentTopOffset = 5;
static const int gComponentYSpacing = 5;
static const int gComponentXSpacing = 5;

static const int gComponentTextFieldWidth = 80;

static const int gComponentLabelWidth = 160;



#if JUCE_IOS
extern int fontHeight;

extern int gComponentComboBoxHeight;
extern int gComponentLabelHeight;
extern int gComponentTextFieldHeight;

extern int gComponentRangeSliderHeight;
extern int gComponentSingleSliderHeight;
extern int gComponentStackedSliderHeight;

#else
static const int fontHeight = 15;

static const int gComponentComboBoxHeight = 24;
static const int gComponentLabelHeight = 17;
static const int gComponentTextFieldHeight = 17;

static const int gComponentRangeSliderHeight = 40;
static const int gComponentSingleSliderHeight = 40;
static const int gComponentStackedSliderHeight = 40;
#endif

static const int gComponentToggleBoxWidth  = gComponentComboBoxHeight;
static const int gComponentToggleBoxHeight  = gComponentToggleBoxWidth;

static const int gComponentSingleSliderXOffset  = 8;

static const int gVCWidth = 375;

static const int gXSpacing = 4;
static const int gYSpacing = 4;

static const int gPaddingConst = 10;
static const float gModAlpha = 0.4;



#endif  // GRAPHICSCONSTANTS_H_INCLUDED
