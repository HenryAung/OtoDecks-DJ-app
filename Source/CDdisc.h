/*
  ==============================================================================

    CDdisc.h
    Created: 31 Aug 2022 12:25:15pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CDdisc  : public juce::Component
{
public:
    CDdisc(AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);

    ~CDdisc() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void loadSong(File file);
    void playSong(); 
    void pauseSong(); 
    void setAngle(float angle); 

    float angle = 0.0; 
    float rotation_speed = 0.01; 

private:

    AudioThumbnail audioThumb;
    bool fileLoaded;
    String songName; 

    bool isPlaying; 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CDdisc)
};
