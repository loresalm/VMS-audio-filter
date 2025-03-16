/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "midiController.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor,
                                  public juce::MidiInputCallback,
                                  public juce::Timer
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }
    bool isMidiControllerConnected() const
        {
            midiController->refreshMidiInputs();
            return midiController->isDeviceConnected();
        }
    void startMidiInput();
    void stopMidiInput();
    int getControllerNumber() { return controllerNumber; }
    int getcontrollerValue() { return controllerValue; }
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void timerCallback() override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
    //==============================================================================
    
    // The MIDI controller
    std::unique_ptr<MidiController> midiController;
    std::unique_ptr<juce::MidiInput> midiInputDevice;
    int controllerNumber;
    int controllerValue;
    // plugin parameters
    juce::AudioProcessorValueTreeState parameters;
    
    
};
