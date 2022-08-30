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
    DeckGUI* _deckgui1, DeckGUI* _deckgui2) : 
    deckgui1 (_deckgui1), deckgui2 (_deckgui2) 
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //trackTitles.push_back("Track 1");
   

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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

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
    searchBar.setText("Search Songs");
    AddSongsToLibaray.setBounds(getWidth() - 150, 10, 130, 25);
    tableComponent.setBounds(0, 50, getWidth(), getHeight() - 60);
}

int PlayListComponent::getNumRows()
{
    return trackTitles.size();
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
    if (columnId == 1) {
        if (rowNumber < getNumRows()) {
            g.drawText(trackTitles[rowNumber],
                2, 0,
                width - 4, height,
                Justification::centredLeft,
                true);
        };
    }

    if (columnId == 2) {
        if (rowNumber < getNumRows()) {
            g.drawText(trackTypes[rowNumber],
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

    auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            auto file = fChooser.getResult();
            PlayListComponent::setTracks(file);
            
        }
    );
}

void PlayListComponent::setTracks(File file)
{
        tracksFile.add(file);
        tracksURL.push_back(file.getFullPathName().toStdString()); 
        trackTitles.push_back(file.getFileNameWithoutExtension().toStdString());
        trackTypes.push_back(file.getFileExtension().toStdString()); 
        
        tableComponent.updateContent();
}

void PlayListComponent::loadLeft(int id ) {
  deckgui1->loadSong(tracksFile[id]); // your player
}

void PlayListComponent::loadRight(int id) {
    deckgui2->loadSong(tracksFile[id]); // your player
    PlayListComponent::searchFilter("b");
}

void PlayListComponent::deleteSong(int id) {

    tracksFile.remove(id); 
    trackTitles.erase(trackTitles.begin() + id); 
    trackTypes.erase(trackTypes.begin() + id); 

    tableComponent.updateContent();
}

void PlayListComponent::textEditorTextChanged(TextEditor& editor) {
    auto text = searchBar.getText().toLowerCase().toStdString(); 
    DBG("the input text is " << text);

    PlayListComponent::searchFilter(text); 
    
}
 

void PlayListComponent::searchFilter(std::string text) {

    /*
    std::string grapText = text;
    std::vector<int> filteredSongs;
    
    //std::copy_if(trackTitles.begin(), trackTitles.end(), std::back_inserter(filteredSongs), [text](auto s) { return s.find(text) != std::string::npos; });

        
  std::copy_if(trackTitles.begin(), trackTitles.end(), back_inserter(filteredSongs), [grapText](auto s) { return s.find(text) != std::string::npos; });
    */
      

  /*
    std::vector<int> filteredSongs;
    std::copy_if(trackTitles.begin(), trackTitles.end(), back_inserter(filteredSongs), [](auto s) { return s.find("b") != std::string::npos; });

    for (auto& i : filteredSongs) {
        DBG("Filtered songs are " << filteredSongs[i]);
    }
  */
    
    Array< juce::File > filteredTracksFile;
  
};


