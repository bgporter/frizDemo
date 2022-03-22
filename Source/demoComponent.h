/*
  ==============================================================================

    demoComponent.h
    Created: 30 Aug 2019 4:22:45pm
    Author:  Brett Porter

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "breadcrumbs.h"

class DemoBox;

class DemoComponent : public juce::Component
{
public:
    enum class EffectType
    {
        kLinear = 0,
        kParametric,
        kEaseIn,
        kEaseOut,
        kSpring,
        kInOut
    };
    DemoComponent (juce::ValueTree params);
    ~DemoComponent ();

    void paint (juce::Graphics&) override;
    void resized () override;

    void mouseDown (const juce::MouseEvent& e) override;

    void CreateDemo (juce::Point<int> startPoint, EffectType type);

    void Clear ();

private:
    void DeleteBox (DemoBox* box);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)

    juce::ValueTree fParams;

    friz::Animator fAnimator;
    Breadcrumbs fBreadcrumbs;

    std::vector<std::unique_ptr<DemoBox>> fBoxList;

    int fNextEffectId;
};
