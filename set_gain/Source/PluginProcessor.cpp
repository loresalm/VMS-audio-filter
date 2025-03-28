/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiGainController.h"

//==============================================================================
Test_filterAudioProcessor::Test_filterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters (*this, nullptr, "PARAMETERS",
       {
           std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 0.5f)
       })
#endif
{
    midiController = std::make_unique<MidiGainController>(parameters);
    startMidiInput();  // Start listening to MIDI input
    startTimer(2000);
}

Test_filterAudioProcessor::~Test_filterAudioProcessor()
{
    stopMidiInput(); // Stop listening to MIDI input when destroyed
}

//==============================================================================
const juce::String Test_filterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Test_filterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Test_filterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Test_filterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Test_filterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Test_filterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Test_filterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Test_filterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Test_filterAudioProcessor::getProgramName (int index)
{
    return {};
}

void Test_filterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Test_filterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    // Initialize MIDI controller
    midiController->prepareToPlay();
}

void Test_filterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Test_filterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Test_filterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //==============================================================================
    
    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage message;
    int samplePosition;

    while (it.getNextEvent(message, samplePosition))
    {
        if (midiController)
        {
            midiController->handleIncomingMidiMessage(nullptr, message);
        }
    }
    
    // Get current gain value from ValueTreeState
    float gainValue = *parameters.getRawParameterValue("gain");
    
    // Apply gain to audio
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] *= gainValue;
            }
        }
}

//==============================================================================
bool Test_filterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Test_filterAudioProcessor::createEditor()
{
    return new Test_filterAudioProcessorEditor (*this);
}

//==============================================================================
void Test_filterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Test_filterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Test_filterAudioProcessor();
}

int Test_filterAudioProcessor::getControllerNumber() { return controllerNumber; }
int Test_filterAudioProcessor::getcontrollerValue() { return controllerValue; }

void Test_filterAudioProcessor::startMidiInput()
{
    auto availableDevices = juce::MidiInput::getAvailableDevices();
    if (!availableDevices.isEmpty())
    {
        auto deviceID = availableDevices[0].identifier;
        midiInputDevice = juce::MidiInput::openDevice(deviceID, this); // Now correctly assigned

        if (midiInputDevice)
            midiInputDevice->start();
    }
}

void Test_filterAudioProcessor::stopMidiInput()
{
    if (midiInputDevice)
    {
        midiInputDevice->stop();
        midiInputDevice.reset(); // Properly cleanup using unique_ptr
    }
}

void Test_filterAudioProcessor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    if (midiController)
    {
        midiController->handleIncomingMidiMessage(nullptr, message);
    }
}

void Test_filterAudioProcessor::timerCallback()
{
    auto availableDevices = juce::MidiInput::getAvailableDevices();

    if (!availableDevices.isEmpty() && !midiInputDevice) // If a device is available but not started
    {
        startMidiInput();
    }
    else if (availableDevices.isEmpty() && midiInputDevice) // If no devices are available but we're still running
    {
        stopMidiInput();
    }
}
