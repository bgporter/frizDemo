/*
 * Copyright (c) 2019 Brett g Porter.
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_DEBUG
#define qRunUnitTests
#include "subTest.h"
#endif

namespace ID
{
const juce::Identifier kParameters { "params" };
const juce::Identifier kBreadcrumbs { "breadcrumbs" };
const juce::Identifier kDuration { "dur" };
const juce::Identifier kCurve { "curve" }; // int/enum

const juce::Identifier kEaseOutToleranceX { "eotx" };
const juce::Identifier kEaseOutToleranceY { "eoty" };
const juce::Identifier kEaseOutSlewX { "eoslewx" };
const juce::Identifier kEaseOutSlewY { "eoslewy" };

const juce::Identifier kEaseInToleranceX { "eitx" };
const juce::Identifier kEaseInToleranceY { "eity" };
const juce::Identifier kEaseInSlewX { "eislewx" };
const juce::Identifier kEaseInSlewY { "eislewy" };

const juce::Identifier kSpringToleranceX { "springToleranceX" };
const juce::Identifier kSpringDampingX { "springDampingX" };
const juce::Identifier kSpringToleranceY { "springToleranceY" };
const juce::Identifier kSpringDampingY { "springDampingY" };

const juce::Identifier kFadeDelay { "fadeDelay" };  // int
const juce::Identifier kFadeDuration { "fadeDur" }; // int
} // namespace ID
