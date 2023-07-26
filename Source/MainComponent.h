#pragma once

#include <JuceHeader.h>
#include <iostream>
#include <RNBO.h>


class MainComponent  : public juce::AudioAppComponent,
                       public juce::Slider::Listener,
                       public juce::Timer   // Add this line

//                       public RNBO::EventHandler

                       
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
    
    //==============================================================================
    // This is the missing method you need to implement.
    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &volumeSlider)
        {
            rnboObject.setParameterValue(volumeParamIndex, volumeSlider.getValue());
        }
    };
    
    void timerCallback() override
    {
        timeElapsed = rnboObject.getParameterValue(timeParamIndex);
        int totalSeconds = static_cast<int>(timeElapsed);
            int minutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
        
        juce::String timeString = juce::String::formatted("%02d:%02d", minutes, seconds);

        outputLabel.setText(timeString, juce::dontSendNotification);
        
        relativeTimeElapsed = rnboObject.getParameterValue(relativeTimeParamIndex);
        
        timeSlider.setValue(relativeTimeElapsed);

        
        
    }

private:
    //==============================================================================
    RNBO::CoreObject rnboObject;
    RNBO::SampleValue** inputs = new RNBO::SampleValue*[2];
    RNBO::SampleValue** outputs = new RNBO::SampleValue*[2];
    
    juce::Slider volumeSlider;
    juce::Slider timeSlider;
    juce::TextButton playStopButton;
    juce::Label outputLabel;
    
    int timeParamIndex;
    int relativeTimeParamIndex;
    int volumeParamIndex;

    double timeElapsed;
    double relativeTimeElapsed;

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
