/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse)
    :   player(_player),
        waveformDisplay(formatManagerToUse, cacheToUse), 
        cd(formatManagerToUse, cacheToUse)
              
{
    addAndMakeVisible(waveformDisplay);

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    posLabel.setText("position", dontSendNotification); 
    posLabel.attachToComponent(&posSlider, true); 

    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setRange(0.0f, 2.0f, 0.01f);

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1f, 5.0f, 0.01f);
    speedSlider.setTextValueSuffix(" x"); 

    addAndMakeVisible(cd); 

    addAndMakeVisible(playButton);
    playButton.addListener(this);

    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);

    addAndMakeVisible(forwardButton);
    forwardButton.addListener(this);

    addAndMakeVisible(backwardButton);
    backwardButton.addListener(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    startTimer(500);
    startTimerHz(60); 
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.fillAll (Colours::lightcoral);   // clear the background

    g.setColour (Colours::lightgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    double rowH = getHeight() / 8;
    double columnW = getWidth() / 8;

    // draw lable for volume and speed sliders 
    g.setColour(Colours::azure); 
    g.setFont(20.0f); 
    g.drawText("Volume", 30.0, rowH * 2.35, columnW * 4, rowH * 1.2, Justification::centredLeft, true); 
    g.drawText("Speed", columnW * 4, rowH * 2.35, columnW * 3.5, rowH * 1.2, Justification::centredRight, true); 
    
} 

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    double columnW = getWidth() / 8; 

    waveformDisplay.setBounds(0, 0, getWidth(), rowH* 1.3); 
    posSlider.setBounds(80, rowH * 1.4, getWidth()- 80, rowH * 0.4);
 
    volSlider.setBounds(0, rowH * 2.35,columnW * 4 , rowH * 1.8);
    speedSlider.setBounds(columnW * 4, rowH * 2.35, columnW * 4, rowH * 1.8);

    cd.setBounds((8 * columnW - (2.7 * columnW)) / 2, (getHeight() - (2.7 * columnW)) * 0.7, columnW * 2.7, columnW * 2.7); 

    playButton.setBounds(columnW * 0.5 , rowH * 5, columnW  , rowH * 0.6);
    loadButton.setBounds(columnW * 6, rowH * 5, columnW , rowH * 0.6);

    pauseButton.setBounds(columnW * 0.8, rowH * 7.1, columnW , rowH * 0.6);
    stopButton.setBounds(columnW * 2.6, rowH * 7.1, columnW , rowH * 0.6);
    forwardButton.setBounds(columnW * 4.4, rowH * 7.1, columnW , rowH * 0.6);
    backwardButton.setBounds(columnW * 6.2, rowH * 7.1, columnW , rowH * 0.6);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked "); 
        player->start();
        cd.playSong(); 
    }
     if (button == &pauseButton)
    {
         DBG("Stop button was clicked "); 
        player->pause();
        cd.pauseSong(); 
    }

     if (button == &stopButton)
     {
         DBG("Stop button was clicked ");
         player->stop();
         waveformDisplay.setPositionRelative(0); 
         cd.setAngle(0); 
     }

     if (button == &backwardButton)
     {
         player->backward(); 
     }
     if (button == &forwardButton)
     {
         player->forward(); 
     }

     if (button == &loadButton)
     {
       auto fileChooserFlags =  
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
                DeckGUI::loadSong(fChooser.getResult()); 
        });
     }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    
}

void DeckGUI::loadSong(File file) {
    player->loadURL(URL{ file }); // loading songs to player 
    waveformDisplay.loadURL(URL{ file }); // and now the waveformDisplay as well
    cd.loadSong(file); // loading song to cd to animate
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
    DBG("DeckGUI::isInterestedInFileDrag"); 
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
    DBG("DeckGUI::filesDropped"); 
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());

    cd.repaint(); 
}


