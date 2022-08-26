/*
  ==============================================================================

    PlayListComponent.cpp
    Created: 16 Aug 2022 3:09:13pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayListComponent.h"

using namespace juce; 

//==============================================================================
PlayListComponent::PlayListComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    //trackTitles.push_back("Track 1");
   

    tableComponent.getHeader().addColumn("Track title", 1, 400);
    tableComponent.getHeader().addColumn("Artist", 2, 200);
    tableComponent.getHeader().addColumn("Audio Type", 3, 130);
    tableComponent.getHeader().addColumn("Duration", 4, 130);
    tableComponent.getHeader().addColumn("", 5, 120);
    tableComponent.getHeader().addColumn("", 6, 120);
    tableComponent.getHeader().addColumn("", 7, 100);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBar);

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

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

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
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::darkgrey);
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
           // int id = std::stoi(btn->getComponentID().toStdString());
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
            //btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    if (columnId == 7)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Delete" };

            String id_str{ std::to_string(rowNumber) };
            //int id = std::stoi(btn->getComponentID().toStdString());
            int id = rowNumber; 

            btn->setComponentID(id_str);
            btn->onClick = [this, id]() { deleteSong(id); };
           // btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }

    return existingComponentToUpdate;
}

/*
void PlayListComponent::buttonClicked(Button* button)
{
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked" + trackTitles[id]);
}
*/

void PlayListComponent::addSongs() {

    auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            auto file = fChooser.getResults();
            PlayListComponent::setTracks(file);
        }
    );
}

void PlayListComponent::setTracks(Array<File> tracksFile)
{
    for (int i = 0; i < tracksFile.size(); i++)
    {
        trackTitles.push_back(tracksFile[i].getFileNameWithoutExtension().toStdString());
        trackTypes.push_back(tracksFile[i].getFileExtension().toStdString()); 
    }
    tableComponent.updateContent();
}

void PlayListComponent::loadLeft(int id ) {
   // player->loadURL(tracksFile[id]); // your player
   // player->start();

    DBG("Player 1 song loaded :: " << id); 
}

void PlayListComponent::loadRight(int id) {
   // player->loadURL(ftracksFile[id]); // your player
  //  player->start();

    DBG("Player 2 song loaded :: " << id); 
}

void PlayListComponent::deleteSong(int id) {
    DBG("Song Deleted"); 
    tracksFile.remove(id); 
    trackTitles.erase(trackTitles.begin() + id); 
    trackTypes.erase(trackTypes.begin() + id); 

    tableComponent.updateContent();
}