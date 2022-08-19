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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    double columnW = getWidth() / 8; 

    waveformDisplay.setBounds(0, 0, getWidth(), rowH* 1.3);
    posSlider.setBounds(80, rowH * 1.3, getWidth()- 80, rowH * 0.5);
 
    volSlider.setBounds(0, rowH * 2.8 ,columnW * 4 , rowH * 2.3);
    speedSlider.setBounds(columnW * 4, rowH * 2.8, columnW * 4, rowH * 2.3);
    

    playButton.setBounds(columnW, rowH * 5, columnW, rowH);
    stopButton.setBounds(columnW * 4, rowH * 5, columnW, rowH);
    loadButton.setBounds(columnW * 2.5, rowH * 6, columnW, rowH);

}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
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
    // if (button == &loadButton)
    // {
    //     FileChooser chooser{"Select a file..."};
    //     if (chooser.browseForFileToOpen())
    //     {
    //         player->loadURL(URL{chooser.getResult()});
    //         waveformDisplay.loadURL(URL{chooser.getResult()});
            
    //     }


    // }
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
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}


    

