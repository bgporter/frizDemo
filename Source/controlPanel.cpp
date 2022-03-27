/*
 * Copyright (c) 2019 Brett g Porter.
 */

#include "controlPanel.h"

//==============================================================================
ControlPanel::ControlPanel (juce::ValueTree params)
: fControls (params)
{
    addAndMakeVisible (fControls);
}

ControlPanel::~ControlPanel () {}

void ControlPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey); // clear the background

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds (), 1); // draw an outline around the component
}

void ControlPanel::resized ()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    auto bounds = getLocalBounds ();
    bounds.removeFromLeft (30);
    fControls.setBounds (bounds);
}

void ControlPanel::mouseDown (const juce::MouseEvent& /*e*/)
{
    sendChangeMessage ();
}

VtSlider::VtSlider (juce::ValueTree tree, float min, float max, bool isInt,
                    juce::Identifier param)
: fTree (tree)
, fParam (param)
, fIsInt (isInt)
{
    fSlider.reset (new juce::Slider (param.toString ()));
    addAndMakeVisible (fSlider.get ());
    fSlider->setRange (min, max, isInt ? 1 : 0);
    fSlider->setSliderStyle (juce::Slider::LinearHorizontal);
    fSlider->setNumDecimalPlacesToDisplay (3);
    fSlider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    fSlider->addListener (this);

    float val = fTree.getProperty (param);

    fSlider->setValue (val, juce::NotificationType::dontSendNotification);
    setSize (190, 24);
}

void VtSlider::resized ()
{
    fSlider->setBounds (getLocalBounds ());
}

void VtSlider::sliderValueChanged (juce::Slider* /*s*/)
{
    if (fIsInt)
    {
        fTree.setProperty (fParam, static_cast<int> (fSlider->getValue () + 0.5f),
                           nullptr);
    }
    else
    {
        fTree.setProperty (fParam, fSlider->getValue (), nullptr);
    }
}

VtCheck::VtCheck (juce::ValueTree tree, juce::Identifier param, juce::StringRef label)
: fTree (tree)
, fParam (param)
{
    fButton.reset (new juce::ToggleButton (param.toString ()));
    addAndMakeVisible (fButton.get ());
    fButton->setButtonText (label);
    fButton->addListener (this);

    bool isSet = fTree.getProperty (param);

    fButton->setToggleState (isSet, juce::NotificationType::dontSendNotification);

    setSize (190, 24);
}

void VtCheck::resized ()
{
    fButton->setBounds (getLocalBounds ());
}

void VtCheck::buttonClicked (juce::Button* /*b*/)
{
    fTree.setProperty (fParam, fButton->getToggleState (), nullptr);
}

VtComboBox::VtComboBox (juce::ValueTree tree, juce::Identifier param)
: fTree (tree)
, fParam (param)
{
    fCombo = std::make_unique<juce::ComboBox> (param.toString ());
    addAndMakeVisible (fCombo.get ());
    fCombo->addListener (this);
    fCombo->setColour (juce::ComboBox::backgroundColourId, juce::Colour (0x00000000));
    setSize (190, 24);
}

void VtComboBox::resized ()
{
    fCombo->setBounds (getLocalBounds ());
}

void VtComboBox::comboBoxChanged (juce::ComboBox*)
{
    auto selected = fCombo->getSelectedItemIndex ();
    if (selected >= 0)
    {
        fTree.setProperty (fParam, selected, nullptr);
    }
}

void VtComboBox::addSelection (int itemId, juce::StringRef label)
{
    fCombo->addItem (label, itemId + 1);
}

void VtComboBox::update ()
{
    int index = fTree.getProperty (fParam);
    fCombo->setSelectedId (index + 1, juce::NotificationType::dontSendNotification);
}

class VtLabel : public juce::Component
{
public:
    VtLabel (bool large, juce::StringRef text)
    {
        fLabel.reset (new juce::Label (text, text));
        addAndMakeVisible (fLabel.get ());
        float fontSize = large ? 15.f : 10.f;
        fLabel->setFont (
            juce::Font (fontSize, juce::Font::plain).withTypefaceStyle ("Regular"));
        fLabel->setJustificationType (juce::Justification::centredLeft);
        fLabel->setEditable (false, false, false);
        fLabel->setColour (juce::TextEditor::textColourId, juce::Colours::black);
        fLabel->setColour (juce::TextEditor::backgroundColourId,
                           juce::Colour (0x00000000));

        setSize (190, large ? 24 : 12);
    }

    void resized () override { fLabel->setBounds (getLocalBounds ()); }

private:
    std::unique_ptr<juce::Label> fLabel;
};

