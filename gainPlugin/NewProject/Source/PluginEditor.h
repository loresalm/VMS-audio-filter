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

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Set custom colors
        knobColour = juce::Colour::fromString ("FF1E90FF");// Dodger Blue for the knob
        dialColour = juce::Colour::fromString ("FF1E90FF"); // Orange for the dial
        // Set the hex color for the text box text (e.g., #FFA500 for orange)
        textColour = juce::Colour::fromString ("FF2a362f");
    }
    /*
    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        // Override the label drawing to use the custom text color
        g.setColour(textColour);
        g.setFont(label.getFont());
        g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::centred, false);
    }
     */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                              const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        float diameter = juce::jmin(width, height);
        float r = diameter/2.;
        float cX = x - width/2.;
        float cY = y - height/2.;
        
        
        /*
        float rX = cX + r;
        float rY = cY + r;
        juce::Rectangle<float> dialArea (rX, rY, diameter, diameter);
        g.setColour(dialColour);
        g.drawRect(dialArea);
         */
        juce::Rectangle<float> dialcenter (cX, cY, width, height);
        g.setColour(textColour);
        g.drawRect(dialcenter);
    }
    

private:
    juce::Colour textColour; // Custom text color
    juce::Colour knobColour; // Color of the knob
    juce::Colour dialColour; // Color of the dial
};


class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener,
                                        public juce::AudioProcessorValueTreeState::Listener,
                                        private juce::Timer
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider *slider) override;
    // Listener function for parameter changes
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void updateMIDIText(int controllerNumber, int controllerValue);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
    
    //==============================================================================
    
    // Timer callback to update MIDI connection status
    void timerCallback() override;
    // Update the connection status label
    void updateConnectionStatus();
    // Parameter attachment for the gain slider
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    juce::Label connectionStatusLabel;

    juce::Label gainLabel;
    juce::Slider gainSlider;
    
    juce::Label midiCCLabel;
    
    CustomLookAndFeel customLookAndFeel;
};



