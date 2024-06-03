/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MakoBiteAudioProcessorEditor::MakoBiteAudioProcessorEditor (MakoBiteAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //R1.00 Create SLIDER ATTACHMENTS so our parameter vars get adjusted automatically for Get/Set states.
    ParAtt_Gain     = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "gain", jsP1_Gain);
    ParAtt_NGate = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "ngate", jsP1_NGate);
    ParAtt_HighPass = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "highpass", jsP1_HighPass);
    ParAtt_MidFreq  = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "midfreq", jsP1_MidFreq);
    ParAtt_MidQ     = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "midq", jsP1_MidQ);
    ParAtt_Drive    = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "drive", jsP1_Drive);
    ParAtt_LowPass  = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "lowpass", jsP1_LowPass);
    
    ParAtt_EnhLow   = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "enhlow", jsP1_EnhLow);
    ParAtt_EnhLowFreq = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "enhlowf", jsP1_EnhLowFreq);
    ParAtt_EnhHigh  = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "enhhigh", jsP1_EnhHigh);
    ParAtt_EnhHighFreq = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "enhhighf", jsP1_EnhHighFreq);

    ParAtt_IR_Model = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "irmodel", jsP1_IR_Model);
    ParAtt_IR_On = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "iron", jsP1_IR_On);
    ParAtt_ClipMode = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "clipmode", jsP1_ClipMode);
    ParAtt_Mono = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mono", jsP1_Mono);
    ParAtt_Thin = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "thin", jsP1_Thin);
    ParAtt_CompThresh = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "comp", jsP1_CompThresh);

    ParAtt_Thump = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "thump", jsP1_Thump);
    ParAtt_Band = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band", jsP1_Band);
    ParAtt_Band1 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band1", jsP1_Band1);
    ParAtt_Band2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band2", jsP1_Band2);
    ParAtt_Band3 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band3", jsP1_Band3);
    ParAtt_Band4 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band4", jsP1_Band4);
    ParAtt_Band5 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "band5", jsP1_Band5);
   
    imgLogo = juce::ImageCache::getFromMemory(BinaryData::stackedlogo_png, BinaryData::stackedlogo_pngSize);
    imgBackground = juce::ImageCache::getFromMemory(BinaryData::makostacked_back_png, BinaryData::makostacked_back_pngSize);

    //R1.00 PRESET COMBO BOX
    cbPreset.setColour(juce::ComboBox::textColourId, juce::Colour(192, 192, 192));
    cbPreset.setColour(juce::ComboBox::backgroundColourId, juce::Colour(32, 32, 32));
    cbPreset.setColour(juce::ComboBox::arrowColourId, juce::Colour(192, 192, 192));
    cbPreset.setColour(juce::ComboBox::outlineColourId, juce::Colour(192, 0, 0));
    addAndMakeVisible(cbPreset);
    //cbPreset.addSectionHeading("PRESETS");
    cbPreset.addItem("---", 1);
    cbPreset.addItem("Southern", 2);
    cbPreset.addItem("Voltage", 3);
    cbPreset.addItem("Hard Cafe", 4);
    cbPreset.addItem("Zinc", 5);
    cbPreset.addItem("Mad Man", 6);
    cbPreset.addItem("The Feels", 7);
    cbPreset.onChange = [this] { cbPresetChanged(); };    //R1.00 PedalChanged is a func we create and gets called on combo selection.
    cbPreset.setSelectedId(1);

    //****************************************************************************************
    //R1.00 Add GUI CONTROLS
    //****************************************************************************************
    GUI_Init_Large_Slider(&jsP1_Gain);
    jsP1_Gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Gain.setRange(0.0f, 1.0f, .01f);
    jsP1_Gain.setValue(audioProcessor.Pedal_Gain);
    jsP1_Gain.addListener(this);
    addAndMakeVisible(jsP1_Gain);

    GUI_Init_Large_Slider(&jsP1_NGate);
    jsP1_NGate.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_NGate.setRange(0.0f, 1.0f, .01f);
    jsP1_NGate.setValue(audioProcessor.Pedal_NGate);
    jsP1_NGate.addListener(this);
    addAndMakeVisible(jsP1_NGate);

    GUI_Init_Large_Slider(&jsP1_MidFreq);
    jsP1_MidFreq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_MidFreq.setRange(100, 1200, 50);
    jsP1_MidFreq.setTextValueSuffix(" Hz");
    jsP1_MidFreq.setValue(audioProcessor.Pedal_MidFreq);
    jsP1_MidFreq.addListener(this);
    addAndMakeVisible(jsP1_MidFreq);

    GUI_Init_Large_Slider(&jsP1_MidQ);
    jsP1_MidQ.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_MidQ.setRange(.01f, 1.0f, .01f);
    jsP1_MidQ.setValue(audioProcessor.Pedal_MidQ);
    jsP1_MidQ.addListener(this);
    addAndMakeVisible(jsP1_MidQ);

    GUI_Init_Large_Slider(&jsP1_Drive);
    jsP1_Drive.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Drive.setRange(0.0f, 1.0f, .01f);
    jsP1_Drive.setValue(audioProcessor.Pedal_Drive);
    jsP1_Drive.addListener(this);
    addAndMakeVisible(jsP1_Drive);

    GUI_Init_Large_Slider(&jsP1_LowPass);
    jsP1_LowPass.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_LowPass.setTextValueSuffix(" Hz");
    jsP1_LowPass.setRange(2000, 8000, 100);
    jsP1_LowPass.setValue(audioProcessor.Pedal_LowPass);
    jsP1_LowPass.addListener(this);
    addAndMakeVisible(jsP1_LowPass);

    GUI_Init_Large_Slider(&jsP1_HighPass);
    jsP1_HighPass.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_HighPass.setTextValueSuffix(" Hz");
    jsP1_HighPass.setRange(50, 250, 10);
    jsP1_HighPass.setValue(audioProcessor.Pedal_HighPass);
    jsP1_HighPass.addListener(this);
    addAndMakeVisible(jsP1_HighPass);

    GUI_Init_Large_Slider(&jsP1_EnhLow);
    jsP1_EnhLow.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_EnhLow.setRange(0.0f, 1.0f, .01f);
    jsP1_EnhLow.setValue(audioProcessor.Pedal_EnhLow);
    jsP1_EnhLow.addListener(this);
    addAndMakeVisible(jsP1_EnhLow);

    GUI_Init_Large_Slider(&jsP1_EnhLowFreq);
    jsP1_EnhLowFreq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_EnhLowFreq.setRange(100, 1200, 50);
    jsP1_EnhLowFreq.setTextValueSuffix(" Hz");
    jsP1_EnhLowFreq.setValue(audioProcessor.Pedal_EnhLowFreq);
    jsP1_EnhLowFreq.addListener(this);
    addAndMakeVisible(jsP1_EnhLowFreq);

    GUI_Init_Large_Slider(&jsP1_EnhHigh);
    jsP1_EnhHigh.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_EnhHigh.setRange(0.0f, 1.0f, .01f);
    jsP1_EnhHigh.setValue(audioProcessor.Pedal_EnhHigh);
    jsP1_EnhHigh.addListener(this);
    addAndMakeVisible(jsP1_EnhHigh);

    GUI_Init_Large_Slider(&jsP1_EnhHighFreq);
    jsP1_EnhHighFreq.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_EnhHighFreq.setRange(500, 3000, 50);
    jsP1_EnhHighFreq.setTextValueSuffix(" Hz");
    jsP1_EnhHighFreq.setValue(audioProcessor.Pedal_EnhHighFreq);
    jsP1_EnhHighFreq.addListener(this);
    addAndMakeVisible(jsP1_EnhHighFreq);

    GUI_Init_Large_Slider(&jsP1_Thump);
    jsP1_Thump.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Thump.setRange(0.0, 1.0, .01);
    jsP1_Thump.setValue(audioProcessor.Pedal_Thump);
    jsP1_Thump.addListener(this);
    addAndMakeVisible(jsP1_Thump);

    GUI_Init_Large_Slider(&jsP1_Band);
    jsP1_Band.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band.setRange(0, 10, 1);
    jsP1_Band.setValue(audioProcessor.Pedal_Band);
    jsP1_Band.addListener(this);
    addAndMakeVisible(jsP1_Band);

    GUI_Init_Large_Slider(&jsP1_Band1);
    jsP1_Band1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band1.setRange(-12.0f, 12.0f, .1f);
    jsP1_Band1.setValue(audioProcessor.Pedal_Band1);
    jsP1_Band1.addListener(this);
    addAndMakeVisible(jsP1_Band1);

    GUI_Init_Large_Slider(&jsP1_Band2);
    jsP1_Band2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band2.setRange(-12.0f, 12.0f, .1f);
    jsP1_Band2.setValue(audioProcessor.Pedal_Band2);
    jsP1_Band2.addListener(this);
    addAndMakeVisible(jsP1_Band2);

    GUI_Init_Large_Slider(&jsP1_Band3);
    jsP1_Band3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band3.setRange(-12.0f, 12.0f, .1f);
    jsP1_Band3.setValue(audioProcessor.Pedal_Band3);
    jsP1_Band3.addListener(this);
    addAndMakeVisible(jsP1_Band3);

    GUI_Init_Large_Slider(&jsP1_Band4);
    jsP1_Band4.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band4.setRange(-12.0f, 12.0f, .1f);
    jsP1_Band4.setValue(audioProcessor.Pedal_Band4);
    jsP1_Band4.addListener(this);
    addAndMakeVisible(jsP1_Band4);

    GUI_Init_Large_Slider(&jsP1_Band5);
    jsP1_Band5.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    jsP1_Band5.setRange(-12.0f, 12.0f, .1f);
    jsP1_Band5.setValue(audioProcessor.Pedal_Band5);
    jsP1_Band5.addListener(this);
    addAndMakeVisible(jsP1_Band5);

    GUI_Init_Small_Slider(&jsP1_IR_Model);
    jsP1_IR_Model.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    jsP1_IR_Model.setRange(1, 5, 1);
    jsP1_IR_Model.setValue(audioProcessor.Pedal_IR_Model);
    jsP1_IR_Model.addListener(this);
    addAndMakeVisible(jsP1_IR_Model);

    GUI_Init_Small_Slider(&jsP1_IR_On);
    jsP1_IR_On.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    jsP1_IR_On.setRange(0, 1, 1);
    jsP1_IR_On.setValue(audioProcessor.Pedal_IR_On);
    jsP1_IR_On.addListener(this);
    addAndMakeVisible(jsP1_IR_On);

    GUI_Init_Small_Slider(&jsP1_ClipMode);
    jsP1_ClipMode.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    jsP1_ClipMode.setRange(0, 1, 1);
    jsP1_ClipMode.setValue(audioProcessor.Pedal_ClipMode);
    jsP1_ClipMode.addListener(this);
    addAndMakeVisible(jsP1_ClipMode);

    GUI_Init_Small_Slider(&jsP1_Mono);
    jsP1_Mono.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    jsP1_Mono.setRange(0, 1, 1);
    jsP1_Mono.setValue(audioProcessor.Pedal_Mono);
    jsP1_Mono.addListener(this);
    addAndMakeVisible(jsP1_Mono);

    GUI_Init_Small_Slider(&jsP1_Thin);
    jsP1_Thin.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    jsP1_Thin.setRange(0.0f, 1.0f, .1f);
    jsP1_Thin.setValue(audioProcessor.Pedal_Thin);
    jsP1_Thin.addListener(this);
    addAndMakeVisible(jsP1_Thin);

    GUI_Init_Small_Slider(&jsP1_CompThresh);
    jsP1_CompThresh.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    jsP1_CompThresh.setRange(0.0f, 1.0f, .1f);
    jsP1_CompThresh.setValue(audioProcessor.Pedal_CompThresh);
    jsP1_CompThresh.addListener(this);
    addAndMakeVisible(jsP1_CompThresh);

    //R1.00 Update the Look and Feel (Global colors) so drop down menu is the correct color. 
    getLookAndFeel().setColour(juce::DocumentWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::DocumentWindow::textColourId, juce::Colour(255, 255, 255));
    getLookAndFeel().setColour(juce::DialogWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(192, 0, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonOnColourId, juce::Colour(192, 0, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ListBox::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::Label::backgroundColourId, juce::Colour(32, 32, 32));

    //R1.00 Define our control positions to make drawing easier.
    KNOB_DefinePosition(0,   0, 10, 70, 80, "Gain"); 
    KNOB_DefinePosition(1,  75, 10, 70, 80, "Gate");
    KNOB_DefinePosition(2, 150, 10, 70, 80, "Mid F");
    KNOB_DefinePosition(3, 225, 10, 70, 80, "Mid Push");
    KNOB_DefinePosition(4, 300, 10, 70, 80, "Drive");
    KNOB_DefinePosition(5, 375, 10, 70, 80, "Fizz"); 
        
    KNOB_DefinePosition( 6,  0, 110, 70, 80, "Mud"); 
    KNOB_DefinePosition( 7, 75, 110, 70, 80, "Enh Lo");  
    KNOB_DefinePosition( 8, 150, 110, 70, 80, "Enh F");
    KNOB_DefinePosition( 9, 225, 110, 70, 80, "Enh Hi");
    KNOB_DefinePosition(10, 300, 110, 70, 80, "Enh F");
    KNOB_DefinePosition(11, 375, 110, 70, 80, "Thump");
    
    KNOB_DefinePosition(12,   0, 210, 70, 80, "Band");
    KNOB_DefinePosition(13,  75, 210, 70, 80, UIS_Band1);
    KNOB_DefinePosition(14, 150, 210, 70, 80, UIS_Band2);
    KNOB_DefinePosition(15, 225, 210, 70, 80, UIS_Band3);
    KNOB_DefinePosition(16, 300, 210, 70, 80, UIS_Band4);
    KNOB_DefinePosition(17, 375, 210, 70, 80, UIS_Band5);

    //R1.00 Update our filter text strings.
    Band_SetFilterValues(false);

    //R1.00 HELP Text!
    labHelp.setText("", juce::dontSendNotification);
    labHelp.setJustificationType(juce::Justification::centred);
    labHelp.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF101010));
    labHelp.setColour(juce::Label::textColourId, juce::Colour(0xFFE0E0E0));
    labHelp.setColour(juce::Label::outlineColourId, juce::Colour(0xFF800000));
    addAndMakeVisible(labHelp);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //R1.00 Set the window size.
    setSize(570, 320);
}


