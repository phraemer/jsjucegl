/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#define JSJUCEGL_USE_OPENGL
#define JSJUCEGL_RESIZE_REATTACH_GL
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
  juce::Label _label;
  int _counter = 0;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  NewProjectAudioProcessor& audioProcessor;

#if defined(JSJUCEGL_USE_OPENGL)
  juce::OpenGLContext _openGLContext;
#endif

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};
