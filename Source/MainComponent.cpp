#include "MainComponent.h"
#include <RNBO.h>


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
    volumeSlider.addListener (this);
    volumeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);

    
    
//    addAndMakeVisible (volumeLabel);
//    volumeLabel.setText ("Volume", juce::dontSendNotification);
//    volumeLabel.attachToComponent (&volumeSlider, true); // [4]
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    rnboObject.prepareToProcess(sampleRate, samplesPerBlockExpected);
    
//    int n = rnboObject.getNumParameters();
//    for (int i = 0; i < n; i++) {
//        const char *id = rnboObject.getParameterId(i);
//        const char *name = rnboObject.getParameterName(i);
//    }
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
    volumeSlider.setBounds (20, getHeight() - 60 - 60, getWidth() - 40, 60);

}