MakoBiteAudioProcessorEditor::~MakoBiteAudioProcessorEditor()
{
}


//==============================================================================
void MakoBiteAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    bool UseImage = true;
    juce::ColourGradient ColGrad;

    if (UseImage)
    {
        g.drawImageAt(imgBackground, 0, 0);        
    }
    else
    {
        //R1.00 Draw our GUI.
        //R1.00 Background.
        ColGrad = juce::ColourGradient(juce::Colour(0xFFAD8748), 0.0f, 0.0f, juce::Colour(0xFFE1B761), 0.0f, 140.0f, false);
        g.setGradientFill(ColGrad);
        g.fillRect(0, 0, 580, 141);
        ColGrad = juce::ColourGradient(juce::Colour(0xFFE1B761), 0.0f, 140.0f, juce::Colour(0xFFAD8748), 0.0f, 320.0f, false);
        g.setGradientFill(ColGrad);
        g.fillRect(0, 140, 580, 180);

        //R1.00 Draw LOGO text.
        g.setColour(juce::Colours::black);
        g.fillRect(450, 5, 110, 30);
        g.setFont(16.0f);
        g.setColour(juce::Colours::white);
        g.drawFittedText("S T A C K E D", 455, 5, 100, 18, juce::Justification::centred, 1);
        g.setFont(12.0f);
        g.setColour(juce::Colours::red);
        g.drawFittedText("M * A * K * O", 455, 20, 100, 15, juce::Justification::centred, 1);
        
        //R1.00 Draw a KNOB inside the slider.
        ColGrad = juce::ColourGradient(juce::Colour(0xFFF0F0E0), 0.0f, Knob_Pos[0].y, juce::Colour(0xFFA0A0A0), 0.0f, Knob_Pos[0].y + 48, false);
        g.setGradientFill(ColGrad);
        for (int t = 0; t < 6; t++)
        {
            g.fillEllipse(Knob_Pos[t].x + 11, Knob_Pos[t].y + 6, 48, 48);
        }
        g.setColour(juce::Colour(0xFF202020));
        for (int t = 0; t < 6; t++)
        {
            g.drawEllipse(Knob_Pos[t].x + 10, Knob_Pos[t].y + 5, 50, 50, 2.0f);
        }

        ColGrad = juce::ColourGradient(juce::Colour(0xFFF0F0E0), 0.0f, Knob_Pos[6].y, juce::Colour(0xFFA0A0A0), 0.0f, Knob_Pos[6].y + 48, false);
        g.setGradientFill(ColGrad);
        for (int t = 6; t < 12; t++)
        {
            g.fillEllipse(Knob_Pos[t].x + 11, Knob_Pos[t].y + 6, 48, 48);
        }
        g.setColour(juce::Colour(0xFF202020));
        for (int t = 6; t < 12; t++)
        {
            g.drawEllipse(Knob_Pos[t].x + 10, Knob_Pos[t].y + 5, 50, 50, 2.0f);
        }

        ColGrad = juce::ColourGradient(juce::Colour(0xFFF0F0E0), 0.0f, Knob_Pos[12].y, juce::Colour(0xFFA0A0A0), 0.0f, Knob_Pos[12].y + 48, false);
        g.setGradientFill(ColGrad);
        for (int t = 12; t < 18; t++)
        {
            g.fillEllipse(Knob_Pos[t].x + 11, Knob_Pos[t].y + 6, 48, 48);
        }
        g.setColour(juce::Colour(0xFF202020));
        for (int t = 12; t < 18; t++)
        {
            g.drawEllipse(Knob_Pos[t].x + 10, Knob_Pos[t].y + 5, 50, 50, 2.0f);
        }
    }
    
    //R1.00 DRAW TEXT.
    //R1.00 Most of these could be done on the image to speed up painting.
    //R1.00 But the EQ frequencies need to update. So we are doing all the text
    //R1.00 here so the font matches no matter what in the future.
    g.setFont(12.0f);
    g.setColour(juce::Colours::black);
    for (int t = 0; t < 13; t++)
    {
        g.drawFittedText(Knob_Name[t], Knob_Pos[t].x, Knob_Pos[t].y - 10, Knob_Pos[t].sizex, 15, juce::Justification::centred, 1);
    }

    //R1.00 Draw the EQ frequency headers.
    g.drawFittedText(UIS_Band1, Knob_Pos[13].x, Knob_Pos[13].y - 10, Knob_Pos[13].sizex, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band2, Knob_Pos[14].x, Knob_Pos[14].y - 10, Knob_Pos[14].sizex, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band3, Knob_Pos[15].x, Knob_Pos[15].y - 10, Knob_Pos[15].sizex, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band4, Knob_Pos[16].x, Knob_Pos[16].y - 10, Knob_Pos[16].sizex, 15, juce::Justification::centred, 1);
    g.drawFittedText(UIS_Band5, Knob_Pos[17].x, Knob_Pos[17].y - 10, Knob_Pos[17].sizex, 15, juce::Justification::centred, 1);

    //1.00 Draw the Option slider headers.
    g.drawFittedText("Amp Presets", 455, 55, 100, 15, juce::Justification::centred, 1);
    g.drawFittedText("IR Cab " + std::to_string(audioProcessor.Pedal_IR_Model), 455, 100, 100, 15, juce::Justification::centred, 1);
    g.drawFittedText("off  CAB  on", 455, 130, 100, 15, juce::Justification::centred, 1);
    g.drawFittedText("s  CLIP  h", 455, 160, 100, 15, juce::Justification::centred, 1);
    g.drawFittedText("Stereo / Mono", 455, 190, 100, 15, juce::Justification::centred, 1);
    g.drawFittedText("Tube Thinner", 455, 220, 100, 15, juce::Justification::centred, 1);
    g.drawFittedText("Compression", 455, 250, 100, 15, juce::Justification::centred, 1);
}

void MakoBiteAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //R1.00 Use RETURN here to have a blank window drawn when creating a GUI image.
    //R1.00 The controls are only visible if thier BOUNDS are defined.
    //return;

    jsP1_Gain.setBounds     (Knob_Pos[0].x, Knob_Pos[0].y, Knob_Pos[0].sizex, Knob_Pos[0].sizey);
    jsP1_NGate.setBounds    (Knob_Pos[1].x, Knob_Pos[1].y, Knob_Pos[1].sizex, Knob_Pos[1].sizey);
    jsP1_MidFreq.setBounds  (Knob_Pos[2].x, Knob_Pos[2].y, Knob_Pos[2].sizex, Knob_Pos[2].sizey);
    jsP1_MidQ.setBounds     (Knob_Pos[3].x, Knob_Pos[3].y, Knob_Pos[3].sizex, Knob_Pos[3].sizey);
    jsP1_Drive.setBounds    (Knob_Pos[4].x, Knob_Pos[4].y, Knob_Pos[4].sizex, Knob_Pos[4].sizey);
    jsP1_LowPass.setBounds  (Knob_Pos[5].x, Knob_Pos[5].y, Knob_Pos[5].sizex, Knob_Pos[5].sizey);
    
    jsP1_HighPass.setBounds   (Knob_Pos[6].x, Knob_Pos[6].y, Knob_Pos[6].sizex, Knob_Pos[6].sizey);
    jsP1_EnhLow.setBounds     (Knob_Pos[7].x, Knob_Pos[7].y, Knob_Pos[7].sizex, Knob_Pos[7].sizey);
    jsP1_EnhLowFreq.setBounds (Knob_Pos[8].x, Knob_Pos[8].y, Knob_Pos[8].sizex, Knob_Pos[8].sizey);
    jsP1_EnhHigh.setBounds    (Knob_Pos[9].x, Knob_Pos[9].y, Knob_Pos[9].sizex, Knob_Pos[9].sizey);
    jsP1_EnhHighFreq.setBounds(Knob_Pos[10].x, Knob_Pos[10].y, Knob_Pos[10].sizex, Knob_Pos[10].sizey);

    jsP1_Thump.setBounds(Knob_Pos[11].x, Knob_Pos[11].y, Knob_Pos[11].sizex, Knob_Pos[11].sizey);
    jsP1_Band.setBounds(Knob_Pos[12].x, Knob_Pos[12].y, Knob_Pos[12].sizex, Knob_Pos[12].sizey);
    jsP1_Band1.setBounds(Knob_Pos[13].x, Knob_Pos[13].y, Knob_Pos[13].sizex, Knob_Pos[13].sizey);
    jsP1_Band2.setBounds(Knob_Pos[14].x, Knob_Pos[14].y, Knob_Pos[14].sizex, Knob_Pos[14].sizey);
    jsP1_Band3.setBounds(Knob_Pos[15].x, Knob_Pos[15].y, Knob_Pos[15].sizex, Knob_Pos[15].sizey);
    jsP1_Band4.setBounds(Knob_Pos[16].x, Knob_Pos[16].y, Knob_Pos[16].sizex, Knob_Pos[16].sizey);
    jsP1_Band5.setBounds(Knob_Pos[17].x, Knob_Pos[17].y, Knob_Pos[17].sizex, Knob_Pos[17].sizey);

    //R1.00 Preset Dropdown List and Switches.
    cbPreset.setBounds (455, 70, 100, 20);    
    jsP1_IR_Model.setBounds(460, 105, 90, 30);
    jsP1_IR_On.setBounds(480, 135, 50, 30);
    jsP1_ClipMode.setBounds(480, 165, 50, 30);
    jsP1_Mono.setBounds (480, 195, 50, 30);
    jsP1_Thin.setBounds (460, 225, 90, 30);
    jsP1_CompThresh.setBounds(460, 255, 90, 30);

    //R1.00 Help.
    labHelp.setBounds(5, 298, 436, 18);
}

