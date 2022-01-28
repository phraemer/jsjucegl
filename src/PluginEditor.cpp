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
  _button("Embiggen", "Click to resize"),
  _frameLabel("0"),
  _scaleLabel("")
{
  _openGLContext.attachTo(*getTopLevelComponent());
  _openGLContext.setContinuousRepainting(true);

  _button.onClick = [this] {

#if defined(JSJUCEGL_USE_OPENGL) && defined(JSJUCEGL_RESIZE_REATTACH_GL)
    auto* pComponent = _openGLContext.getTargetComponent();

    // Detaching and reattaching prevents a visual stretching effect
    if (_openGLContext.isAttached())
    {
      _openGLContext.detach();
    }
#endif

    if (getWidth() == 200)
    {
      setSize(300, 300);
      _button.setButtonText("Enshrinken");
    }
    else
    {
      setSize(200, 200);
      _button.setButtonText("Embiggen");
    }

#if defined(JSJUCEGL_USE_OPENGL) && defined(JSJUCEGL_RESIZE_REATTACH_GL)
    _openGLContext.attachTo(*pComponent);
#endif
  };

  addAndMakeVisible(_button);
  addAndMakeVisible(_frameLabel);
  addAndMakeVisible(_scaleLabel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(200, 200);

  startTimerHz(30);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  _scale = g.getInternalContext().getPhysicalPixelScaleFactor();
}

void NewProjectAudioProcessorEditor::resized()
{
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..

  auto bounds = getLocalBounds();

  _button.setBounds(bounds.removeFromBottom(25).removeFromLeft(100));
  _frameLabel.setBounds(bounds.removeFromBottom(25).removeFromLeft(100));
  _scaleLabel.setBounds(bounds.removeFromBottom(25).removeFromLeft(100));
}

void NewProjectAudioProcessorEditor::timerCallback()
{
  _frame++;
  _frame %= 30;

  _frameLabel.setText("Frame:" + juce::var(_frame).toString(), juce::NotificationType::dontSendNotification);
  _scaleLabel.setText("Scale: " + juce::var(_scale).toString(), juce::NotificationType::dontSendNotification);

  repaint();
}
