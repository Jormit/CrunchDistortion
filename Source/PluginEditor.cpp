/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CrunchDistortionAudioProcessorEditor::CrunchDistortionAudioProcessorEditor (CrunchDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Gain Slider setup.
    addAndMakeVisible (gainSlider);
    gainSlider.setRange (0.0, 30.0f, 0.5f);
    gainSlider.setTextValueSuffix ("dB");
    gainSlider.setValue (processor.mGain);
    gainSlider.addListener (this);
    
    addAndMakeVisible(gainLabel);
    gainLabel.setText ("Gain", dontSendNotification);
    gainLabel.attachToComponent (&gainSlider, true);
    
    // Presence Slider setup.
    addAndMakeVisible (presenceSlider);
    presenceSlider.setRange (0.0, 10.0f, 0.5f);
    presenceSlider.setValue (processor.mPresence);
    presenceSlider.addListener (this);
    
    addAndMakeVisible(presenceLabel);
    presenceLabel.setText ("Tone", dontSendNotification);
    presenceLabel.attachToComponent (&presenceSlider, true);    
    
    // Algo selection setup.    
    addAndMakeVisible (gainStyle);
    gainStyle.addItem ("Soft", 1);
    gainStyle.addItem ("Hard", 2);
    
    gainStyle.onChange = [this] { gainStyleChanged(); };
    gainStyle.setSelectedId (processor.mCurrentGainStyle);
    
    
    setSize (500, 130);
}

CrunchDistortionAudioProcessorEditor::~CrunchDistortionAudioProcessorEditor()
{
}

//==============================================================================
void CrunchDistortionAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    
}

void CrunchDistortionAudioProcessorEditor::resized()
{
    auto margin = 20;
    auto labelSpace = 40;
    gainSlider.setBounds (margin + labelSpace, 20, getWidth() - 2 * margin - labelSpace, 20);   
    presenceSlider.setBounds (margin + labelSpace, 50, getWidth() - 2 * margin - labelSpace, 20);      
    gainStyle.setBounds (margin , 80, getWidth() - 2 * margin , 20);

}

void CrunchDistortionAudioProcessorEditor::sliderValueChanged(Slider *slider) 
{
    
    processor.mGain = gainSlider.getValue();
    processor.mPresence = presenceSlider.getValue();
}

void CrunchDistortionAudioProcessorEditor::gainStyleChanged()
{
    gainStyle.setSelectedId(gainStyle.getSelectedId());
    processor.mCurrentGainStyle = gainStyle.getSelectedId();
    
}