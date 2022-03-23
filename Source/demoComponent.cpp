/*
 * Copyright (c) 2019 Brett g Porter.
 */

#include "demoComponent.h"
#include "animatorApp.h"

class DemoBox : public juce::Component
{
public:
    DemoBox ()
    {
        // juce::Random r;
        auto& r { juce::Random::getSystemRandom () };
        fFill    = juce::Colour (r.nextFloat (), 0.9f, 0.9f, 0.9f);
        int size = r.nextInt ({ 50, 100 });
        this->setSize (size, size);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (fFill);
        auto bounds = this->getLocalBounds ();
        g.setColour (juce::Colours::black);
        g.drawRect (bounds, 4);
    }

    float GetSaturation () const { return fFill.getSaturation (); }

    void SetSaturation (float newSaturation)
    {
        fFill = fFill.withSaturation (newSaturation);
        this->repaint ();
    }

public:
    juce::Colour fFill;
};

//==============================================================================
DemoComponent::DemoComponent (juce::ValueTree params)
: fParams (params)
, fAnimator (50)
, fNextEffectId { 0 }
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    this->addAndMakeVisible (fBreadcrumbs);
    fBreadcrumbs.toBack ();
}

DemoComponent::~DemoComponent ()
{
    this->Clear ();
}

void DemoComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::lightgrey);

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds (), 1); // draw an outline around the component
}

void DemoComponent::resized ()
{
    fBreadcrumbs.setBounds (this->getLocalBounds ());
}

void DemoComponent::Clear ()
{
    fAnimator.CancelAllAnimations (false);
    fBoxList.clear ();
    fBreadcrumbs.Clear ();
    this->repaint ();
}

void DemoComponent::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu ())
    {
        this->Clear ();
    }
    else
    {
        EffectType type = EffectType::kParametric;

        if (e.mods.isShiftDown ())
        {
            if (e.mods.isAltDown ())
            {
                type = EffectType::kInOut;
            }
            else
            {
                type = EffectType::kEaseOut;
            }
        }
        else if (e.mods.isAltDown ())
        {
            type = EffectType::kEaseIn;
        }
        else if (e.mods.isCommandDown ())
        {
            type = EffectType::kSpring;
        }

        this->CreateDemo (e.getPosition (), type);
    }
}

