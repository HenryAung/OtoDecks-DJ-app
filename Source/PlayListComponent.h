/*
  ==============================================================================

    PlayListComponent.h
    Created: 16 Aug 2022 3:09:13pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

//==============================================================================
/*
*/
class PlayListComponent  : public juce::Component,
    public juce::TableListBoxModel,
    public juce::TextEditor::Listener
{
public:
    PlayListComponent(// DJAudioPlayer* player1, DJAudioPlayer* player2
        DeckGUI* deckgui1, DeckGUI* deckgui2, AudioFormatManager& _formatManager);
    ~PlayListComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;

    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

        Component* refreshComponentForCell(int rowNumber,
            int columnId,
            bool isRowSelected,
            Component* existingComponentToUpdate) override;

        void addSongs(); 
        void setTracks(File file);
        void searchFilter(std::string text); 
        void textEditorTextChanged(TextEditor& editor) override; 

        void loadLeft(int id); 
        void loadRight(int id); 
        void deleteSong(int id); 

private:
    AudioFormatManager& formatManager;
    AudioTransportSource transportSource;
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    TextButton AddSongsToLibaray{ "Add Songs" };
    FileChooser fChooser{ "Select a file..." };

    
    TextEditor searchBar;

    TableListBox tableComponent;

   // std::vector< juce::File > tracksFile; 
    Array< juce::File > tracksFile;

    std::vector < std::string > tracksURL;
    std::vector<std::string> trackTitles;
    std::vector<std::string> trackTypes; 

    DeckGUI* deckgui1; 
    DeckGUI* deckgui2;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayListComponent)
};
