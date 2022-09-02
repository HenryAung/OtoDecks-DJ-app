/*
  ==============================================================================

    CDdisc.cpp
    Created: 31 Aug 2022 12:25:15pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CDdisc.h"

//==============================================================================
CDdisc::CDdisc(AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse) :
    audioThumb(1000, formatManagerToUse, cacheToUse),
    fileLoaded(false), 
    isPlaying(false)
{
    //audioThumb.addChangeListener(this);
}

CDdisc::~CDdisc()
{
}

void CDdisc::paint(juce::Graphics& g)
{
    double smallCricXY = (getWidth() - (getWidth() * 0.2)) / 2;
    
    if (fileLoaded) {
        g.addTransform(AffineTransform::rotation(angle, getWidth()/ 2, getHeight() / 2)); 
        g.setColour(Colours::darkslategrey);
        g.fillEllipse(0, 0, getWidth()-1.0, getWidth() +1.0);

        g.setColour(Colours::azure);
        g.fillEllipse(smallCricXY, smallCricXY, getWidth() * 0.2, getWidth() * 0.2);
        
        g.setColour(Colours::darkcyan);
        g.drawEllipse(0.0, 0.0, getWidth() - 1.5, getWidth() + 1.5, 3);
        g.drawEllipse(smallCricXY, smallCricXY, getWidth() * 0.2, getWidth() * 0.2, 3); 

        
        g.setColour(Colours::azure); 
        g.setFont(20.0f); 
        g.drawText(songName, 0.0, getHeight()* 0.25, getWidth() , getHeight(), Justification::centred, true); 

        if (isPlaying) {
            angle += rotation_speed;
        }
        
    } 

    else {
        g.setFont(20.0f);
        g.drawText("Please load your song...", getLocalBounds(),
            Justification::centred, true);   // draw some placeholder text
        g.addTransform(AffineTransform::rotation(angle));
    }

}

void CDdisc::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void CDdisc::loadSong(File file)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(URL{ file }));

    songName = file.getFileNameWithoutExtension(); 

    if (fileLoaded)
    {
        DBG("CDdisc: loaded! ");
        repaint();
    }
    else {
        std::cout << "wfd: not loaded! " << std::endl;
    }

}

void CDdisc::playSong() {
    isPlaying = true; 
}

void CDdisc::pauseSong() {
    isPlaying = false; 
}

void CDdisc::setAngle(float ang) {
    if (ang >= 0) {
        angle = ang; 
        repaint();
    }
    isPlaying = false; 
}