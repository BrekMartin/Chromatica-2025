# Chromatica-2025
Chromatic Tuner for Sony Playstation Portable



This is a new program for tuning instruments with the Sony PSP-3000 models!
It works in a similar manner to consumer chromatic tuners, and tuner apps.

Like other chromatic tuners, the program has no user interface interaction.
It just listens to the biult in microphone of PSP-3000 models, and displays
the nearest note name, and a tuning guide to indicate a note is either flat
or sharp, and a string needs to be tuned up or down.

The tuning indicator near the bottom of the display moves horizontally, and
the width of the PSP display represents a window of 100 cents. The far left
of the screen is -50 cents, and the far right is +50 cents, and a permanent
mark in the mddle indicates the target true note reading you should aim for.

Note that the program will likely work with other model PSP units, so long
as they have the headset microphone, or some other audio input arrangement.

It is possible to plug an electric guitar directly into many PSP models with
the correct headphone remote control hardware. Although PSP accessories are
getting expensive to buy for the purpose of modification these days, I have
made a YouTube video about an audio effects program showing how to do this:
https://youtu.be/MlPtfeSyyak?si=5f9svxTTRuPo8Mld




12/08/25:
First working implementation of KISSFFT with wave and frequency displays.

13/08/25:
Started a basic tuning guide to indicate sharp or flat guitar strings.
Ditched the FFT version and created a version based on the Yin algorithm.
This is more CPU heavy, but appears to give better results for guitar.

14/08/25:
Made a copy of the audio buffer before processing so that it isn't written
with new audio data while the Yin algorithm is processing. Also prevented
the wave being drawn while processing, by moving the wave draw to the main
loop, which improves speed. Made the tuning indicator larger.
Slight fix to the wave display. Some minor optimisation in processing.
Intro is much faster so program starts faster.
Reduced the audio processing buffer to 2048 samples to double the speed.
The sample size can be changed to 4096 for a session by holding the up and
triangle buttons as the program is started.

15/08/25:
Created a smooth colour gradient for the tuning indicator. Colours transition
from red at the extreme sharp and flat edges of the scale, to orange, yellow,
and through to green for the center. Removed frequency display by default.
Removed the intro by default to make the program start as fast as possible. 

16/08/25:
Increased audio sensitivity setting. Slight fix for wave display left edge.
Increased the size of the tuning indicator and added a fade effect.

18/08/25:
Speed optimisation by only checking partial buffer for sufficient energy.
Lowered the Yin detection threshold to 0.08, which makes the program more
strict against noise to register a pitch detection.

23/08/25:
Holding the Square button at startup now sets Yin detection threshold to 0.04
for the current program session. Program will be even more sensitive this way.
Doubled the size of the font used to display detected note names.

24/08/25:
Increased the font size for the displayed detected note name even further.

25/08/25:
Included a mode to display the detected frequency with the large font instead
of the note name, mostly for the purpose of a YouTube video.
Supported the lower case "z" character for "Hz" in the frequency displays.
Made most special modes require holding two buttons down at startup instead of
just one, to prevent accidental triggering of a special mode.
