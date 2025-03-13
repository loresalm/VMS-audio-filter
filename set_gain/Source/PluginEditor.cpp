/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Test_filterAudioProcessorEditor::Test_filterAudioProcessorEditor (Test_filterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    
    // Set up the MIDI CC Label
    midiCCLabel.setText("MIDI CC: 0", juce::dontSendNotification);  // Default text
    midiCCLabel.setFont(juce::Font(16.0f));  // Set font size
    midiCCLabel.setColour(juce::Label::textColourId, juce::Colours::white);  // Set text color
    midiCCLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(midiCCLabel);  // Add the label to the editor

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.5);
    gainSlider.addListener(this);
    
    // Set the button text
    //toggleButton.setButtonText("Reduce Volume");
    // Add button to editor
    //addAndMakeVisible(toggleButton);
    // listen for button click
    //toggleButton.onClick = [this]() { toggleButtonChanged(); };
    
    // Add slider to editor
    addAndMakeVisible(gainSlider);

    // listen for change in the slider
    audioProcessor.getParameters().addParameterListener("gain", this);
    
    //set window size
    setSize (300, 150);
}

Test_filterAudioProcessorEditor::~Test_filterAudioProcessorEditor()
{
    audioProcessor.getParameters().removeParameterListener("gain", this);
}

//==============================================================================
void Test_filterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    const juce::String colourString ("0a0a0a");
    const juce::Colour colour (juce::Colour::fromString ("FF" + colourString));
    g.fillAll (colour);
}

void Test_filterAudioProcessorEditor::resized()
{
    midiCCLabel.setBounds(10, 10, 200, 40);
    // Set button bounds (centered)
    // toggleButton.setBounds (200, 10, 100, 30);
    // Set slider bounds
    gainSlider.setBounds(getLocalBounds());
    // set midi msg bounds
    midiMessageLabel.setBounds(10, getHeight() - 40, getWidth() - 20, 30);
}

void Test_filterAudioProcessorEditor::toggleButtonChanged()
{
    // Update the processor with the button state
    audioProcessor.setToggleState(toggleButton.getToggleState());
}

void  Test_filterAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.getParameters().getParameter("gain")->setValueNotifyingHost(gainSlider.getValue());
    }
}

void Test_filterAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "gain")
    {
        gainSlider.setValue(newValue, juce::dontSendNotification);
    }
    int controllerNumber = audioProcessor.getControllerNumber();
    int controllerValue = audioProcessor.getcontrollerValue();
    this->updateMIDIText(controllerNumber, controllerValue);
    
}

void Test_filterAudioProcessorEditor::updateMIDIText(int controllerNumber, int controllerValue)
{
    // Update the label text with the received MIDI CC value
    midiCCLabel.setText("MIDI CC: " + juce::String(controllerNumber) + " Value: " + juce::String(controllerValue), juce::dontSendNotification);
}
