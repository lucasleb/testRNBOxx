#include "MainComponent.h"
#include <RNBO.h>
#include <iostream>

//

//==============================================================================
MainComponent::MainComponent()

{

    setSize (337, 600);

    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
    
    addAndMakeVisible (volumeSlider);
    volumeSlider.setRange (-70, 0);
    volumeSlider.setValue(-12);
    volumeSlider.addListener (this);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    
    addAndMakeVisible (timeSlider);
    timeSlider.setRange (0.0, 1.0);
    timeSlider.setValue(0.0);
    timeSlider.addListener (this);
    timeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    timeSlider.setSliderStyle(juce::Slider::LinearBar);
    timeSlider.setEnabled(false);
    
    playStopButton.setButtonText("Play"); // Initial text
    playStopButton.onClick = [this] {
        if (playStopButton.getButtonText() == "Play") {
            playStopButton.setButtonText("Stop");
            rnboObject.sendMessage(RNBO::TAG("state"), 1, RNBO::TAG("")); // Signal to start playing
            startTimer(40); // Start the timer when the audio starts playing

        } else {
            playStopButton.setButtonText("Play");
            rnboObject.sendMessage(RNBO::TAG("state"), 0, RNBO::TAG("")); // Signal to stop playing
            stopTimer(); // Stop the timer when the audio stops playing
            outputLabel.setText("00:00", juce::dontSendNotification);
            timeSlider.setValue(0.0);
        }
    };
    addAndMakeVisible(playStopButton);
    
    outputLabel.setJustificationType(juce::Justification::centredLeft);
    outputLabel.setFont(juce::Font(16.0f));
    outputLabel.setText("00:00", juce::dontSendNotification);
    
    addAndMakeVisible(outputLabel);
    
   


}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    rnboObject.prepareToProcess(sampleRate, samplesPerBlockExpected);
    volumeParamIndex = rnboObject.getParameterIndexForID("volume");
    timeParamIndex = rnboObject.getParameterIndexForID("time");
    relativeTimeParamIndex = rnboObject.getParameterIndexForID("relativeTime");
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* device = deviceManager.getCurrentAudioDevice();
    auto activeInputChannels  = device->getActiveInputChannels();
    auto maxInputChannels  = activeInputChannels .getHighestBit() + 1;

    for(int i = 0; i < 2; i++)
        inputs[i] = new double[bufferToFill.numSamples];

    
    for(int i = 0; i < 2; i++)
        outputs[i] = new double[bufferToFill.numSamples];
    
    
    for (auto chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
    {
        auto actualInputChannel = chan % maxInputChannels; // [1]
        
        if (! activeInputChannels[chan]) // [2]
                {
                    bufferToFill.buffer->clear (chan, bufferToFill.startSample, bufferToFill.numSamples);
                }
        else
                {
                    auto* channelDataInput = bufferToFill.buffer->getReadPointer (actualInputChannel, bufferToFill.startSample);
                    for (auto i = 0; i < bufferToFill.numSamples ; ++i)
                    {
                    inputs[chan][i] = channelDataInput[i];
                    }
                }

        auto* channelDataInput = bufferToFill.buffer->getReadPointer (chan, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples ; ++i)
        {
             inputs[chan][i] = channelDataInput[i];
        }
    }

    rnboObject.process(inputs, 2, outputs, 2, bufferToFill.numSamples);
    
    for (auto chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
    {
        auto* channelData = bufferToFill.buffer->getWritePointer (chan, bufferToFill.startSample);
        for (auto i = 0; i < bufferToFill.numSamples ; ++i)
        {
            channelData[i] = outputs[chan][i];
        }
    }
    
    for(int i = 0; i < 2; i++) {
    delete[] inputs[i];
    delete[] outputs[i];
}
}

void MainComponent::releaseResources()
{}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized() 
{
    volumeSlider.setBounds (100, getHeight() - 60 - 120, getWidth() - 200, 60);
    timeSlider.setBounds (20, getHeight() - 30 - 60, getWidth() - 40, 10);
    playStopButton.setBounds(getWidth()/4, getHeight()/3, getWidth()/2, getWidth()/4);
    outputLabel.setBounds (15, getHeight() - 30 - 40, 120, 20);

}

