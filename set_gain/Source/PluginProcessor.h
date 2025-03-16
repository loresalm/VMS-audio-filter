/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiGainController.h"

//==============================================================================
/**
*/
class Test_filterAudioProcessor  : public juce::AudioProcessor,
                                   public juce::MidiInputCallback,
                                   public juce::Timer
{
public:
    //==============================================================================
    Test_filterAudioProcessor();
    ~Test_filterAudioProcessor() override;

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
    void setToggleState(bool state) { shouldReduceVolume = state; }
    bool getToggleState() const { return shouldReduceVolume; }
    void setRawVolume(double volume) {rawVolume = volume; }
    juce::AudioProcessorValueTreeState& getParameters() { return parameters; }
    int getControllerNumber();
    int getcontrollerValue();
    bool getMidiActivity() const { return midiActivityDetected; }
    bool isMidiControllerConnected() const
        {
            return midiController->isDeviceConnected();
        }
    void startMidiInput();
    void stopMidiInput();
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    void timerCallback() override;

    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Test_filterAudioProcessor)
    //==============================================================================
    double rawVolume;
    bool shouldReduceVolume = false;
    bool midiActivityDetected = false;

    juce::AudioProcessorValueTreeState parameters;
    
    int controllerNumber;
    int controllerValue;
    
    // The MIDI controller
    std::unique_ptr<MidiGainController> midiController;
    std::unique_ptr<juce::MidiInput> midiInputDevice;

};
