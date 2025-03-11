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
    // Set the button text
    toggleButton.setButtonText("Reduce Volume");
    
    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setRange(0.0, 1.0);
    gainSlider.setValue(0.5);
    gainSlider.addListener(this);
    
    
    // Add button to editor
    addAndMakeVisible(toggleButton);
    addAndMakeVisible(gainSlider);

    // Add listener for button click
    toggleButton.onClick = [this]() { toggleButtonChanged(); };
    
    //set window size
    setSize (300, 150);
}

Test_filterAudioProcessorEditor::~Test_filterAudioProcessorEditor()
{
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
    // Set button bounds (centered)
    toggleButton.setBounds (200, 10, 100, 30);
    // Set slider bounds
    gainSlider.setBounds(getLocalBounds());
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
        audioProcessor.setRawVolume(gainSlider.getValue());
    }
}

