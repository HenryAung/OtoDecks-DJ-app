/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1200, 800);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  

    addAndMakeVisible(crossfade); 
    crossfade.addListener(this);
    crossfade.setRange(-1.0f, 0.99999f, 0.01f);
    crossfade.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    crossfadeLabel.setText("Vol Crossfade", dontSendNotification);
    crossfadeLabel.attachToComponent(&crossfade, true);

    addAndMakeVisible(playlistComponent); 

    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
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
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    double height = getHeight() / 12; 
    double width = getWidth() / 12; 
    deckGUI1.setBounds(0, 0, getWidth()/2, height * 8 );
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, height * 8);
    crossfade.setBounds(width, height * 8, width * 10, height * 0.5); 
    playlistComponent.setBounds(0, height * 8.5, getWidth(), getHeight() * 4);
}

void MainComponent::sliderValueChanged(Slider* slider) {
    float crossfadeValue, crossFadeLeft = 1.0, crossFadeRight = 1.0; 
    if (slider == &crossfade) {
        crossfadeValue = crossfade.getValue();
        if (crossfadeValue < 0) {
            crossFadeRight = 1 + crossfadeValue; 
        } 
        else if (crossfadeValue > 0) {
            crossFadeLeft =  1.00000001 - crossfadeValue; 
        }
        
        DBG("crossfade Left :: " << crossFadeLeft);
        DBG("crossfade Right :: " << crossFadeRight);

        player1.setGain(crossFadeLeft);
        player2.setGain(crossFadeRight);
    }
}
