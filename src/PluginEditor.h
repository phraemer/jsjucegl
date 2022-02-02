/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
  NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);
  ~NewProjectAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  void timerCallback() override;

  juce::TextButton _button;
  juce::Label _frameLabel, _scaleLabel;
  int _frame = 0;
  std::atomic<float> _scale = 1.0f;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  NewProjectAudioProcessor& audioProcessor;

  juce::OpenGLContext _openGLContext;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};
