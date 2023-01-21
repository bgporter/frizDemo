/*
 * Copyright (c) 2019 Brett g Porter.
 */

#include "MainComponent.h"

namespace
{
const int kOpenPanelWidth { 230 };
const int kClosedPanelWidth { 30 };
} // namespace

//==============================================================================
MainComponent::MainComponent ()
: fParams (ID::kParameters)
, fStage (fParams)
, fPanelState (PanelState::kOpen)
{
    fParams.setProperty (ID::kBreadcrumbs, true, nullptr);
    fParams.setProperty (ID::kDuration, 50, nullptr);
    fParams.setProperty (ID::kEaseOutToleranceX, 0.6f, nullptr);
    fParams.setProperty (ID::kEaseOutToleranceY, 0.6f, nullptr);
    fParams.setProperty (ID::kEaseOutSlewX, 1.2f, nullptr);
    fParams.setProperty (ID::kEaseOutSlewY, 1.2f, nullptr);
    fParams.setProperty (ID::kEaseInToleranceX, 0.01f, nullptr);
    fParams.setProperty (ID::kEaseInToleranceY, 0.01f, nullptr);
    fParams.setProperty (ID::kEaseInSlewX, 0.5f, nullptr);
    fParams.setProperty (ID::kEaseInSlewY, 0.5f, nullptr);

    fParams.setProperty (ID::kSpringDampingX, 0.5f, nullptr);
    fParams.setProperty (ID::kSpringDampingY, 0.5f, nullptr);
    fParams.setProperty (ID::kSpringToleranceX, 0.5f, nullptr);
    fParams.setProperty (ID::kSpringToleranceY, 0.5f, nullptr);

    fParams.setProperty (ID::kFadeDelay, 50, nullptr);
    fParams.setProperty (ID::kFadeDuration, 150, nullptr);

    addAndMakeVisible (fStage);

    fControls.reset (new ControlPanel (fParams));

    addAndMakeVisible (fControls.get ());
    fControls->addChangeListener (this);
    setSize (1000, 740);
}

MainComponent::~MainComponent ()
{
    fControls->removeChangeListener (this);
    fControls = nullptr;
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid
    // colour)
    g.fillAll (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized ()
{
    const auto bounds = getLocalBounds ();
    fStage.setBounds (bounds);

    if (fPanelState != PanelState::kClosing || fPanelState != kOpening)
    {
        // handwave past the possibility of resizing while the panel is moving.
        int panelWidth = kOpenPanelWidth;
        int showing =
            (PanelState::kClosed == fPanelState) ? kClosedPanelWidth : panelWidth;

        int xPos = bounds.getWidth () - showing;
        fControls->setBounds (xPos, 0, kOpenPanelWidth, bounds.getHeight ());
    }
}

void MainComponent::changeListenerCallback (juce::ChangeBroadcaster* src)
{
    if (src == fControls.get ())
    {
        // user clicked on panel -- open or close it.
        if (PanelState::kOpen == fPanelState)
        {
            closePanel ();
        }
        else if (PanelState::kClosed == fPanelState)
        {
            openPanel ();
        }
        // else, we're already in motion, do nothing.
    }
}

void MainComponent::openPanel ()
{
    jassert (PanelState::kClosed == fPanelState);
    int width = getWidth ();

    const auto startX = static_cast<float> (fControls->getX ());
    const auto endX   = static_cast<float> (width - kOpenPanelWidth);

    float slew = 0.4f;

    using fx = friz::Animation<1>::SourceList;

    // add a quick anticipation effect; when the mouse clicks on us, we
    // go 'inward' before popping out, like there's the other part of the spring
    // we bounds against when the panel is closed.
    auto clickIn = std::make_unique<friz::Animation<1>> (
        fx { std::make_unique<friz::EaseIn> (startX, startX + 10.f, 0.05f, 0.7f) });

    auto popOut = std::make_unique<friz::Animation<1>> (friz::Animation<1>::SourceList {
        std::make_unique<friz::EaseIn> (startX, endX, 0.4f, slew) });

    auto animation = std::make_unique<friz::Sequence<1>> ();
    animation->AddAnimation (std::move (clickIn));
    animation->AddAnimation (std::move (popOut));

    animation->OnUpdate (
        [=] (int /*id*/, const friz::Animation<1>::ValueList& val)
        { fControls->setTopLeftPosition (static_cast<int> (val[0]), 0); });

    animation->OnCompletion ([=] (int /*id*/) { fPanelState = PanelState::kOpen; });

    fPanelState = PanelState::kOpening;
    fPanelAnimator.AddAnimation (std::move (animation));
}

void MainComponent::closePanel ()
{
    jassert (PanelState::kOpen == fPanelState);
    int width = getWidth ();

    const auto startX = static_cast<float> (fControls->getX ());
    const auto endX   = static_cast<float> (width - kClosedPanelWidth);

    float accel  = 1.4f;
    float dampen = 0.4f;

    auto curve     = std::make_unique<friz::Spring> (startX, endX, 0.5f, accel, dampen);
    auto animation = std::make_unique<friz::Animation<1>> (
        friz::Animation<1>::SourceList { std::move (curve) }, 0);

    animation->OnUpdate (
        [=] (int /*id*/, const friz::Animation<1>::ValueList& val)
        { fControls->setTopLeftPosition (static_cast<int> (val[0]), 0); });

    animation->OnCompletion ([=] (int /*id*/) { fPanelState = PanelState::kClosed; });

    fPanelState = PanelState::kClosing;
    fPanelAnimator.AddAnimation (std::move (animation));
}
