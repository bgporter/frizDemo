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
