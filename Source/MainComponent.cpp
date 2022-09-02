/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        setAudioChannels (0, 2); // Specify the number of input and output channels that we want to open
    }  
    

    setSize(1200, 800); /// Setting the main window for app 
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustonSytle);   /// setting custom look and feel class 

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  

    addAndMakeVisible(crossfade); 
    crossfade.addListener(this);
    crossfade.setRange(-2.0f, 1.99999f, 0.01f);
    crossfade.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(playlistComponent); 

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr); 
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(Colours::darkslategrey);

    /// setting label for crossfade 
    g.setColour(Colours::azure); 
    g.drawText("Left Speaker", 0, getHeight() * 7.4 / 12, getWidth() / 12, getHeight() / 12 * 0.8, Justification::centred, true); 
    g.drawText("Right Speaker", getWidth() / 12 * 11, getHeight() * 7.4 / 12, getWidth() / 12, getHeight() / 12 * 0.8, Justification::centred, true);
}

void MainComponent::resized()
{
    double height = getHeight() / 12; 
    double width = getWidth() / 12; 

    deckGUI1.setBounds(0, 0, getWidth()/2, height * 7.5 );
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, height * 7.5);
    crossfade.setBounds(width, height * 7.5, width * 10, height * 0.8 ); 
    playlistComponent.setBounds(0, height * 8.3, getWidth(), getHeight() * 3.7);
}

void MainComponent::sliderValueChanged(Slider* slider) {

    float crossfadeValue, crossFadeLeft = 2.0, crossFadeRight = 2.0; 

    if (slider == &crossfade) {
        crossfadeValue = crossfade.getValue();
        if (crossfadeValue < 0) {
            crossFadeRight = 2 + crossfadeValue; 
        } 
        else if (crossfadeValue > 0) {
            crossFadeLeft =  2.00000001 - crossfadeValue; 
        }
        player1.setGain(crossFadeLeft);
        player2.setGain(crossFadeRight);
    }
}
