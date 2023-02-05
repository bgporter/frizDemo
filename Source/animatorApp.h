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
