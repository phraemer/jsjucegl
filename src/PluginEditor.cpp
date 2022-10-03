/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

class CallOnceAfter : public juce::Timer
{
public:
  CallOnceAfter(const int delayInMS, std::function<void()> callback, bool startTimerNow = true)
    : _ms(delayInMS), _callback(callback)
  {
    if (startTimerNow)
    {
      startTimer(_ms);
    }
  }

  // Calls the passed callback again after delayInMS if it has been called already
  void reset(const int delayInMS)
  {
    if (!isTimerRunning())
    {
      _ms = delayInMS;
      startTimer(_ms);
    }
  }

private:
  int _ms;
  std::function<void()> _callback;

  void timerCallback() override
  {
    stopTimer();
    if (_callback)
    {
      _callback();
    }
  }
};

namespace {
  std::unique_ptr<CallOnceAfter> g_callOnce;
}

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
  : AudioProcessorEditor(&p),
  audioProcessor(p),
  _button("Embiggen", "Click to detatch and reattach OpenGL"),
  _frameLabel("0"),
  _scaleLabel("")
{
  _openGLContext.setOpenGLVersionRequired(juce::OpenGLContext::OpenGLVersion::openGL3_2);
  _openGLContext.attachTo(*getTopLevelComponent());

  _button.onClick = [this] {
    g_callOnce = std::make_unique<CallOnceAfter>(5000, [this] {

      auto* pComponent = _openGLContext.getTargetComponent();

      DBG("DETACH OPENGL");

      // Detaching and reattaching prevents a visual stretching effect
      if (_openGLContext.isAttached())
      {
        _openGLContext.detach();
      }

      DBG("RE-ATTACH OPENGL");
      _openGLContext.attachTo(*pComponent);

      DBG("ATTACHED OPENGL");

      });
  };
  
  _edit.onReturnKey = []{
    DBG("TextEdit return key");
  };

  addAndMakeVisible(_button);
  addAndMakeVisible(_edit);
  addAndMakeVisible(_frameLabel);
  addAndMakeVisible(_scaleLabel);

  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(800, 332);

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
  _edit.setBounds(bounds.removeFromBottom(25).removeFromLeft(100));
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
