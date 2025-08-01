/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MyFirstPluginAudioProcessor::MyFirstPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, &undoManager, "PARAMETERS", createParameterLayout())
#endif
{
}

MyFirstPluginAudioProcessor::~MyFirstPluginAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout MyFirstPluginAudioProcessor::createParameterLayout() {
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto pGain = std::make_unique<juce::AudioParameterFloat>("gain", "Le Gain", -48.0f, 12.0f, 0.0f);
    params.push_back(std::move(pGain));
    
    auto pPolarity = std::make_unique<juce::AudioParameterBool>("polarity", "La Polarity", false);
    params.push_back(std::move(pPolarity));
    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String MyFirstPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MyFirstPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MyFirstPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MyFirstPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MyFirstPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MyFirstPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MyFirstPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MyFirstPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MyFirstPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void MyFirstPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MyFirstPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MyFirstPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MyFirstPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MyFirstPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    /*
    // Simple method with no gain riding
    rawVolume = pow (10, dbVolume / 20);
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            channelData[sample] *= rawVolume;
        }
    }
     */
    /*
    // Manual gain ramp
    rawVolume = pow (10, dbVolume / 20);
    float targetGain = rawVolume;
    float gainIncrement = (targetGain - previousVolume) / buffer.getNumSamples();
    float currentGain = previousVolume;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            channelData[sample] *= currentGain;
            currentGain += gainIncrement;
        }
    }
    previousVolume = targetGain;
     */
    
    /* old version of the gain processing
    // JUCE gain ramp
    rawVolume = pow(10, dbVolume / 20);
    float targetGain = rawVolume;
    buffer.applyGainRamp(0, buffer.getNumSamples(), previousVolume, targetGain);
    previousVolume = targetGain;
    
    // old school without the parameter tree state
    // Phase inversion
    if (polarityInversion){
        buffer.applyGain(-1.0f);
    }
     */
    
    /*
    // Manual phase inversion
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            channelData[sample] *= -1;
        }
    }
     */
    
    float dBGain = *parameters.getRawParameterValue("gain");
    rawVolume = juce::Decibels::decibelsToGain(dBGain);
    float targetGain = rawVolume;
    buffer.applyGainRamp(0, buffer.getNumSamples(), previousVolume, targetGain);
    previousVolume = targetGain;
     
    if (*parameters.getRawParameterValue("polarity")) {
        buffer.applyGain(-1.0f);
    }
    
}

//==============================================================================
bool MyFirstPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MyFirstPluginAudioProcessor::createEditor()
{
    return new MyFirstPluginAudioProcessorEditor (*this);
}

//==============================================================================
void MyFirstPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MyFirstPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

        if (xmlState != nullptr)
        {
            if (xmlState->hasTagName(parameters.state.getType()))
            {
                parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
            }
        }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MyFirstPluginAudioProcessor();
}