void DemoComponent::CreateDemo (juce::Point<int> startPoint, EffectType type)
{
    juce::Random r;

    bool enableCrumbs = fParams.getProperty (ID::kBreadcrumbs);
    if (enableCrumbs != fBreadcrumbs.IsEnabled ())
    {
        fBreadcrumbs.Enable (enableCrumbs);
    }

    fBreadcrumbs.Clear ();
    this->repaint ();

    auto box = new DemoBox ();
    this->addAndMakeVisible (box);
    box->setBounds (startPoint.x, startPoint.y, box->getWidth (), box->getHeight ());

    // set the animation parameters.
    auto startX = static_cast<float>(startPoint.x);
    auto endX   = static_cast<float>(r.nextInt ({ 0, this->getWidth () - box->getWidth () }));
    auto startY = static_cast<float>(startPoint.y);
    auto endY   = static_cast<float>(r.nextInt ({ 0, this->getHeight () - box->getHeight () }));

    auto movement = std::make_unique<friz::Animation<2>> (++fNextEffectId);

    std::unique_ptr<friz::AnimatedValue> xCurve;
    std::unique_ptr<friz::AnimatedValue> yCurve;

    int duration = fParams.getProperty (ID::kDuration, 50);

    if (EffectType::kLinear == type)
    {
        xCurve = std::make_unique<friz::Linear> (startX, endX, duration);
        yCurve = std::make_unique<friz::Linear> (startY, endY, duration);
    }
    else if (EffectType::kParametric == type)
    {
        int curveType =
            fParams.getProperty (ID::kCurve, friz::Parametric::CurveType::kLinear);
        xCurve = std::make_unique<friz::Parametric> (
            friz::Parametric::CurveType (curveType), startX, endX, duration);
        yCurve = std::make_unique<friz::Parametric> (
            friz::Parametric::CurveType (curveType), startY, endY, duration);
    }
    else if (EffectType::kEaseOut == type)
    {
        float xTolerance = fParams.getProperty (ID::kEaseOutToleranceX, 0.1f);
        float xSlew      = fParams.getProperty (ID::kEaseOutSlewX, 1.1f);
        xCurve = std::make_unique<friz::EaseOut> (startX, endX, xTolerance, xSlew);

        float yTolerance = fParams.getProperty (ID::kEaseOutToleranceY, 0.1f);
        float ySlew      = fParams.getProperty (ID::kEaseOutSlewY, 1.1f);
        yCurve = std::make_unique<friz::EaseOut> (startY, endY, yTolerance, ySlew);
    }
    else if (EffectType::kEaseIn == type)
    {
        float xTolerance = fParams.getProperty (ID::kEaseInToleranceX, 0.1f);
        float xSlew      = fParams.getProperty (ID::kEaseInSlewX, 1.1f);
        xCurve = std::make_unique<friz::EaseIn> (startX, endX, xTolerance, xSlew);

        float yTolerance = fParams.getProperty (ID::kEaseInToleranceY, 0.1f);
        float ySlew      = fParams.getProperty (ID::kEaseInSlewY, 1.1f);
        yCurve = std::make_unique<friz::EaseIn> (startY, endY, yTolerance, ySlew);
    }
    else if (EffectType::kSpring == type)
    {
        auto xAccel = std::abs (endX - startX) / 50.f;
        auto yAccel = std::abs (endY - startY) / 50.f;

        float tolerance = fParams.getProperty (ID::kSpringToleranceX);
        float damping   = fParams.getProperty (ID::kSpringDampingX);
        xCurve =
            std::make_unique<friz::Spring> (startX, endX, tolerance, xAccel, damping);

        tolerance = fParams.getProperty (ID::kSpringToleranceY);
        damping   = fParams.getProperty (ID::kSpringDampingY);
        yCurve =
            std::make_unique<friz::Spring> (startY, endY, tolerance, yAccel, damping);
    }
    else if (EffectType::kInOut == type)
    {
        auto midX = (startX + endX) / 2;
        auto midY = (startY + endY) / 2;

        float xTolerance = fParams.getProperty (ID::kEaseInToleranceX, 0.1f);
        float xSlew      = fParams.getProperty (ID::kEaseInSlewX, 1.1f);
        auto xCurve1 = std::make_unique<friz::EaseIn> (startX, midX, xTolerance, xSlew);

        float yTolerance = fParams.getProperty (ID::kEaseInToleranceY, 0.1f);
        float ySlew      = fParams.getProperty (ID::kEaseInSlewY, 1.1f);
        auto yCurve1 = std::make_unique<friz::EaseIn> (startY, midY, yTolerance, ySlew);

        // maybe a cleaner way to write this?
        using fx2 = friz::Animation<2>::SourceList;

        auto effect1 = std::make_unique<friz::Animation<2>> (
            fx2 { std::move (xCurve1), std::move (yCurve1) });

        xTolerance   = fParams.getProperty (ID::kEaseOutToleranceX, 0.1f);
        xSlew        = fParams.getProperty (ID::kEaseOutSlewX, 1.1f);
        auto xCurve2 = std::make_unique<friz::EaseOut> (midX, endX, xTolerance, xSlew);

        yTolerance   = fParams.getProperty (ID::kEaseOutToleranceY, 0.1f);
        ySlew        = fParams.getProperty (ID::kEaseOutSlewY, 1.1f);
        auto yCurve2 = std::make_unique<friz::EaseOut> (midY, endY, yTolerance, ySlew);
        // compare to the above that uses the alias `fx2`
        auto effect2 = std::make_unique<friz::Animation<2>> (
            friz::Animation<2>::SourceList { std::move (xCurve2), std::move (yCurve2) });
        //
        auto sequence = std::make_unique<friz::Sequence<2>> (++fNextEffectId);
        sequence->AddAnimation (std::move (effect1));
        sequence->AddAnimation (std::move (effect2));

        movement = std::move (sequence);
    }

    const int kXpos { 0 };
    const int kYpos { 1 };

    if (EffectType::kInOut != type)
    {
        movement->SetValue (kXpos, std::move (xCurve));
        movement->SetValue (kYpos, std::move (yCurve));
    }

    // On each update: move this box to the next position on the (x,y) curve.
    movement->OnUpdate (
        [=] (int /*id*/, const friz::Animation<2>::ValueList& val)
        {
            const auto x { static_cast<int> (val[kXpos]) };
            const auto y { static_cast<int> (val[kYpos]) };
            box->setTopLeftPosition (x, y);
            fBreadcrumbs.AddPoint (val[kXpos], val[kYpos]);
        });

    // When the main animation completes: start a second animation that slowly
    // fades the color all the way out.
    movement->OnCompletion (
        [=] (int /*id*/)
        {
            float currentSat = box->GetSaturation ();

            int delay = this->fParams.getProperty (ID::kFadeDelay);
            int dur   = this->fParams.getProperty (ID::kFadeDuration);

            auto fade = std::make_unique<friz::Animation<1>> (
                friz::Animation<1>::SourceList {
                    std::make_unique<friz::Linear> (currentSat, 0.f, dur) },
                ++fNextEffectId);

            // don't start fading until `delay` frames have elapsed
            fade->SetDelay (delay);

            fade->OnUpdate (
                [=] (int /*id*/, const friz::Animation<1>::ValueList& val)
                {
                    // every update, change the saturation value of the color.
                    box->SetSaturation (val[0]);
                });

            fade->OnCompletion (
                [=] (int /*id*/)
                {
                    // ...and when the fade animation is complete, delete the box from the
                    // demo component.
                    // DBG("Completing # " << id);
                    this->DeleteBox (box);
                });

            fAnimator.AddAnimation (std::move (fade));
        });

    fAnimator.AddAnimation (std::move (movement));

    fBoxList.emplace_back (box);
}

void DemoComponent::DeleteBox (DemoBox* box)
{
    fBoxList.erase (std::remove_if (fBoxList.begin (), fBoxList.end (),
                                    [&] (const std::unique_ptr<DemoBox>& b)
                                    { return (b.get () == box); }));
}
