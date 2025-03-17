/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/


class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Set custom colors
        knobColour = juce::Colour::fromString ("FFe35807");// Dodger Blue for the knob
        dialColour = juce::Colour::fromString ("FF1E90FF"); // Orange for the dial
        // Set the hex color for the text box text (e.g., #FFA500 for orange)
        textColour = juce::Colour::fromString ("FF2a362f");
    }
    /*
    void drawLabel(juce::Graphics& g, juce::Label& label) override
    {
        // Override the label drawing to use the custom text color
        g.setColour(textColour);
        g.setFont(label.getFont());
        g.drawText(label.getText(), label.getLocalBounds(), juce::Justification::centred, false);
    }
     */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                              const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
    {
        float diameter = juce::jmin(width, height);
        float r = diameter/2.;
        float cX = x + width/2.;
        float cY = y + height/2.;
        
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        float circleRadius = r * 0.1f; // You can adjust this size as needed
        float circleDistance = r * 0.9f; // Distance from center to the circle
        

        float rX = cX - r;
        float rY = cY - r;
        /*
        juce::Rectangle<float> dialArea (rX, rY, diameter, diameter);
        g.setColour(dialColour);
        g.drawRect(dialArea);
         */
        float diameter_int = diameter - diameter/2.5;
        float r_int = diameter_int/2;
        float rX_int = cX - r_int;
        float rY_int = cY - r_int;
        juce::Rectangle<float> dialBounds (x, y, width, height);
        juce::Rectangle<float> dialArea_ext (rX, rY, diameter, diameter);
        juce::Rectangle<float> dialArea_int (rX_int, rY_int, diameter_int, diameter_int);
        //juce::Rectangle<int> intdialcenter = dialcenter.toNearestInt();
        //g.setColour(textColour);
        //g.drawRect(dialBounds,  2.0f);
        g.setColour(textColour);
        g.drawEllipse(dialArea_ext,  2.0f);
        g.drawEllipse(dialArea_int,  2.0f);
        //g.fillEllipse(dialArea);
        
        
        // DOTS
        // Draw dots between rotaryStartAngle and rotaryEndAngle
        int numDots = 10; // Number of dots to draw

        float dotStep = (rotaryEndAngle - rotaryStartAngle)/numDots;
        float dotAngle = rotaryStartAngle;
        juce::Path dialTicks;
        // Add a circle at the appropriate position
        dialTicks.addEllipse(-circleRadius, -circleDistance,
                             circleRadius * 2.0f, circleRadius * 2.0f);
        for (int i = 0; i <= numDots; ++i)
        {
            g.setColour(knobColour); // Use knobColour for the dots
            g.fillPath(dialTicks, juce::AffineTransform::rotation(dotAngle).translated(cX,cY));
            
            dotAngle = dotAngle + dotStep;
            
        }
        
        //TICK
        juce::Path dialTick;
        // Add a circle at the appropriate position
        dialTick.addEllipse(-circleRadius, -circleDistance,
                            circleRadius * 2.0f, circleRadius * 2.0f);
        g.setColour(textColour);
        g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(cX,cY));
        g.fillPath(dialTick, juce::AffineTransform::rotation(rotaryStartAngle).translated(cX,cY));
        
        
        
        // ARC
        juce::Path arcPath;
        float arcThickness = 2.0f; // Thickness of the arc
        float arcRadius = r * 0.8f; // Radius of the arc (adjust as needed)
        // Add the arc to the path
        arcPath.addArc(cX - arcRadius, cY - arcRadius, arcRadius * 2.0f, arcRadius * 2.0f,
                       rotaryStartAngle, angle, true);
        // Set the color and stroke the arc
        g.setColour(textColour); // Use knobColour for the arc
        g.strokePath(arcPath, juce::PathStrokeType(arcThickness));
        
    }
    

private:
    juce::Colour textColour; // Custom text color
    juce::Colour knobColour; // Color of the knob
    juce::Colour dialColour; // Color of the dial
};








class GUI_testAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    GUI_testAudioProcessorEditor (GUI_testAudioProcessor&);
    ~GUI_testAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GUI_testAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUI_testAudioProcessorEditor)
    //==============================================================================
    
    void updateConnectionStatus();
    juce::Label connectionStatusLabel;
    juce::Label gainLabel;
    juce::Slider gainSlider;
    CustomLookAndFeel customLookAndFeel;
};
