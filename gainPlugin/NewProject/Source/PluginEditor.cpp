#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize(400, 400);

    // Connect slider to parameter using the getter method
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getParameters(), "gain", gainSlider);

    // Set up the MIDI CC Label
    midiCCLabel.setText("MIDI CC: 0", juce::dontSendNotification);  // Default text
    midiCCLabel.setFont(juce::Font(16.0f));  // Set font size
    midiCCLabel.setColour(juce::Label::textColourId, juce::Colours::white);  // Set text color
    midiCCLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(midiCCLabel);  // Add the label to the editor

    // Set up connection status label
    addAndMakeVisible(connectionStatusLabel);
    connectionStatusLabel.setJustificationType(juce::Justification::centred);
    updateConnectionStatus(); // Initial status

    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.5);
    addAndMakeVisible(gainSlider);
    //gainSlider.setLookAndFeel(&customLookAndFeel);

    // Start timer to check connection status periodically
    startTimer(1000); // Check every second
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
    gainAttachment.reset();
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    const juce::String colourString ("ece5d1");
    const juce::Colour colour (juce::Colour::fromString ("FF" + colourString));
    g.fillAll (colour);
}

void NewProjectAudioProcessorEditor::resized()
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

void NewProjectAudioProcessorEditor::updateConnectionStatus()
{
    bool connected = audioProcessor.isMidiControllerConnected();

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

void NewProjectAudioProcessorEditor::sliderValueChanged(juce::Slider *slider)
{
    if (slider == &gainSlider)
    {
        audioProcessor.getParameters().getParameter("gain")->setValueNotifyingHost(gainSlider.getValue());
    }
}

void NewProjectAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "gain")
    {
        gainSlider.setValue(newValue, juce::dontSendNotification);
    }

    // Ensure the audioProcessor is still valid
    if (&audioProcessor != nullptr)
    {
        int controllerNumber = audioProcessor.getControllerNumber();
        int controllerValue = audioProcessor.getcontrollerValue();
        this->updateMIDIText(controllerNumber, controllerValue);
    }
}

void NewProjectAudioProcessorEditor::updateMIDIText(int controllerNumber, int controllerValue)
{
    // Update the label text with the received MIDI CC value
    midiCCLabel.setText("MIDI CC: " + juce::String(controllerNumber) + " Value: " + juce::String(controllerValue), juce::dontSendNotification);
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    updateConnectionStatus();
}
