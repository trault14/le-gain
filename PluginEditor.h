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

class CustomSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        // Example: fill background
        g.setColour(juce::Colours::dimgrey);
        g.fillRect(width/2 - (width/6)/2, y, width/6, height);

        // Example: draw thumb
        g.setColour(juce::Colours::beige);
        g.fillRect((width/2) - (width/3)/2, (int)sliderPos - 5, width / 3, 15);
    }
};

class MyFirstPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener // public juce::Slider::Listener
{
public:
    MyFirstPluginAudioProcessorEditor (MyFirstPluginAudioProcessor&);
    ~MyFirstPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    // Not necessary anymore since we're using Attachements
    // void sliderValueChanged(juce::Slider *slider) override;
    void buttonClicked(juce::Button* button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MyFirstPluginAudioProcessor& audioProcessor;
    juce::Slider gainSlider;
    juce::Label sliderLabel;
    juce::Label buttonLabel;
    juce::TextButton polarityButton;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::ButtonAttachment> polarityButtonAttachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachement;
    CustomSliderLookAndFeel customLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyFirstPluginAudioProcessorEditor)
};
