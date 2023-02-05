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

/**
 * @class SubTest
 * @brief An extension to the JUCE UnitTest system.
 */

class SubTest : public juce::UnitTest
{
public:
    SubTest (const juce::String& name, const juce::String& category = juce::String ())
    : UnitTest (name, category)
    , fIsSetup (false)
    {
    }

    /**
     * Perform any common setup actions needed by your sub-tests.
     *
     * Called automatically by the `Test()` method; you shouldn't need to
     * call this explicitly.
     *
     * If your test class needs resources that are allocated once for
     * all of your subtests, you can handle that by overriding the
     * `UnitTest::initialise()` method.
     *
     * Default does nothing.
     */
    virtual void Setup () {}

    /**
     * Perform any common cleanup needed by your subtests.
     *
     * If your test class allocated resources in the `initialise()` method that
     * stayed in scope for all of your subtests, you should handle that cleanup
     * by overriding the `UnitTest::shutdown()` method.
     *
     * If the class you're testing has a method that lets you check a class
     * invariant, adding a call inside the `TearDown()` method like:
     * ```
     *    // call your class invariant checker
     *    expect(this->IsValid());
     * ```
     *
     * Lets you check that each test not only succeeded on its own terms, but
     * left the object being tested in a valid state.
     */
    virtual void TearDown () {}

    /**
     * The Test method:
     * * performs common setup.
     * * executes a single subtest
     * * performs the common cleanup.
     *
     * Your unit tests will be a sequence of calls to `Test(...)` inside of your
     * test suite class' `runTest()` method.
     *
     * @param testName Name of the sub-test.
     * @param testFn   Test function to call.
     * @sa SkipTest
     */
    void Test (juce::StringRef testName, std::function<void (void)> testFn)
    {
        beginTest (testName);
        jassert (!fIsSetup);
        Setup ();
        fIsSetup = true;

        testFn ();

        TearDown ();
        fIsSetup = false;
    }

    /**
     * An easy way to disable a test that's implemented by calling an std::function
     * that doesn't just comment (or conditionally compile) out that test. Your
     * test log will include a line helping you remember that the test wasn't run.
     *
     * Assumption is that you may encounter working tests that momentarily break
     * so we want to temporarily disable them. Instead of commenting or `#ifdef`-ing
     * those tests out, change the function call from `Test(...)` to
     * `SkipTest(...)`. To re-enable the test, just change that method call back
     * to `Test(...)`.
     *
     * @param testName name/description of this test.
     * @param void     test function to (not) call.
     *
     * @sa Test
     */
    void SkipTest (juce::StringRef testName, std::function<void (void)> /*testFn*/)
    {
        logMessage ("-----------------------------------------------------------------");
        logMessage (TRANS ("WARNING: Skipping ") + this->getName () + " / " + testName);
    }

private:
    bool fIsSetup;
};
