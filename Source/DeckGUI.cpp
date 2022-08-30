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
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
    
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
    volLabel.setText("Volume" , dontSendNotification); 
    volLabel.attachToComponent(&volSlider, false); 

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1f, 5.0f, 0.01f);
    speedSlider.setTextValueSuffix(" x"); 
    speedLabel.setText("Speed", dontSendNotification); 
    speedLabel.attachToComponent(&speedSlider, false); 


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

    addAndMakeVisible(loopButton);
    loopButton.addListener(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    startTimer(500);


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

    
    double W = getWidth() / 8;
    double bigCircleSize = W * 2.7;
    double bigCircleX = ((8*W) - bigCircleSize) / 2; 
    double bigCircleY = (getHeight() - bigCircleSize) * 0.7;

    double smallCircleSize = W * 0.6; 
    double smallCircleX = ((8*W) - smallCircleSize) / 2; 
    double smallCircleY = bigCircleY + ((bigCircleSize - smallCircleSize) / 2);

    g.setColour(Colours::darkcyan);
    g.fillEllipse(bigCircleX, bigCircleY , bigCircleSize , bigCircleSize);

    g.setColour(Colours::azure); 
    g.fillEllipse(smallCircleX, smallCircleY , smallCircleSize, smallCircleSize);

    if (isLoaded) {
        g.drawText("Songs Name", smallCircleX, smallCircleY - 10, smallCircleSize, smallCircleSize, Justification::centred, true); 
    }
    
} 

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    double columnW = getWidth() / 8; 

    waveformDisplay.setBounds(0, 0, getWidth(), rowH* 1.3); 
    posSlider.setBounds(80, rowH * 1.4, getWidth()- 80, rowH * 0.4);
 
    volSlider.setBounds(0, rowH * 2.35,columnW * 4 , rowH * 1.8);
    speedSlider.setBounds(columnW * 4, rowH * 2.35, columnW * 4, rowH * 1.8);
    

    playButton.setBounds(columnW * 0.5 , rowH * 7.1, columnW  , rowH * 0.6);
    pauseButton.setBounds(columnW * 2, rowH * 7.1, columnW , rowH * 0.6);
    stopButton.setBounds(columnW * 3.5, rowH * 7.1, columnW , rowH * 0.6);
    forwardButton.setBounds(columnW * 5, rowH * 7.1, columnW , rowH * 0.6);
    backwardButton.setBounds(columnW * 6.5, rowH * 7.1, columnW , rowH * 0.6);
    loopButton.setBounds(columnW * 3.6, rowH , columnW * 0.8, rowH * 0.6);
    loadButton.setBounds(columnW * 5.8, rowH , columnW * 0.8, rowH * 0.6);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked "); 
        player->start();
    }
     if (button == &pauseButton)
    {
         DBG("Stop button was clicked "); 
        player->pause();
    }

     if (button == &stopButton)
     {
         DBG("Stop button was clicked ");
         player->stop();
     }

     if (button == &backwardButton)
     {
         player->backward(); 
     }
     if (button == &forwardButton)
     {
         player->forward(); 
     }
     if (button == &loopButton)
     {
         player->setLoop(); 
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
}


void DeckGUI::loadSong(File file ) {
    player->loadURL(URL{ file });
    isLoaded = true; 
    // and now the waveformDisplay as well
    waveformDisplay.loadURL(URL{ file });
}
