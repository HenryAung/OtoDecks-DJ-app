/*
  ==============================================================================

    PlayListComponent.cpp
    Created: 16 Aug 2022 3:09:13pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayListComponent.h"
#include <algorithm>  // for copy_if
#include <iterator>  
using namespace juce; 

//==============================================================================
PlayListComponent::PlayListComponent(
    DeckGUI* _deckgui1, DeckGUI* _deckgui2, AudioFormatManager& _formatManager)
    : deckgui1(_deckgui1), deckgui2(_deckgui2), formatManager(_formatManager), SearchBarIsEmpty(true)
{
    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Audio Type", 2, 200);
    tableComponent.getHeader().addColumn("Duration", 4, 200);
    tableComponent.getHeader().addColumn("", 5, 150);
    tableComponent.getHeader().addColumn("", 6, 150);
    tableComponent.getHeader().addColumn("", 7, 100);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBar);
    searchBar.addListener(this); 

    addAndMakeVisible(AddSongsToLibaray);
    AddSongsToLibaray.onClick = [this]() { addSongs(); };
}

PlayListComponent::~PlayListComponent()
{
}

void PlayListComponent::paint (juce::Graphics& g)
{
    g.fillAll(Colours::lightcoral); 

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlayListComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlayListComponent::resized()
{
    searchBar.setBounds(10, 10, 200, 25);
    AddSongsToLibaray.setBounds(getWidth() - 150, 10, 130, 25);
    tableComponent.setBounds(0, 50, getWidth(), getHeight() - 60);
}

int PlayListComponent::getNumRows()
{
    Array <File> input;

    if (SearchBarIsEmpty == true) {
        input = tracksFile;
    }

    if (SearchBarIsEmpty == false) {
        input = filteredTracksFile;
    }
    return input.size();
}

void PlayListComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::darkgrey);
    }
    else {
        g.fillAll(Colours::lightcoral);
    }
}

void PlayListComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{   
    Array <File> input; 

    if (SearchBarIsEmpty == true) {
         input = tracksFile;
    } 

    if (SearchBarIsEmpty == false) {
         input = filteredTracksFile;
    }
    

    if (columnId == 1) {        
        if (rowNumber < getNumRows()) {
            g.drawText(input[rowNumber].getFileNameWithoutExtension(),
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        };
    }

    // song type 
    if (columnId == 2) {
        if (rowNumber < getNumRows()) {
            g.drawText(input[rowNumber].getFileExtension() ,
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        };
    }

    // song duration
    if (columnId == 4) {
        if (rowNumber < getNumRows()) {

            auto* reader = formatManager.createReaderFor(input[rowNumber]); //
            if (reader != nullptr) // good file!  
            {
                std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                readerSource.reset(newSource.release());
            }
            if (reader == nullptr) {
                DBG("Songs is not a good file");
            }
            
            int totalSongLenghtInSecond = transportSource.getLengthInSeconds();

            std::string songLengthInMin_str = std::to_string(totalSongLenghtInSecond / 60);
            std::string songLengthInSec_str = std::to_string(totalSongLenghtInSecond % 60);
            
            g.drawText(songLengthInMin_str + ":" + songLengthInSec_str,
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        };
    }
    
}

Component* PlayListComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Load Left" };

            String id_str{ std::to_string(rowNumber) };
            int id = rowNumber; 

            btn->setComponentID(id_str); 
            btn->onClick = [this, id]() { loadLeft(id); };
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 6)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Load Right" };

            String id_str{ std::to_string(rowNumber) };
            int id = rowNumber;

            btn->setComponentID(id_str);
            btn->onClick = [this, id]() { loadRight(id); };
 
            existingComponentToUpdate = btn;
        }
    }

    if (columnId == 7)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Delete" };

            String id_str{ std::to_string(rowNumber) };
            int id = rowNumber; 

            btn->setComponentID(id_str);
            btn->onClick = [this, id]() { deleteSong(id); };

            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}



void PlayListComponent::addSongs() {

    auto fileChooserFlags = FileBrowserComponent::canSelectMultipleItems;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            auto file = fChooser.getResults();
            PlayListComponent::setTracks(file);
        }
    );
}

void PlayListComponent::setTracks(Array <File> file)
{
        tracksFile.addArray(file);        
        tableComponent.updateContent();
}

void PlayListComponent::loadLeft(int id ) {
    File input;

    if (SearchBarIsEmpty == true) {
        input = tracksFile[id];
    }

    if (SearchBarIsEmpty == false) {
        input = filteredTracksFile[id];
    }

    deckgui1->loadSong(input); // your player
    tableComponent.updateContent();
}

void PlayListComponent::loadRight(int id) {
    File input;

    if (SearchBarIsEmpty == true) {
        input = tracksFile[id];
    }

    if (SearchBarIsEmpty == false) {
        input = filteredTracksFile[id];
    }
    deckgui2->loadSong(input); // your player
    tableComponent.updateContent();
}

void PlayListComponent::deleteSong(int id) {
    tracksFile.remove(id); 
    tableComponent.updateContent();
} 

void PlayListComponent::textEditorTextChanged(TextEditor& editor) {

    PlayListComponent::searchBarEmptyCheck();

    if (SearchBarIsEmpty == false) {
        auto text = searchBar.getText().toLowerCase().toStdString();
        DBG("the input text is " << text);
        filteredTracksFile.clear();  // to clear out previous recorded data 

        for (auto& file : tracksFile) {
            auto filename = file.getFileNameWithoutExtension().toStdString();

            if (filename.rfind(text, 0) == 0) {
                filteredTracksFile.add(file);
                DBG("filtered Tracks name is >> " << filename);
            }
        }
        DBG(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        DBG("filtered Tracks file size is >> " << filteredTracksFile.size());
    }
    //tableComponent.updateContent(); 
    repaint(); 
}
 


bool PlayListComponent::searchBarEmptyCheck() {
    if (searchBar.isEmpty()) {
        SearchBarIsEmpty = true; 
    }

    if (!searchBar.isEmpty()) {
        SearchBarIsEmpty = false;
    }

    return SearchBarIsEmpty; 
}