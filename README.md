# Mako STACKED
* JUCE VST3 Guitar Amp Sim.
* Tested on Windows only.
* Written in Visual C++ 2022.
* Written for new programmers, not complicated.
* Version: 1.10
* Posted: June 3, 2024

VERSION
------------------------------------------------------------------
1.00 - Initial release.

DISCLAIMER
------------------------------------------------------------------  
This VST was written in a very simple way. No object defs, etc. 
Just simple code for people who are not programmers but want to 
dabble in the magic of JUCE VSTs.

If you are new to JUCE code, the Mako Thump VST may be a better
start place.
       
SUMMARY
------------------------------------------------------------------
A Juce/C++ VST3 written to simulate a guitar amplifier. 

![Demo Image](docs/assets/makostackeddemo01.png)

#THEORY OF OPERATION<br />
This VST is designed to mimic a medium gain amplifier. It uses
a typical OverDrive/Distortion pedal technique of passing the
guitar signal into a 700 Hz Band Pass filter and then distorting
the signal. 

The band pass filter frequency and bandwidth value (Q) are
adjustable.

ENHANCE
There are also three additional gain adjust stages:
- Enhance High, adds distorted high freqs before the gain stage.
- Enahnce Low, adds distortion thickness after the gain stage.
- Thump, add low freq distortion to mimic speaker cab/compression.

5 BAND EQ
There are also 10 predefined 5 band EQs available.

IMPULSE RESPONSE
There is also an Impulse Response section, to add a realistic
speaker response. IRs are typically WAVE files 1024 or 2048 samples
in length. For simplification, the IRs used here are in stored in 
code as float arrays. There are 5 IRs available.

COMPRESSOR
The program also add a pre gain compressor stage. This can help 
get a fatter tone in certain instances.

