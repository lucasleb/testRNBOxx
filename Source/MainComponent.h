#pragma once

#include <JuceHeader.h>
#include <RNBO.h>
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Slider::Listener
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
            int parameterIndex = rnboObject.getParameterIndexForID("volume");
            rnboObject.setParameterValue(parameterIndex, volumeSlider.getValue());
        }
            
        
        


                    
    };


private:
    //==============================================================================
    RNBO::CoreObject rnboObject;
    RNBO::SampleValue** inputs = new RNBO::SampleValue*[2];
    RNBO::SampleValue** outputs = new RNBO::SampleValue*[2];
    
    juce::Slider volumeSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
