/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CrunchDistortionAudioProcessorEditor  : public AudioProcessorEditor,
                                              private Slider::Listener
{
public:
    CrunchDistortionAudioProcessorEditor (CrunchDistortionAudioProcessor&);
    ~CrunchDistortionAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    CrunchDistortionAudioProcessor& processor;
    
    void sliderValueChanged (Slider* slider) override;
    void gainStyleChanged();
    
    // UI components.
    Slider gainSlider;
    Slider presenceSlider;
    
    Label gainLabel;
    Label presenceLabel;
    
    ComboBox gainStyle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrunchDistortionAudioProcessorEditor)
};