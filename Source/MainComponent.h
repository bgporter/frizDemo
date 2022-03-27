/*
 * Copyright (c) 2019 Brett g Porter.
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
