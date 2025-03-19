/*
  ==============================================================================

    MidiController.cpp
    Created: 14 Mar 2025 8:55:49am
    Author:  Lorenzo Salmina

  ==============================================================================
*/

// MidiController.cpp
#include "midiController.h"

//==============================================================================
MidiController::MidiController(juce::AudioProcessorValueTreeState& apvts)
    : parameters(apvts)
{
    // Initialize the device manager with no audio inputs/outputs, but enable MIDI input
    deviceManager.initialise(0, 2, nullptr, true);
    
    // Start checking for controller connections every 2 seconds
    startTimer(2000);
    
    // Initial search for MIDI devices
    refreshMidiInputs();
    
}

MidiController::~MidiController()
{
    stopTimer();
        
    // Clean up any MIDI callbacks
    if (!lastConnectedDeviceID.isEmpty()) {
        deviceManager.removeMidiInputCallback(lastConnectedDeviceID, this);
    }
    // Initialize MIDI output
    auto availableDevices = juce::MidiOutput::getAvailableDevices();
    if (!availableDevices.isEmpty())
    {
        midiOutputDevice = juce::MidiOutput::openDevice(availableDevices[0].identifier);
    }
}

//==============================================================================
bool MidiController::isDeviceConnected() const
{
    return deviceConnected;
}

void MidiController::processMidiBuffer(const juce::MidiBuffer& midiMessages)
{
    // Process any incoming MIDI messages from the plugin's audio processor
    for (const auto metadata : midiMessages)
    {
        handleIncomingMidiMessage(nullptr, metadata.getMessage());
    }
}

void MidiController::prepareToPlay()
{
    // Ensure MIDI is initialized properly
    refreshMidiInputs();
}

//==============================================================================
void MidiController::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    
    if (message.isController())  // Check if the message is a MIDI CC message
    {
        if (message.getControllerNumber() == kMidiCC)  // Assuming CC#7 (Volume) controls the gain
        {
            float midiValue = message.getControllerValue() / 127.0f; // Normalize 0-127 to 0.0-1.0
            
            // Update the "gain" parameter in the AudioProcessorValueTreeState
            parameters.getParameter("gain")->setValueNotifyingHost(midiValue);
            //deviceConnected = true;
        }
        
    }
}

void MidiController::sendMidiMessage(const juce::MidiMessage& message)
{
    if (midiOutputDevice)
    {
        midiOutputDevice->sendMessageNow(message);
    }
}


//==============================================================================
void MidiController::timerCallback()
{
    // Periodically check if our device is still connected
    refreshMidiInputs();
    // Initialize MIDI output
    auto availableDevices = juce::MidiOutput::getAvailableDevices();
    if (!availableDevices.isEmpty())
    {
        midiOutputDevice = juce::MidiOutput::openDevice(availableDevices[0].identifier);
    }
}

void MidiController::refreshMidiInputs()
{
    // Get list of all available MIDI input devices
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    
    // If no devices are available at all
    if (midiInputs.isEmpty())
    {
        deviceConnected = false;

    }else{deviceConnected = true;}
    
}