ControlWell::ControlWell (juce::ValueTree params)
: fTree (params)
{
    addControl (std::make_unique<VtCheck> (fTree, ID::kBreadcrumbs, "Show Breadcrumbs"));
    addControl (std::make_unique<VtLabel> (true, "Parametric - [click]"));
    addControl (std::make_unique<VtLabel> (false, "Curve"));

    auto combo { std::make_unique<VtComboBox> (fTree, ID::kCurve) };
    combo->addSelection (friz::Parametric::kLinear, "Linear");
    combo->addSelection (friz::Parametric::kEaseInSine, "Sine (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutSine, "Sine (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutSine, "Sine (in/out)");
    combo->addSelection (friz::Parametric::kEaseInQuad, "Quad (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutQuad, "Quad (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutQuad, "Quad (in/out)");
    combo->addSelection (friz::Parametric::kEaseInCubic, "Cubic (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutCubic, "Cubic (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutCubic, "Cubic (in/out)");
    combo->addSelection (friz::Parametric::kEaseInQuartic, "Quartic (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutQuartic, "Quartic (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutQuartic, "Quartic (in/out)");
    combo->addSelection (friz::Parametric::kEaseInQuintic, "Quintic (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutQuintic, "Quintic (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutQuintic, "Quintic (in/out)");
    combo->addSelection (friz::Parametric::kEaseInExpo, "Exponential (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutExpo, "Exponential (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutExpo, "Exponential (in/out)");
    combo->addSelection (friz::Parametric::kEaseInCirc, "Circular (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutCirc, "Circular (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutCirc, "Circular (in/out)");
    combo->addSelection (friz::Parametric::kEaseInBack, "Back (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutBack, "Back (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutBack, "Back (in/out)");
    combo->addSelection (friz::Parametric::kEaseInElastic, "Elastic (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutElastic, "Elastic (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutElastic, "Elastic (in/out)");
    combo->addSelection (friz::Parametric::kEaseInBounce, "Bounce (ease in)");
    combo->addSelection (friz::Parametric::kEaseOutBounce, "Bounce (ease out)");
    combo->addSelection (friz::Parametric::kEaseInOutBounce, "Bounce (in/out)");
    combo->update (); // set the combo box to the current selection.
    addControl (std::move (combo));

    addControl (std::make_unique<VtLabel> (false, "Effect Duration"));
    addControl (std::make_unique<VtSlider> (fTree, 10.f, 50 * 5.f, true, ID::kDuration));

    addControl (std::make_unique<VtLabel> (true, "Ease In - [alt+click]"));
    addControl (std::make_unique<VtLabel> (false, "X Tolerance"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 5.f, false, ID::kEaseInToleranceX));
    addControl (std::make_unique<VtLabel> (false, "X Slew"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 0.99f, false, ID::kEaseInSlewX));
    addControl (std::make_unique<VtLabel> (false, "Y Tolerance"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 5.f, false, ID::kEaseInToleranceY));
    addControl (std::make_unique<VtLabel> (false, "Y Slew"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 0.99f, false, ID::kEaseInSlewY));

    addControl (std::make_unique<VtLabel> (true, "Ease Out - [shift+click]"));
    addControl (std::make_unique<VtLabel> (false, "X Tolerance"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 5.f, false, ID::kEaseOutToleranceX));
    addControl (std::make_unique<VtLabel> (false, "X Slew"));
    addControl (
        std::make_unique<VtSlider> (fTree, 1.01f, 1.99f, false, ID::kEaseOutSlewX));
    addControl (std::make_unique<VtLabel> (false, "Y Tolerance"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 5.f, false, ID::kEaseOutToleranceY));
    addControl (std::make_unique<VtLabel> (false, "Y Slew"));
    addControl (
        std::make_unique<VtSlider> (fTree, 1.01f, 1.99f, false, ID::kEaseOutSlewY));

#if JUCE_MAC
    addControl (std::make_unique<VtLabel> (true, "Spring - [cmd+click]"));
#else
    addControl (std::make_unique<VtLabel> (true, "Spring - [ctrl+click]"));
#endif
    addControl (std::make_unique<VtLabel> (false, "X Tolerance"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 5.f, false, ID::kSpringToleranceX));
    addControl (std::make_unique<VtLabel> (false, "X Damping"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 0.99f, false, ID::kSpringDampingX));
    addControl (std::make_unique<VtLabel> (false, "Y Tolerance"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 5.f, false, ID::kSpringToleranceY));
    addControl (std::make_unique<VtLabel> (false, "Y Damping"));
    addControl (
        std::make_unique<VtSlider> (fTree, 0.01f, 0.99f, false, ID::kSpringDampingY));

    addControl (std::make_unique<VtLabel> (true, "Fade"));
    addControl (std::make_unique<VtLabel> (false, "Fade Delay"));
    addControl (std::make_unique<VtSlider> (fTree, 0.f, 50 * 5.f, true, ID::kFadeDelay));
    addControl (std::make_unique<VtLabel> (false, "Fade Duration"));
    addControl (
        std::make_unique<VtSlider> (fTree, 10.f, 50 * 5.f, true, ID::kFadeDuration));
}

void ControlWell::addControl (std::unique_ptr<Component> control)
{
    addAndMakeVisible (control.get ());
    fControls.push_back (std::move (control));
}

void ControlWell::resized ()
{
    int yPos { 0 };
    int w = getWidth ();

    for (auto& c : fControls)
    {
        auto h = c->getHeight ();
        c->setBounds (2, yPos, w, h);
        yPos += h;
    }
}

void ControlWell::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF505050));
}