void MakoBiteAudioProcessorEditor::cbPresetChanged()
{
    //R1.00 User selected one of our premade sounds.
    int Sel = cbPreset.getSelectedId() - 1;
    
    //R1.00 We are changing settings in code, dont flag the proc to update until done.
    //R1.00 This stops every slider change from trying to update the proc.
    //R1.00 The UPDATE ALL (100) will happen at the end.
    if (Sel) audioProcessor.SettingsType = 100;

    //R1.00 Get our new settings. Slider Changes occur and vars get set in SliderValuesChanged.
    //R1.00 But the filters are not recalc'd because SettingsType = 100. Change them last.
    switch (Sel)
    {
        case 1:  Preset00(); break;   //R1.00 Southern.
        case 2:  Preset01(); break;   //R1.00 Voltage.
        case 3:  Preset02(); break;   //R1.00 Hard Cafe.
        case 4:  Preset03(); break;   //R1.00 Zinc.
        case 5:  Preset04(); break;   //R1.00 Mad Man.
        case 6:  Preset05(); break;   //R1.00 Mad Man.
    }
    
    //R1.00 All settings are changed, flag the processor it needs to update.
    if (Sel) audioProcessor.SettingsChanged = true;

}

void MakoBiteAudioProcessorEditor::cbPreset_UpdateSelection(int button, int idx, int editmode)
{
    
}

