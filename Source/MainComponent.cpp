#include "MainComponent.h"
#include <RNBO.h>


//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    rnboObject.prepareToProcess(sampleRate, samplesPerBlockExpected);
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
                    // inputs[chan][i] =(random.nextFloat()*2.0f - 1.0f) * 0.1f; // can I replace this line by the following ?
                    inputs[chan][i] = channelDataInput[i];

                    }
                    
                }

        auto* channelDataInput = bufferToFill.buffer->getReadPointer (chan, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples ; ++i)
        {
             // inputs[chan][i] =(random.nextFloat()*2.0f - 1.0f) * 0.1f; // can I replace this line by the following ?
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
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
