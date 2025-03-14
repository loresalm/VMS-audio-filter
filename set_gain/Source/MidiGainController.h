/*
  ==============================================================================

    MidiGainController.h
    Created: 14 Mar 2025 8:55:49am
    Author:  Lorenzo Salmina

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MidiGainController : public juce::MidiInputCallback,
                           private juce::Timer
{
public:
    //==============================================================================
    // Constructor now takes a reference to AudioProcessorValueTreeState
    MidiGainController(juce::AudioProcessorValueTreeState& apvts);
    ~MidiGainController() override;

    //==============================================================================
    // Checks if the MIDI controller is connected
    bool isDeviceConnected() const;
    
    // Call this from your processor's processBlock
    void processMidiBuffer(const juce::MidiBuffer& midiMessages);
    
    // Call this in the plugin's prepareToPlay method
    void prepareToPlay();
    
    //==============================================================================
    // MidiInputCallback implementation
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

private:
    //==============================================================================
    // Timer callback to check for controller connection
    void timerCallback() override;
    
    // Refreshes the list of MIDI inputs and finds the controller
    void refreshMidiInputs();
    
    //==============================================================================
    juce::AudioDeviceManager deviceManager;
    juce::String lastConnectedDeviceID;
    
    // Reference to the AudioProcessorValueTreeState that contains the gain parameter
    juce::AudioProcessorValueTreeState& parameters;
    
    // MIDI parameters
    static constexpr int kMidiCC = 7;  // CC #7 is standard for volume control
    bool deviceConnected = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGainController)
};
