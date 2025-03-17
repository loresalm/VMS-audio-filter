/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GUI_testAudioProcessorEditor::GUI_testAudioProcessorEditor (GUI_testAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    // Set up connection status label
    addAndMakeVisible(connectionStatusLabel);
    connectionStatusLabel.setJustificationType(juce::Justification::centred);
    updateConnectionStatus();
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.5);
    addAndMakeVisible(gainSlider);
    gainSlider.setLookAndFeel(&customLookAndFeel);
}

GUI_testAudioProcessorEditor::~GUI_testAudioProcessorEditor()
{
}

//==============================================================================
void GUI_testAudioProcessorEditor::paint (juce::Graphics& g)
{
    const juce::String colourString ("ece5d1");
    const juce::Colour colour (juce::Colour::fromString ("FF" + colourString));
    g.fillAll (colour);
}

void GUI_testAudioProcessorEditor::resized()
{
    // Define the size and position of the gain knob
    int knobSize = 100; // Set the size of the gain knob
    int knobX = 250;     // X position of the gain knob
    int knobY = 50;     // Y position of the gain knob

    // Set the bounds of the gain knob
    gainSlider.setBounds(knobX, knobY, knobSize, knobSize);
    
    // Define the position, size, and font size for the connection status label
    int labelX = 5;           // X position of the label
    int labelY = 5; // Y position of the label (50 pixels from the bottom)
    int labelWidth = 200;       // Width of the label
    int labelHeight = 30;       // Height of the label
    float fontSize = 16.0f;     // Font size for the label

    // Set the bounds of the connection status label
    connectionStatusLabel.setBounds(labelX, labelY, labelWidth, labelHeight);

    // Set the font size for the connection status label using FontOptions
    juce::Font font(juce::FontOptions{fontSize});
    connectionStatusLabel.setFont(font);
}


//==============================================================================

void GUI_testAudioProcessorEditor::updateConnectionStatus()
{
    bool connected = false;

    connectionStatusLabel.setText(
        connected ? "MIDI Controller: Connected" : "MIDI Controller: Disconnected",
        juce::dontSendNotification);

    // Define colors using hex values
    const juce::Colour connectedColor (juce::Colour::fromString ("FF88b4b4"));
    const juce::Colour disconnectedColor (juce::Colour::fromString ("FF85827a"));

    connectionStatusLabel.setColour(
        juce::Label::textColourId,
        connected ? connectedColor : disconnectedColor);
}
