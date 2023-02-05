/*
    Copyright (c) 2019-2023 Brett g Porter

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "breadcrumbs.h"

class DemoBox;

class DemoComponent : public juce::Component,
                      public juce::Timer
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

    void timerCallback () override;

    void createDemo (juce::Point<int> startPoint, EffectType type);

    void clear ();

private:
    DemoBox* findBox (int boxId);

    bool deleteBox (int boxId);

    void updateRate ();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)

    juce::ValueTree fParams;
    juce::TooltipWindow tooltips;
    juce::Label frameRate;

    friz::Animator fAnimator;
    Breadcrumbs fBreadcrumbs;

    std::vector<std::unique_ptr<DemoBox>> fBoxList;

    // int fNextEffectId { 0 };
};