//R1.00 SOUTHERN.
void MakoBiteAudioProcessorEditor::Preset00()
{
    jsP1_Gain.setValue(.78f, juce::sendNotification);

    jsP1_MidFreq.setValue(650, juce::sendNotification);
    jsP1_MidQ.setValue(.4f, juce::sendNotification);
    jsP1_Drive.setValue(.24f, juce::sendNotification);
    jsP1_LowPass.setValue(3400, juce::sendNotification);

    jsP1_HighPass.setValue(110, juce::sendNotification);
    jsP1_EnhLow.setValue(0.0f, juce::sendNotification);
    jsP1_EnhLowFreq.setValue(450, juce::sendNotification);
    jsP1_EnhHigh.setValue(.2f, juce::sendNotification);
    jsP1_EnhHighFreq.setValue(1200.0f, juce::sendNotification);
    jsP1_Thump.setValue(.12f, juce::sendNotification);

    jsP1_Band.setValue(7, juce::sendNotification);
    jsP1_Band1.setValue(-1.6f, juce::sendNotification);
    jsP1_Band2.setValue(0.0f, juce::sendNotification);
    jsP1_Band3.setValue(4.0f, juce::sendNotification);
    jsP1_Band4.setValue(-.8f, juce::sendNotification);
    jsP1_Band5.setValue(1.6f, juce::sendNotification);

    jsP1_IR_Model.setValue(1, juce::sendNotification);
    jsP1_ClipMode.setValue(0, juce::sendNotification);
    jsP1_Thin.setValue(0.0f, juce::sendNotification);
    jsP1_CompThresh.setValue(1.0f, juce::sendNotification);
}

//R1.00 VOLTAGE
void MakoBiteAudioProcessorEditor::Preset01()
{
    jsP1_Gain.setValue(.78f, juce::sendNotification);

    jsP1_MidFreq.setValue(700, juce::sendNotification);
    jsP1_MidQ.setValue(.5f, juce::sendNotification);
    jsP1_Drive.setValue(.2f, juce::sendNotification);
    jsP1_LowPass.setValue(5000, juce::sendNotification);

    jsP1_HighPass.setValue(50, juce::sendNotification);
    jsP1_EnhLow.setValue(0.0f, juce::sendNotification);
    jsP1_EnhLowFreq.setValue(450, juce::sendNotification);
    jsP1_EnhHigh.setValue(.18f, juce::sendNotification);
    jsP1_EnhHighFreq.setValue(1200.0f, juce::sendNotification);
    jsP1_Thump.setValue(.2f, juce::sendNotification);

    jsP1_Band.setValue(1, juce::sendNotification);
    jsP1_Band1.setValue(-1.6f, juce::sendNotification);
    jsP1_Band2.setValue(1.6f, juce::sendNotification);
    jsP1_Band3.setValue(0.0f, juce::sendNotification);
    jsP1_Band4.setValue(1.5f, juce::sendNotification);
    jsP1_Band5.setValue(0.0f, juce::sendNotification);

    jsP1_IR_Model.setValue(2, juce::sendNotification);
    jsP1_ClipMode.setValue(0, juce::sendNotification);
    jsP1_Thin.setValue(0.0f, juce::sendNotification);
    jsP1_CompThresh.setValue(.4f, juce::sendNotification);
    
}

//R1.00 HARD CAFE
void MakoBiteAudioProcessorEditor::Preset02()
{
    jsP1_Gain.setValue(.77f, juce::sendNotification);

    jsP1_MidFreq.setValue(800, juce::sendNotification);
    jsP1_MidQ.setValue(.56f, juce::sendNotification);
    jsP1_Drive.setValue(.32f, juce::sendNotification);
    jsP1_LowPass.setValue(4400, juce::sendNotification);

    jsP1_HighPass.setValue(120, juce::sendNotification);
    jsP1_EnhLow.setValue(0.2f, juce::sendNotification);
    jsP1_EnhLowFreq.setValue(500, juce::sendNotification);
    jsP1_EnhHigh.setValue(.32f, juce::sendNotification);
    jsP1_EnhHighFreq.setValue(1200.0f, juce::sendNotification);
    jsP1_Thump.setValue(.32f, juce::sendNotification);

    jsP1_Band.setValue(1, juce::sendNotification);
    jsP1_Band1.setValue(-2.4f, juce::sendNotification);
    jsP1_Band2.setValue(0.0f, juce::sendNotification);
    jsP1_Band3.setValue(0.0f, juce::sendNotification);
    jsP1_Band4.setValue(-1.6f, juce::sendNotification);
    jsP1_Band5.setValue(1.6f, juce::sendNotification);

    jsP1_IR_Model.setValue(3, juce::sendNotification);
    jsP1_ClipMode.setValue(1, juce::sendNotification);
    jsP1_Thin.setValue(0.0f, juce::sendNotification);
    jsP1_CompThresh.setValue(1.0f, juce::sendNotification);
}

