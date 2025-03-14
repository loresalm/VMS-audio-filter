/*
  ==============================================================================

    MidiGainController.cpp
    Created: 14 Mar 2025 8:55:49am
    Author:  Lorenzo Salmina

  ==============================================================================
*/

// MidiGainController.cpp
#include "MidiGainController.h"

//==============================================================================
MidiGainController::MidiGainController(juce::AudioProcessorValueTreeState& apvts)
    : parameters(apvts)
{
    // Initialize the device manager with no audio inputs/outputs, but enable MIDI input
    deviceManager.initialise(0, 2, nullptr, true);
    
    // Start checking for controller connections every 2 seconds
    startTimer(2000);
    
    // Initial search for MIDI devices
    refreshMidiInputs();
    
}

MidiGainController::~MidiGainController()
{
    stopTimer();
    
    // Clean up any MIDI callbacks
    if (!lastConnectedDeviceID.isEmpty())
        deviceManager.removeMidiInputCallback(lastConnectedDeviceID, this);
}

//==============================================================================
bool MidiGainController::isDeviceConnected() const
{
    return deviceConnected;
}

void MidiGainController::processMidiBuffer(const juce::MidiBuffer& midiMessages)
{
    // Process any incoming MIDI messages from the plugin's audio processor
    for (const auto metadata : midiMessages)
    {
        handleIncomingMidiMessage(nullptr, metadata.getMessage());
    }
}

void MidiGainController::prepareToPlay()
{
    // Ensure MIDI is initialized properly
    refreshMidiInputs();
}

//==============================================================================
void MidiGainController::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    
    if (message.isController())  // Check if the message is a MIDI CC message
    {
        if (message.getControllerNumber() == kMidiCC)  // Assuming CC#7 (Volume) controls the gain
        {
            float midiValue = message.getControllerValue() / 127.0f; // Normalize 0-127 to 0.0-1.0
            
            // Update the "gain" parameter in the AudioProcessorValueTreeState
            parameters.getParameter("gain")->setValueNotifyingHost(midiValue);
            deviceConnected = true;
        }
        
    }
}
//==============================================================================
void MidiGainController::timerCallback()
{
    // Periodically check if our device is still connected
    refreshMidiInputs();
}

void MidiGainController::refreshMidiInputs()
{
    // Get list of all available MIDI input devices
    auto midiInputs = juce::MidiInput::getAvailableDevices();
    
    // Track if we find the device
    bool foundDevice = false;
    
    // Look for any MIDI device - in a real app, you might want to look for specific device names
    for (auto& device : midiInputs)
    {
        // You could filter for specific devices by name, for example:
        // if (device.name.containsIgnoreCase("Arduino") || device.name.containsIgnoreCase("Micro"))
        
        // Enable the first available device
        if (!deviceManager.isMidiInputDeviceEnabled(device.identifier))
        {
            // If we already had a different device, remove its callback
            if (!lastConnectedDeviceID.isEmpty() && lastConnectedDeviceID != device.identifier)
                deviceManager.removeMidiInputCallback(lastConnectedDeviceID, this);
            
            // Enable the new device and register for callbacks
            deviceManager.setMidiInputDeviceEnabled(device.identifier, true);
            deviceManager.addMidiInputCallback(device.identifier, this);
            
            // Remember this device
            lastConnectedDeviceID = device.identifier;
            foundDevice = true;
            deviceConnected = true;
            
            DBG("MIDI controller connected: " + device.name);
            break;
        }
        else if (device.identifier == lastConnectedDeviceID)
        {
            // Our device is still connected
            foundDevice = true;
            break;
        }
    }
    
    // Update connection status
    deviceConnected = foundDevice;
    
    // If no device found and we had one before, update state
    if (!foundDevice && !lastConnectedDeviceID.isEmpty())
    {
        DBG("MIDI controller disconnected");
        deviceManager.removeMidiInputCallback(lastConnectedDeviceID, this);
        lastConnectedDeviceID = "";
    }
}
