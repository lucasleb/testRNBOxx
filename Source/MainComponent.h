#pragma once

#include <JuceHeader.h>
#include <RNBO.h>
#include <juce_core/juce_core.h> // Include the JUCE core module for the Random class.



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    RNBO::CoreObject rnboObject;
    RNBO::SampleValue** inputs = new RNBO::SampleValue*[2];
    RNBO::SampleValue** outputs = new RNBO::SampleValue*[2];
    juce::Random random; // Declare the Random object.


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
