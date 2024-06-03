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

//R1.00 Structure to hold our slider/knob screen positions.
struct t_KnobCoors {
    float x;
    float y;
    float sizex;
    float sizey;    
};

//*******************************************************************************************************************
//R1.00 Create a new LOOK AND FEEL class based on Juces LnF class.
//R1.00 We will override the SLIDER drawing routine.
//*******************************************************************************************************************
class MakoLookAndFeel : public juce::LookAndFeel_V4
{
public:

    juce::Image imgKnobDent = juce::ImageCache::getFromMemory(BinaryData::knobdent_png, BinaryData::knobdent_pngSize);

    MakoLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::red);
    }

    //R1.00 Override the Juce SLIDER drawing function so our code gets called instead of Juces code.
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        float sinA;
        float cosA;
        float sinAR = sinf(angle) * radius;
        float cosAR = cosf(angle) * radius;
        juce::ColourGradient ColGrad;

        //**********************************************************************
        //R1.00 Draw the KNOB background.
        //R1.00 Set background brightness based on setting. sliderPos (0 - 1.0f).
        //**********************************************************************
        //int Col = 0xFF000000 | (int(sliderPos * 0x00FF0000) & 0x00FF0000); //R1.00 Calc RED val and MASK off Green and Blue bits.
        //Col |= (int(sliderPos * 0x00008000) & 0x0000FF00);                 //R1.00 Calc GREEN val and MASK off Red and Blue bits.
        //g.setColour(juce::Colour(Col));
        //g.drawEllipse(rx - 1.0f, ry - 1.0f, rw + 2.0f, rw + 2.0f, 2.0f);

        //**********************************************************************
        //1.00 Draw the KNOB face.
        //********************************************************************** 
        //ColGrad = juce::ColourGradient(juce::Colour(0xFFF0F0E0), 0.0f, y, juce::Colour(0xFFA0A0A0), 0.0f, y + height, false);
        //g.setGradientFill(ColGrad);
        //g.fillEllipse(rx, ry, rw, rw);

        //**********************************************************************
        //R1.00 Knob notches.
        //**********************************************************************
        ColGrad = juce::ColourGradient(juce::Colour(0xFFC0C0C0), 0.0f, y, juce::Colour(0xFF808080), 0.0f, y + height, false);
        g.setGradientFill(ColGrad);
        for (float ang = .7854f; ang < 6.0f; ang += .7854f)
        {
            sinA = sinf(ang + angle) * radius;
            cosA = cosf(ang + angle) * radius;
            g.drawLine(centreX + (sinA * .8f), centreY - (cosA * .8f), centreX + sinA, centreY - cosA, 2.0f);
        }

        //**********************************************************************
        //R1.00 Draw finger adjust dent.
        //**********************************************************************
        auto dentSize = 12.0f;
        auto dentSize2 = 6.0f;
        auto dentX = centreX + (sinAR * .5f) - dentSize2;
        auto dentY = centreY - (cosAR * .5f) - dentSize2;
        //ColGrad = juce::ColourGradient(juce::Colour(0xFF000000), 0.0f, dentY - dentSize2, juce::Colour(0xFFFFFFEF), 0.0f, dentY + dentSize2, false);
        //g.setGradientFill(ColGrad);
        ////g.setColour(juce::Colour(0xFF402020));
        //g.fillEllipse(dentX, dentY, dentSize, dentSize);
        g.drawImageAt(imgKnobDent, dentX, dentY);

        //**********************************************************************
        //R1.00 MAKO version of KNOB position indicator (pointer).
        //**********************************************************************
        //ColGrad = juce::ColourGradient(juce::Colour(0xFFE0E0E0), 0.0f, y, juce::Colour(0xFF404040), 0.0f, y + height, false);
        //g.setGradientFill(ColGrad);
        g.setColour(juce::Colour(0xFFF0F0F0));
        g.drawLine(centreX + sinAR * .8f, centreY - cosAR * .8f, centreX + sinAR, centreY - cosAR, 4.0f);

        /*
        //************************************************************************
        //R1.00 JUCE version of KNOB position indicator (pointer).
        //R1.00 This method may be faster and is better for drawing weird shapes.
        //************************************************************************
        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 6.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(juce::Colours::red);
        g.fillPath(p);
        */
    }
};


