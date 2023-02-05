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

#include "animatorApp.h"

#include "controlPanel.h"
#include "demoComponent.h"

class MainComponent : public juce::Component,
                      public juce::ChangeListener
{
public:
    MainComponent ();
    ~MainComponent ();

    void paint (juce::Graphics&) override;
    void resized () override;

    void changeListenerCallback (juce::ChangeBroadcaster* src) override;

private:
    void openPanel ();

    void closePanel ();

private:
    juce::ValueTree fParams;

    DemoComponent fStage;
    std::unique_ptr<ControlPanel> fControls;

    friz::Animator fPanelAnimator;

    enum PanelState
    {
        kClosed = 0,
        kOpen,
        kOpening,
        kClosing
    };

    PanelState fPanelState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
