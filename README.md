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

# THEORY OF OPERATION<br />
This VST is designed to mimic a medium gain amplifier. It uses
a typical OverDrive/Distortion pedal technique of passing the
guitar signal into a 700 Hz Band Pass filter and then distorting
the signal. <br />

The band pass filter frequency and bandwidth value (Q) are
adjustable.<br />

ENHANCE<br />
There are also three additional gain adjust stages:
- Enhance High, adds distorted high freqs before the gain stage.
- Enahnce Low, adds distortion thickness after the gain stage.
- Thump, add low freq distortion to mimic speaker cab/compression.

5 BAND EQ<br />
There are also 10 predefined 5 band EQs available.

LOW AND HIGH PASS FILTERS
As you add gain, you may want to remove bass from the signal. Use the High Pass 
filter for this. As you add gain, you will add high frequency harmonics. This 'fizz'
can be very harsh. Use the LOW PASS filter to get rid of the fizz.

IMPULSE RESPONSE<br />
There is also an Impulse Response section, to add a realistic
speaker response. IRs are typically WAVE files 1024 or 2048 samples
in length. For simplification, the IRs used here are in stored in 
code as float arrays. There are 5 IRs available.

IRs are very CPU heavy. For best results an IR of sample size 2048 should be used.
This VST uses 1024 based IRs to keep CPU usage low. A slider could be added to 
reduce the sample size even further, at the cost of fidelity.

COMPRESSOR<br />
The program also adds a pre gain compressor stage. This can help 
get a fatter tone in certain instances.

TUBE THINNER<br />
This hack tries to reduce the guitar signal volume when driving hard into the clipping stages.
It should only be used when soloing if you are hearing ghost notes and alien sounds. Since we
applying a lot of gain, we wil start to hear random noises that are normally at the noise floor
of your guitar signal. The thinner tries to reduce your volume between note peaks to reduce these
weird noises.

CLIPPING<br />
The backbone of a distorted sound is the clipping stage. This is the magic place where people
feel tubes amps excel. We have two types of clipping in this VST, Hard and Soft. Hard does exactly
what it says. Any signal above 1.0 is clipped to 1.0. The soft method uses a hypertangent function.
This function creates a soft curve that limits the signal from -1.0 to 1.0. 

If you want to create the next best VST, this is where you want to start editing the code.

STEREO/MONO<br />
When in MONO mode, the VST uses the LEFT (Channel 0) channel for calculations. The audio is then
copied to the right channel. In stereo mode, all calculations are done twice. Once for each channel. 
This is expensive on CPU usage. Run Mono when possible for best results.

# FINAL THOUGHTS
The bones of an amplifier are here. Adjusting filter Q values and frequencies is one thing to play with.
Another thing to try is where EQ is applied.  

High gain amps have EQ before the gain stage. Because any low freqs will overwhelm the distortion stage.
This is how a FUZZ pedal works. So a certain amps EQ style and position can determine how that amp sounds.

This VST is designed to be a sort of tool chest or playground for you to figure things out, play, and have fun.