//R1.00 ZINC
void MakoBiteAudioProcessorEditor::Preset03()
{
    jsP1_Gain.setValue(.64f, juce::sendNotification);

    jsP1_MidFreq.setValue(900, juce::sendNotification);
    jsP1_MidQ.setValue(.74f, juce::sendNotification);
    jsP1_Drive.setValue(.32f, juce::sendNotification);
    jsP1_LowPass.setValue(4600, juce::sendNotification);

    jsP1_HighPass.setValue(140, juce::sendNotification);
    jsP1_EnhLow.setValue(0.36f, juce::sendNotification);
    jsP1_EnhLowFreq.setValue(350, juce::sendNotification);
    jsP1_EnhHigh.setValue(.36f, juce::sendNotification);
    jsP1_EnhHighFreq.setValue(900.0f, juce::sendNotification);
    jsP1_Thump.setValue(.36f, juce::sendNotification);

    jsP1_Band.setValue(6, juce::sendNotification);
    jsP1_Band1.setValue(0.0f, juce::sendNotification);
    jsP1_Band2.setValue(3.2f, juce::sendNotification);
    jsP1_Band3.setValue(0.0f, juce::sendNotification);
    jsP1_Band4.setValue(-3.2f, juce::sendNotification);
    jsP1_Band5.setValue(1.6f, juce::sendNotification);

    jsP1_IR_Model.setValue(4, juce::sendNotification);
    jsP1_ClipMode.setValue(1, juce::sendNotification);
    jsP1_Thin.setValue(0.0f, juce::sendNotification);
    jsP1_CompThresh.setValue(.4f, juce::sendNotification);
}

//R1.00 MAD MAN
void MakoBiteAudioProcessorEditor::Preset04()
{
    jsP1_Gain.setValue(.66f, juce::sendNotification);

    jsP1_MidFreq.setValue(700, juce::sendNotification);
    jsP1_MidQ.setValue(.74f, juce::sendNotification);
    jsP1_Drive.setValue(.25f, juce::sendNotification);
    jsP1_LowPass.setValue(4400, juce::sendNotification);

    jsP1_HighPass.setValue(120, juce::sendNotification);
    jsP1_EnhLow.setValue(0.4f, juce::sendNotification);
    jsP1_EnhLowFreq.setValue(450, juce::sendNotification);
    jsP1_EnhHigh.setValue(.5f, juce::sendNotification);
    jsP1_EnhHighFreq.setValue(1200.0f, juce::sendNotification);
    jsP1_Thump.setValue(.24f, juce::sendNotification);

    jsP1_Band.setValue(5, juce::sendNotification);
    jsP1_Band1.setValue(-2.0f, juce::sendNotification);
    jsP1_Band2.setValue(2.0f, juce::sendNotification);
    jsP1_Band3.setValue(-1.6f, juce::sendNotification);
    jsP1_Band4.setValue(-0.8f, juce::sendNotification);
    jsP1_Band5.setValue(0.0f, juce::sendNotification);

    jsP1_IR_Model.setValue(5, juce::sendNotification);
    jsP1_ClipMode.setValue(1, juce::sendNotification);
    jsP1_Thin.setValue(0.0f, juce::sendNotification);
    jsP1_CompThresh.setValue(1.0f, juce::sendNotification);
}

//R1.00 THE FEELS
void MakoBiteAudioProcessorEditor::Preset05()
{
    jsP1_Gain.setValue(1.0f, juce::sendNotification);

    jsP1_MidFreq.setValue(700, juce::sendNotification);
    jsP1_MidQ.setValue(.25f, juce::sendNotification);
    jsP1_Drive.setValue(.02f, juce::sendNotification);
    jsP1_LowPass.setValue(3000, juce::sendNotification);

    jsP1_HighPass.setValue(50, juce::sendNotification);
    jsP1_EnhLow.setValue(0.4f, juce::sendNotification);
    jsP1_EnhLowFreq.setValue(700, juce::sendNotification);
    jsP1_EnhHigh.setValue(.28f, juce::sendNotification);
    jsP1_EnhHighFreq.setValue(1500.0f, juce::sendNotification);
    jsP1_Thump.setValue(.0f, juce::sendNotification);

    jsP1_Band.setValue(4, juce::sendNotification);
    jsP1_Band1.setValue(.0f, juce::sendNotification);
    jsP1_Band2.setValue(.0f, juce::sendNotification);
    jsP1_Band3.setValue(.0f, juce::sendNotification);
    jsP1_Band4.setValue(2.4f, juce::sendNotification);
    jsP1_Band5.setValue(.0f, juce::sendNotification);

    jsP1_IR_Model.setValue(3, juce::sendNotification);
    jsP1_ClipMode.setValue(0, juce::sendNotification);
    jsP1_CompThresh.setValue(1.0f, juce::sendNotification);
}

void MakoBiteAudioProcessorEditor::GUI_Init_Large_Slider(juce::Slider* slider)
{
    //R1.00 Setting the LOOK and FEEL overrides the default JUCE drawing functions.
    //R1.00 So everything after this only works if LNF is not set.
    //R1.00 We have made our own SLIDER draw functions, so point the slider to our funcs.
    slider->setLookAndFeel(&myLookAndFeel);

    //R1.00 Set the look for the default JUCE slider control.
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFC0C0C0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF600000));   
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(0xFF000000));
}

void MakoBiteAudioProcessorEditor::GUI_Init_Small_Slider(juce::Slider* slider)
{
    //R1.00 Set the look for the default JUCE slider control.
    slider->setSliderStyle(juce::Slider::LinearHorizontal);
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::trackColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(0xFFF0F0F0));
}

void MakoBiteAudioProcessorEditor::KNOB_DefinePosition(int idx,float x, float y, float sizex, float sizey, juce::String name)
{
    Knob_Pos[idx].x = x;
    Knob_Pos[idx].y = y;
    Knob_Pos[idx].sizex = sizex;
    Knob_Pos[idx].sizey = sizey;
    Knob_Name[idx] = name;
}

/*
void MakoBiteAudioProcessorEditor::updateToggleState(juce::Button* button, juce::String name)
{
    auto state = button->getToggleState();
    bool Butt_On = state ? true : false;

}
*/

