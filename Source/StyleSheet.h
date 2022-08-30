/*
  ==============================================================================

    StyleSheet.h
    Created: 29 Aug 2022 4:23:53pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//#include "MainComponent.h"
//==============================================================================

class StyleSheet  : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(Graphics&,
        int 	x,
        int 	y,
        int 	width,
        int 	height,
        float 	sliderPosProportional,
        float 	rotaryStartAngle,
        float 	rotaryEndAngle,
        Slider&
    );

    void drawLinearSlider(Graphics&, 
        int x, 
        int y, 
        int width, 
        int height,
        float sliderPos, 
        float minSliderPos, 
        float maxSliderPos,
        const Slider::SliderStyle, Slider&) override;
};

class CustomDial : public juce::Slider {
public : 
    CustomDial() {
        
        setSliderStyle(Slider::SliderStyle::Rotary);
        setColour(juce::Slider::ColourIds::thumbColourId, Colours::azure);
        setColour(juce::Slider::ColourIds::rotarySliderFillColourId, Colours::azure);
        setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
        setValue(1.0f);
    }
private :
}; 