//*******************************************************************************************************************
//R1.00 Add SLIDER listener. BUTTON or TIMER listeners also go here if needed. Must add ValueChanged overrides!
//*******************************************************************************************************************
class MakoBiteAudioProcessorEditor  : public juce::AudioProcessorEditor , public juce::Slider::Listener //, public juce::Button::Listener , public juce::Timer
{
public:
    MakoBiteAudioProcessorEditor (MakoBiteAudioProcessor&);
    ~MakoBiteAudioProcessorEditor() override;

    //R1.00 OUR override functions.
    void sliderValueChanged(juce::Slider* slider) override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MakoBiteAudioProcessor& audioProcessor;

    MakoLookAndFeel myLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MakoBiteAudioProcessorEditor)

    juce::Image imgLogo;
    juce::Image imgBackground;

    //R1.00 COMBO BOX VARIABLES.
    juce::ComboBox cbPreset;
    void cbPresetChanged();
    void cbPreset_UpdateSelection(int button, int idx, int editmode);
    void Preset00();
    void Preset01();
    void Preset02();
    void Preset03();
    void Preset04();
    void Preset05();

    void Setting_UpdateProcessor(int SettingType);

    void GUI_Init_Large_Slider(juce::Slider* slider);
    void GUI_Init_Small_Slider(juce::Slider* slider);
    void Band_SetFilterValues(bool ForcePaint);

    //R1.00 These strings hold the current EQ frequencies for UI.
    juce::String UIS_Band1 = "";
    juce::String UIS_Band2 = "";
    juce::String UIS_Band3 = "";
    juce::String UIS_Band4 = "";
    juce::String UIS_Band5 = "";

    juce::uint32 colorButton = 0xFFA0A0A0; //  0xFFC0B4A0;

    //R1.00 Define our UI Juce Slider controls.
    juce::Slider jsP1_Gain; 
    juce::Slider jsP1_MidFreq;
    juce::Slider jsP1_MidQ;
    juce::Slider jsP1_Drive;
    juce::Slider jsP1_LowPass;
    
    juce::Slider jsP1_HighPass;
    juce::Slider jsP1_EnhLow;
    juce::Slider jsP1_EnhLowFreq;
    juce::Slider jsP1_EnhHigh;
    juce::Slider jsP1_EnhHighFreq;
    juce::Slider jsP1_NGate;

    juce::Slider jsP1_Thump;
    juce::Slider jsP1_Band;
    juce::Slider jsP1_Band1;
    juce::Slider jsP1_Band2;
    juce::Slider jsP1_Band3;
    juce::Slider jsP1_Band4;
    juce::Slider jsP1_Band5;

    juce::Slider jsP1_IR_Model;
    juce::Slider jsP1_IR_On;
    juce::Slider jsP1_ClipMode;
    juce::Slider jsP1_Mono;
    juce::Slider jsP1_Thin;
    juce::Slider jsP1_CompThresh;


    //R1.00 Add some context sensitive help text. 
    int ctrlHelp = 0;
    int ctrlHelpLast = 0;
    juce::Label labHelp;

    //R1.00 Define the coords and text for our knobs. Not JUCE related. 
    t_KnobCoors Knob_Pos[30] = {};
    juce::String Knob_Name[30] = {};
    void KNOB_DefinePosition(int t, float x, float y, float sizex, float sizey, juce::String name);


public:
    
    //R1.00 Define our SLIDER attachment variables.
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Gain;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_HighPass;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_MidFreq;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_MidQ;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Drive;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_LowPass;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_EnhLow;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_EnhLowFreq;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_EnhHigh;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_EnhHighFreq;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_ClipMode;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_NGate;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_IR_Model;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_IR_On;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Mono;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Thin;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_CompThresh;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Thump;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band1;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band3;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band4;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> ParAtt_Band5;

};
