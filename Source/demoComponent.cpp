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
        setSize (size, size);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (fFill);
        const auto bounds = getLocalBounds ();
        g.setColour (juce::Colours::black);
        g.drawRect (bounds, 4);
    }

    float getSaturation () const { return fFill.getSaturation (); }

    void setSaturation (float newSaturation)
    {
        fFill = fFill.withSaturation (newSaturation);
        repaint ();
    }

public:
    juce::Colour fFill;
};

//==============================================================================
DemoComponent::DemoComponent (juce::ValueTree params)
: fParams (params)
{
    addAndMakeVisible (fBreadcrumbs);
    fBreadcrumbs.toBack ();
}

DemoComponent::~DemoComponent ()
{
    clear ();
}

void DemoComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::lightgrey);
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds (), 1); // draw an outline around the component
}

void DemoComponent::resized ()
{
    fBreadcrumbs.setBounds (getLocalBounds ());
}

void DemoComponent::clear ()
{
    fAnimator.cancelAllAnimations (false);
    fBoxList.clear ();
    fBreadcrumbs.clear ();
    repaint ();
}

void DemoComponent::mouseDown (const juce::MouseEvent& e)
{
    if (e.mods.isPopupMenu ())
    {
        clear ();
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

        createDemo (e.getPosition (), type);
    }
}

void DemoComponent::createDemo (juce::Point<int> startPoint, EffectType type)
{
    auto& r { juce::Random::getSystemRandom () };

    bool enableCrumbs = fParams.getProperty (ID::kBreadcrumbs);
    if (enableCrumbs != fBreadcrumbs.isEnabled ())
    {
        fBreadcrumbs.enable (enableCrumbs);
    }

    fBreadcrumbs.clear ();
    repaint ();

    auto box = new DemoBox ();
    addAndMakeVisible (box);
    box->setBounds (startPoint.x, startPoint.y, box->getWidth (), box->getHeight ());

    // set the animation parameters.
    auto startX = static_cast<float> (startPoint.x);
    auto endX   = static_cast<float> (r.nextInt ({ 0, getWidth () - box->getWidth () }));
    auto startY = static_cast<float> (startPoint.y);
    auto endY = static_cast<float> (r.nextInt ({ 0, getHeight () - box->getHeight () }));

    auto movement = std::make_unique<friz::Animation<2>> (++fNextEffectId);

    std::unique_ptr<friz::AnimatedValue> xCurve;
    std::unique_ptr<friz::AnimatedValue> yCurve;

    int duration = fParams.getProperty (ID::kDuration, 500);

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
        auto xAccel = std::abs (endX - startX) / 1000.f;
        auto yAccel = std::abs (endY - startY) / 1000.f;

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
        sequence->addAnimation (std::move (effect1));
        sequence->addAnimation (std::move (effect2));

        movement = std::move (sequence);
    }

    const int kXpos { 0 };
    const int kYpos { 1 };

    if (EffectType::kInOut != type)
    {
        movement->setValue (kXpos, std::move (xCurve));
        movement->setValue (kYpos, std::move (yCurve));
    }

    // On each update: move this box to the next position on the (x,y) curve.
    movement->onUpdate (
        [=] (int /*id*/, const friz::Animation<2>::ValueList& val)
        {
            const auto x { static_cast<int> (val[kXpos]) };
            const auto y { static_cast<int> (val[kYpos]) };
            box->setTopLeftPosition (x, y);
            fBreadcrumbs.addPoint (val[kXpos], val[kYpos]);
        });

    // When the main animation completes: start a second animation that slowly
    // fades the color all the way out.
    movement->onCompletion (
        [=] (int /*id*/)
        {
            float currentSat = box->getSaturation ();

            int delay = fParams.getProperty (ID::kFadeDelay);
            int dur   = fParams.getProperty (ID::kFadeDuration);

            auto fade = std::make_unique<friz::Animation<1>> (
                friz::Animation<1>::SourceList {
                    std::make_unique<friz::Linear> (currentSat, 0.f, dur) },
                ++fNextEffectId);

            // don't start fading until `delay` frames have elapsed
            fade->setDelay (delay);

            fade->onUpdate (
                [=] (int /*id*/, const friz::Animation<1>::ValueList& val)
                {
                    // every update, change the saturation value of the color.
                    box->setSaturation (val[0]);
                });

            fade->onCompletion (
                [=] (int /*id*/)
                {
                    // ...and when the fade animation is complete, delete the box from the
                    // demo component.
                    this->deleteBox (box);
                });

            fAnimator.addAnimation (std::move (fade));
        });

    fAnimator.addAnimation (std::move (movement));

    fBoxList.emplace_back (box);
}

void DemoComponent::deleteBox (DemoBox* box)
{
    fBoxList.erase (std::remove_if (fBoxList.begin (), fBoxList.end (),
                                    [&] (const std::unique_ptr<DemoBox>& b)
                                    { return (b.get () == box); }));
}
