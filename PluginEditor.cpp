/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyFirstPluginAudioProcessorEditor::MyFirstPluginAudioProcessorEditor (MyFirstPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 400);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setNumDecimalPlacesToDisplay(2);
    gainSlider.setRange(-48.0, 12.0);
    //gainSlider.setValue(audioProcessor.dbVolume);
    gainSlider.setDoubleClickReturnValue(true, 0.0f);
    gainSlider.setSliderSnapsToMousePosition(false);
    gainSlider.setVelocityBasedMode(false);
    gainSlider.setVelocityModeParameters(0.25, 1.0, 0.0, true);
    gainSlider.setLookAndFeel(&customLook);
    // Manual listener not necessary anymore since we're using the slider attachement
    // gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);
    gainSliderAttachement = new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, "gain", gainSlider);
    addAndMakeVisible(sliderLabel);
    sliderLabel.setText("Le Gain", juce::dontSendNotification);
    sliderLabel.setColour(juce::Label::textColourId, juce::Colours::beige);
    sliderLabel.setJustificationType(juce::Justification::centred);
    sliderLabel.setFont(juce::Font("SignPainter", 24.0f, juce::Font::plain));
    addAndMakeVisible(polarityButton);
    polarityButton.setButtonText("Original");
    polarityButton.setClickingTogglesState(true);
    polarityButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::dimgrey);
    polarityButton.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::beige);
    polarityButton.setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::dimgrey);
    polarityButton.setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::beige);
    // Listener not necessary anymore
    polarityButton.addListener(this);
    polarityButtonAttachment = new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.parameters, "polarity", polarityButton);
    buttonLabel.setText("La Polarity", juce::dontSendNotification);
    buttonLabel.setColour(juce::Label::textColourId, juce::Colours::beige);
    buttonLabel.setJustificationType(juce::Justification::centred);
    buttonLabel.setFont(juce::Font("SignPainter", 24.0f, juce::Font::plain));
    addAndMakeVisible(buttonLabel);
}

MyFirstPluginAudioProcessorEditor::~MyFirstPluginAudioProcessorEditor()
{
}

//==============================================================================
void MyFirstPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(0xff323031));

//    g.setColour (juce::Colours::white);
//    g.setFont (juce::FontOptions (15.0f));
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void MyFirstPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    
    int sliderLabelHeight = 50;
    int buttonHeight = 50;
    int buttonWidth = 100;
    int buttonLabelHeight = 50;
    sliderLabel.setBounds(area.removeFromTop(sliderLabelHeight));
    //polarityButton.setBounds(area.removeFromBottom(buttonHeight));
    polarityButton.setBounds(area.getWidth()/2 - buttonWidth/2, area.getHeight() - 5, buttonWidth, buttonHeight);
    area.removeFromBottom(buttonHeight + 5);
    buttonLabel.setBounds(area.removeFromBottom(buttonLabelHeight));
    gainSlider.setBounds(area);
}

/* not necessary anymore since we use a slider attachement to the parameters tree state
void MyFirstPluginAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    
    // if my slider has an updated gain
    if (slider == &gainSlider) {
        audioProcessor.dbVolume = gainSlider.getValue();
    }
}
 */
    

void MyFirstPluginAudioProcessorEditor::buttonClicked(juce::Button* button){
    if (button == &polarityButton) {
        // audioProcessor.polarityInversion = polarityButton.getToggleState();
        if (polarityButton.getToggleState()) {
            polarityButton.juce::Button::setButtonText("Reversed");
        }
        else {
            polarityButton.juce::Button::setButtonText("Original");
        }
    }
}

 
