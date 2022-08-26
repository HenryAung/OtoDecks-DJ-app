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

//==============================================================================
/*
*/
class PlayListComponent  : public juce::Component,
    public juce::TableListBoxModel, 
    public Button::Listener
{
public:
    PlayListComponent();
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

        void buttonClicked(Button* button) override;

        void addSongs(); 
        void setTracks(Array<File> tracksFile);

private:
    TextButton AddSongsToLibaray{ "Add Songs" };
    FileChooser fChooser{ "Select a file..." };


    TextEditor searchBar;

    TableListBox tableComponent; 
    Array < juce:: File > tracksFile;
    std::vector<std::string> trackTitles;
    std::vector<std::string> trackTypes; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayListComponent)
};
