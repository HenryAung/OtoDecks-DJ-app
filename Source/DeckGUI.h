/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "StyleSheet.h"

//==============================================================================
/*
*/
class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);
    ~DeckGUI();

    void paint(Graphics&) override;
    void resized() override;

    /** implement Button::Listener */
    void buttonClicked(Button*) override;

    /** implement Slider::Listener */
    void sliderValueChanged(Slider* slider) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;
    void loadSong(File file);

private:
    WaveformDisplay waveformDisplay;
    Slider posSlider;
    CustomDial volSlider, speedSlider;
    Label volLabel, speedLabel, posLabel;

    TextButton  playButton{ "PLAY" }, 
                pauseButton{ "PAUSE" }, 
                loadButton{ "LOAD" }, 
                stopButton{ "STOP" }, 
                forwardButton{ "FROWARD" }, 
                backwardButton{ "BACKWARD" }, 
                loopButton{ "LOOP" }; 

 

    FileChooser fChooser{"Select a file..."};
    DJAudioPlayer* player; 

    bool isLoaded = false; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
