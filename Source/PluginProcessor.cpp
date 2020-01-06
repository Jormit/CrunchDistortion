/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrunchDistortionAudioProcessor::CrunchDistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), highShelf(), lowShelf()
#endif
{
}

CrunchDistortionAudioProcessor::~CrunchDistortionAudioProcessor()
{
}

//==============================================================================
const String CrunchDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CrunchDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CrunchDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CrunchDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CrunchDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CrunchDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CrunchDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CrunchDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String CrunchDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void CrunchDistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CrunchDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    
    // Using a lowshelf to boost lower frequencies to make sound warmer.
    // Need a filter for each channel to eliminate crackling.
    lowShelf[0].setCoefficients(IIRCoefficients::makeLowShelf (sampleRate, 500, 1.0f, 2.0f));
    lowShelf[1].setCoefficients(IIRCoefficients::makeLowShelf (sampleRate, 500, 1.0f, 2.0f));
}

void CrunchDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CrunchDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void CrunchDistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    
        
    // Main processing.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // Calculate the highshelf coefficients.
        highShelf[channel].setCoefficients(IIRCoefficients::makeHighShelf (sampleRate, 300, 1.0f/std::sqrt(2), mPresence/5.0f));
        
        float* channelData = buffer.getWritePointer (channel);
        
        // Pre Filtering.
        lowShelf[channel].processSamples(channelData, buffer.getNumSamples());     
        
        // Apply gain.
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) 
        {
            channelData[sample] = channelData[sample] * Decibels::decibelsToGain <float> (mGain);
        }
        
        // Soft Clipping Algorithm.
        if (mCurrentGainStyle == GainStyle::soft)
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++) 
            {
                channelData[sample] = std::atan (channelData[sample]);
            }
            
        } 
        
        // Hard Clipping Algorithm.
        else if (mCurrentGainStyle == GainStyle::hard)
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                
                if (channelData[sample] > 1.0f)
                {
                    channelData[sample] = 1.0f;
                }
                
                else if (channelData[sample] < -1.0f)
                {
                    channelData[sample] = -1.0f;
                }
            }            
        }
        
        // Post filtering.
        highShelf[channel].processSamples(channelData, buffer.getNumSamples());
    }    
}

//==============================================================================
bool CrunchDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CrunchDistortionAudioProcessor::createEditor()
{
    return new CrunchDistortionAudioProcessorEditor (*this);
}

//==============================================================================
void CrunchDistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CrunchDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CrunchDistortionAudioProcessor();
}