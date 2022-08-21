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

    getLookAndFeel().setColour(Slider::thumbColourId, Colours::azure); 
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::aliceblue); 
    getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colours::black); 

    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    posLabel.setText("position", dontSendNotification); 
    posLabel.attachToComponent(&posSlider, true); 

    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setRange(0.0f, 2.0f, 0.01f);
    volSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    volSlider.setValue(1.0f);
    volLabel.setText("Volume" , dontSendNotification); 
    volLabel.attachToComponent(&volSlider, false); 

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.1f, 10.0f, 0.01f);
    speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);
    speedSlider.setTextValueSuffix(" x"); 
    speedLabel.setText("Speed", dontSendNotification); 
    speedLabel.attachToComponent(&speedSlider, false); 


    addAndMakeVisible(playButton);
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);

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
    g.fillAll (Colours::dimgrey);   // clear the background

    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

} 

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    double columnW = getWidth() / 8; 

    waveformDisplay.setBounds(0, 0, getWidth(), rowH* 1.3); 
    posSlider.setBounds(80, rowH * 1.3, getWidth()- 80, rowH * 0.4);
 
    volSlider.setBounds(0, rowH * 2.0,columnW * 4 , rowH * 2.3);
    speedSlider.setBounds(columnW * 4, rowH * 2.0, columnW * 4, rowH * 2.3);
    

    playButton.setBounds(columnW, rowH * 5, columnW, rowH);
    stopButton.setBounds(columnW * 4, rowH * 5, columnW, rowH);
    loadButton.setBounds(columnW * 2.5, rowH * 6, columnW, rowH);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("Play button was clicked "); 
        player->start();
    }
     if (button == &stopButton)
    {
         DBG("Stop button was clicked "); 
        player->stop();

    }
       if (button == &loadButton)
    {
       auto fileChooserFlags =  
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
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


    