void MakoBiteAudioProcessorEditor::Setting_UpdateProcessor(int SettingType)
{
    //R1.00 Set flags here that let the PROCESSOR know it needs to update settings.
    //R1.00 If we are changing settings in CODE, we dont want to update the proc until all done changing.
    if (audioProcessor.SettingsType != 100)
    {
        audioProcessor.SettingsType = SettingType;
        audioProcessor.SettingsChanged = true;  //R1.00 We need to update settings in processor.
    }
}


void MakoBiteAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{   
    //R1.00 When a slider is adjusted, this func gets called. Capture the new edits and flag
    //R1.00 the processor when it needs to recalc things.

    //R1.00 Vol / Gain
    if (slider == &jsP1_Gain)
    {        
        labHelp.setText("Adjust the final output volume.", juce::dontSendNotification);
        audioProcessor.Pedal_Gain = float(jsP1_Gain.getValue());
        return;
    }

    //R1.00 High Pass.
    if (slider == &jsP1_HighPass)
    {
        labHelp.setText("Reduce input bass to add tightness (50=off).", juce::dontSendNotification);
        audioProcessor.Pedal_HighPass = int(jsP1_HighPass.getValue());
        Setting_UpdateProcessor(1);
        return;
    }

    //R1.00 Low Pass
    if (slider == &jsP1_LowPass)
    {
        labHelp.setText("Use to reduce the fizz of the distortion (8k=off).", juce::dontSendNotification);
        audioProcessor.Pedal_LowPass = int(jsP1_LowPass.getValue());
        Setting_UpdateProcessor(1);
        return;
    }

    //R1.00  Mid Range Voice Filter Frequency.
    if (slider == &jsP1_MidFreq)
    {        
        labHelp.setText("Select the distortion frequency/voice.", juce::dontSendNotification);
        audioProcessor.Pedal_MidFreq = int(jsP1_MidFreq.getValue());
        Setting_UpdateProcessor(1);
        return;
    }

    //R1.00 Mid Range Voice Filter Q.
    if (slider == &jsP1_MidQ)
    {        
        labHelp.setText("Adjust the wah sound of the distortion.", juce::dontSendNotification);
        audioProcessor.Pedal_MidQ = float(jsP1_MidQ.getValue());
        Setting_UpdateProcessor(1);
        return;
    }

    //R1.00 Drive
    if (slider == &jsP1_Drive)
    {        
        labHelp.setText("Adjust the amount of distortion.", juce::dontSendNotification);
        audioProcessor.Pedal_Drive = float(jsP1_Drive.getValue());
        return;
    }

    //R1.00 Enhancer Low Drive. Post Distortion Phase.
    if (slider == &jsP1_EnhLow)
    {        
        labHelp.setText("Add some thickness after distortion.", juce::dontSendNotification);
        audioProcessor.Pedal_EnhLow = float(jsP1_EnhLow.getValue());
        Setting_UpdateProcessor(2);
        return;
    }

    //R1.00 Enhancer Low Drive frequency.
    if (slider == &jsP1_EnhLowFreq)
    {
        labHelp.setText("Select the thickness frequency/voice.", juce::dontSendNotification);
        audioProcessor.Pedal_EnhLowFreq = int(jsP1_EnhLowFreq.getValue());
        Setting_UpdateProcessor(2);
        return;
    }

    //R1.00 Enhancer Low Drive. Pre Distortion Phase.
    if (slider == &jsP1_EnhHigh)
    {        
        labHelp.setText("Add some brightness before distortion to restore attack.", juce::dontSendNotification);
        audioProcessor.Pedal_EnhHigh = float(jsP1_EnhHigh.getValue());
        Setting_UpdateProcessor(2);
        return;
    }
    //R1.00 Enhancer Low Drive frequency.
    if (slider == &jsP1_EnhHighFreq)
    {        
        labHelp.setText("Select the attack frequency/voice.", juce::dontSendNotification);
        audioProcessor.Pedal_EnhHighFreq = int(jsP1_EnhHighFreq.getValue());
        Setting_UpdateProcessor(2);
        return;
    }

    //R1.00 Thump
    if (slider == &jsP1_Thump)
    {
        labHelp.setText("Add enhanced lows to simulate speaker/cab thump.", juce::dontSendNotification);
        audioProcessor.Pedal_Thump = float(jsP1_Thump.getValue());
        Setting_UpdateProcessor(2);
    }

    //R1.00 Band
    if (slider == &jsP1_Band)
    {
        labHelp.setText("Select a preset EQ setup.", juce::dontSendNotification);
        audioProcessor.Pedal_Band = int(jsP1_Band.getValue());
        Band_SetFilterValues(true);
        Setting_UpdateProcessor(3);
    }

    //R1.00 Band 1
    if (slider == &jsP1_Band1)
    {
        labHelp.setText("Adjust the EQ.", juce::dontSendNotification);
        audioProcessor.Pedal_Band1 = float(jsP1_Band1.getValue());
        Setting_UpdateProcessor(3);
    }

    //R1.00 Band 2
    if (slider == &jsP1_Band2)
    {
        labHelp.setText("Adjust the EQ.", juce::dontSendNotification);
        audioProcessor.Pedal_Band2 = float(jsP1_Band2.getValue());
        Setting_UpdateProcessor(3);
    }

    //R1.00 Band 3
    if (slider == &jsP1_Band3)
    {
        labHelp.setText("Adjust the EQ.", juce::dontSendNotification);
        audioProcessor.Pedal_Band3 = float(jsP1_Band3.getValue());
        Setting_UpdateProcessor(3);
    }

    //R1.00 Band 4
    if (slider == &jsP1_Band4)
    {
        labHelp.setText("Adjust the EQ.", juce::dontSendNotification);
        audioProcessor.Pedal_Band4 = float(jsP1_Band4.getValue());
        Setting_UpdateProcessor(3);
    }

    //R1.00 Band 5
    if (slider == &jsP1_Band5)
    {
        labHelp.setText("Adjust the EQ.", juce::dontSendNotification);
        audioProcessor.Pedal_Band5 = float(jsP1_Band5.getValue());
        Setting_UpdateProcessor(3);
    }

    //R1.00 Noise Gate.
    if (slider == &jsP1_NGate)
    {
        labHelp.setText("Adjust the noise gate to reduce hiss when not playing.", juce::dontSendNotification);
        audioProcessor.Pedal_NGate = float(jsP1_NGate.getValue());
        return;
    }

    //R1.00 IR Model.
    if (slider == &jsP1_IR_Model)
    {
        labHelp.setText("Select one of five speaker cabinets (IRs).", juce::dontSendNotification);
        audioProcessor.Pedal_IR_Model = float(jsP1_IR_Model.getValue());
        Setting_UpdateProcessor(4);
        return;
    }

    //R1.00 IR CAB ON/OFF.
    if (slider == &jsP1_IR_On)
    {
        labHelp.setText("Toggle the speaker cabinet simulation on or off.", juce::dontSendNotification);
        audioProcessor.Pedal_IR_On = float(jsP1_IR_On.getValue());
        return;
    }
    
    //R1.00 CLIP MODE.
    if (slider == &jsP1_ClipMode)
    {
        labHelp.setText("Toggle soft or hard clipping.", juce::dontSendNotification);
        audioProcessor.Pedal_ClipMode = float(jsP1_ClipMode.getValue());
        return;
    }

    //R1.00 Stereo MONO.
    if (slider == &jsP1_Mono)
    {
        labHelp.setText("Toggle Stereo or Mono operation.", juce::dontSendNotification);
        audioProcessor.Pedal_Mono = float(jsP1_Mono.getValue());
        return;
    }

    //R1.00 Tub Thinner. Adds compression during loud playing toreduce unwanted ghost notes.
    if (slider == &jsP1_Thin)
    {
        labHelp.setText("Adjust the Tube Amp thinning. 0 = off.", juce::dontSendNotification);
        audioProcessor.Pedal_Thin = float(jsP1_Thin.getValue());
        return;
    }

    //R1.00 Compression.
    if (slider == &jsP1_CompThresh)
    {
        labHelp.setText("Set the compression threshold (before gain). 1 = off.", juce::dontSendNotification);
        audioProcessor.Pedal_CompThresh = float(jsP1_CompThresh.getValue());
        return;
    }

    return;
}

