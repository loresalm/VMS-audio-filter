/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"



// In your PluginEditor.h, add:
class MidiActivityIndicator : public juce::Component
{
public:
    MidiActivityIndicator()
    {
        midiActive = false;
    }

    void paint(juce::Graphics& g) override
    {
        // Draw the square
        g.setColour(midiActive ? juce::Colours::dodgerblue : juce::Colours::darkgrey);
        g.fillRect(getLocalBounds());
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds(), 1);
    }

    void setMidiActivity(bool active)
    {
        if (midiActive != active)
        {
            midiActive = active;
            repaint();
        }
    }

private:
    bool midiActive;
};




//==============================================================================
/**
*/
class Test_filterAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                         public juce::Slider::Listener,
                                         public juce::AudioProcessorValueTreeState::Listener
{
public:
    Test_filterAudioProcessorEditor (Test_filterAudioProcessor&);
    ~Test_filterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *slider) override;
    // Listener function for parameter changes
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    void updateMIDIText(int controllerNumber, int controllerValue);
    void updateMidiActivity(bool active) { midiIndicator.setMidiActivity(active); }


    // New label to display MIDI CC info
    juce::Label midiCCLabel;
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Test_filterAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Test_filterAudioProcessorEditor)
    
    //==============================================================================
    
    // Declare a toggle button
    juce::ToggleButton toggleButton;
    juce::Slider gainSlider;
    juce::Label midiMessageLabel;
    
    bool shouldReduceVolume = false;
    MidiActivityIndicator midiIndicator;

    // Function to handle toggle button changes
    void toggleButtonChanged();

};
