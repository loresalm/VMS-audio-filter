/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Test_filterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Test_filterAudioProcessorEditor (Test_filterAudioProcessor&);
    ~Test_filterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Test_filterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Test_filterAudioProcessorEditor)
    
    //==============================================================================
    
    // Declare a toggle button
    juce::ToggleButton toggleButton;
    bool shouldReduceVolume = false;

    // Function to handle toggle button changes
    void toggleButtonChanged();

};
