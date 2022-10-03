/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
  : AudioProcessorEditor(&p),
  audioProcessor(p),
  _frameLabel("")
{
  _edit.onReturnKey = [this]{
    _frameLabel.setText("onReturnKey " + juce::String(_counter++), juce::NotificationType::dontSendNotification);
  };
  
  _edit.onTextChange = [this]{
    _frameLabel.setText("onTextChange" + juce::String(_counter++), juce::NotificationType::dontSendNotification);
  };

  addAndMakeVisible(_edit);
  addAndMakeVisible(_frameLabel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(800, 332);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void NewProjectAudioProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  auto bounds = getLocalBounds();

  _edit.setBounds(bounds.removeFromBottom(25).removeFromLeft(100));
  _frameLabel.setBounds(bounds.removeFromBottom(25).removeFromLeft(100));
}

