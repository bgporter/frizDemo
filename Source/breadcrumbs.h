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
