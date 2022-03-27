/*
 * Copyright (c) 2019 Brett g Porter.
 */

#pragma once

#include "animatorApp.h"

class VtSlider : public juce::Component,
                 public juce::Slider::Listener
{
public:
    VtSlider (juce::ValueTree tree, float min, float max, bool isInt,
              juce::Identifier param);

    void resized () override;

    void sliderValueChanged (juce::Slider* s) override;

private:
    std::unique_ptr<juce::Slider> fSlider;
    juce::ValueTree fTree;
    juce::Identifier fParam;
    bool fIsInt;
};

class VtCheck : public juce::Component,
                public juce::Button::Listener
{
public:
    VtCheck (juce::ValueTree tree, juce::Identifier param, juce::StringRef label);

    void resized () override;

    void buttonClicked (juce::Button* b) override;

private:
    std::unique_ptr<juce::ToggleButton> fButton;
    juce::ValueTree fTree;
    juce::Identifier fParam;
};

class VtComboBox : public juce::Component,
                   public juce::ComboBox::Listener
{
public:
    VtComboBox (juce::ValueTree tree, juce::Identifier param);

    void resized () override;

    void comboBoxChanged (juce::ComboBox*) override;

    void addSelection (int itemId, juce::StringRef label);

    void update ();

private:
    std::unique_ptr<juce::ComboBox> fCombo;
    juce::ValueTree fTree;
    juce::Identifier fParam;
};

class ControlWell : public juce::Component
{
public:
    ControlWell (juce::ValueTree params);

    void resized () override;

    void paint (juce::Graphics& g) override;

private:
    void addControl (std::unique_ptr<juce::Component> control);

private:
    juce::ValueTree fTree;
    std::vector<std::unique_ptr<juce::Component>> fControls;
};

class ControlPanel : public juce::Component,
                     public juce::ChangeBroadcaster
{
public:
    ControlPanel (juce::ValueTree params);
    ~ControlPanel ();

    void paint (juce::Graphics&) override;
    void resized () override;

    void mouseDown (const juce::MouseEvent& e) override;

private:
    ControlWell fControls;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPanel)
};