void MakoBiteAudioProcessorEditor::Band_SetFilterValues(bool ForcePaint)
{
    //R1.01 Define the user selected EQ mode.
    switch (audioProcessor.Pedal_Band)
    {
    default:
    {
        audioProcessor.Band1_Freq = 150.0f;
        audioProcessor.Band2_Freq = 300.0f;
        audioProcessor.Band3_Freq = 750.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 3000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 1:
    {
        audioProcessor.Band1_Freq = 150.0f;
        audioProcessor.Band2_Freq = 450.0f;
        audioProcessor.Band3_Freq = 900.0f;
        audioProcessor.Band4_Freq = 1800.0f;
        audioProcessor.Band5_Freq = 3500.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 2:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 220.0f;
        audioProcessor.Band3_Freq = 750.0f;
        audioProcessor.Band4_Freq = 2200.0f;
        audioProcessor.Band5_Freq = 6000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 3:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 350.0f;
        audioProcessor.Band3_Freq = 900.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 3000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 4:
    {
        audioProcessor.Band1_Freq = 100.0f;
        audioProcessor.Band2_Freq = 400.0f;
        audioProcessor.Band3_Freq = 800.0f;
        audioProcessor.Band4_Freq = 1600.0f;
        audioProcessor.Band5_Freq = 3200.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = 1.414f;
        break;
    }
    case 5:
    {
        audioProcessor.Band1_Freq = 120.0f;
        audioProcessor.Band2_Freq = 330.0f;
        audioProcessor.Band3_Freq = 660.0f;
        audioProcessor.Band4_Freq = 1320.0f;
        audioProcessor.Band5_Freq = 2500.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = .707f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .707f;
        break;
    }
    case 6:
    {
        audioProcessor.Band1_Freq = 150.0f;
        audioProcessor.Band2_Freq = 500.0f;
        audioProcessor.Band3_Freq = 900.0f;
        audioProcessor.Band4_Freq = 1800.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = 1.414f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .707f;
        break;
    }
    case 7:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 300.0f;
        audioProcessor.Band3_Freq = 650.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = 1.414f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 2.00f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .707f;
        break;
    }
    case 8:
    {
        audioProcessor.Band1_Freq = 100.0f;
        audioProcessor.Band2_Freq = 400.0f;
        audioProcessor.Band3_Freq = 800.0f;
        audioProcessor.Band4_Freq = 1500.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 1.414f;
        audioProcessor.Band4_Q = 2.00f;
        audioProcessor.Band5_Q = .35f;
        break;
    }
    case 9:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 500.0f;
        audioProcessor.Band3_Freq = 1000.0f;
        audioProcessor.Band4_Freq = 2000.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = .707f;
        audioProcessor.Band2_Q = 1.414f;
        audioProcessor.Band3_Q = .707f;
        audioProcessor.Band4_Q = .707f;
        audioProcessor.Band5_Q = .350f;
        break;
    }
    case 10:
    {
        audioProcessor.Band1_Freq = 80.0f;
        audioProcessor.Band2_Freq = 250.0f;
        audioProcessor.Band3_Freq = 750.0f;
        audioProcessor.Band4_Freq = 1800.0f;
        audioProcessor.Band5_Freq = 5000.0f;
        audioProcessor.Band1_Q = 2.000f;
        audioProcessor.Band2_Q = .707f;
        audioProcessor.Band3_Q = 2.00f;
        audioProcessor.Band4_Q = 1.414f;
        audioProcessor.Band5_Q = .350f;
        break;
    }

    }

    //R1.00 Create formatted strings for our UI.
    UIS_Band1 = std::to_string(int(audioProcessor.Band1_Freq));
    UIS_Band2 = std::to_string(int(audioProcessor.Band2_Freq));
    UIS_Band3 = std::to_string(int(audioProcessor.Band3_Freq));
    UIS_Band4 = std::to_string(int(audioProcessor.Band4_Freq));
    UIS_Band5 = std::to_string(int(audioProcessor.Band5_Freq));

    //R1.01 We changed some stuff so refresh the screen/UI.
    if (ForcePaint) repaint();

}
