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

private:

    AudioThumbnail audioThumb;
    bool fileLoaded;
    String songName; 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CDdisc)
};
