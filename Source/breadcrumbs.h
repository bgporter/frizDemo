/*
 * Copyright (c) 2019 Brett g Porter.
 */

#pragma once

#include "animatorApp.h"

class Breadcrumbs : public juce::Component
{
public:
    Breadcrumbs ()
    : fEnabled (true)
    {
        // ignore all mouse clicks.
        setInterceptsMouseClicks (false, false);
    }

    void enable (bool isEnabled)
    {
        clear ();
        fEnabled = isEnabled;
    }

    bool isEnabled () const { return fEnabled; }

    void clear () { fBreadcrumbs.clear (); }

    void addPoint (float x, float y)
    {
        if (fEnabled)
        {
            fBreadcrumbs.addRectangle (x, y, 2, 2);
        }
    }

    void paint (juce::Graphics& g) override
    {
        if (fEnabled)
        {
            g.setColour (juce::Colours::black);
            g.fillPath (fBreadcrumbs);
        }
    }

private:
    juce::Path fBreadcrumbs;

    bool fEnabled;
};
