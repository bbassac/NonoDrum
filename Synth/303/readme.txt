An Arduino synth using mozzi library.

The goal of this project is to create a TB-303 like midi synthesizer with:

ADSR envelope
Low pass filter
Midi control
Multiple waveshape
The project is under development.

To connect the audio part, please follow this instructions.

Then, connect potentiometers to A0-A7 which are:

A0: Attack
A1: Decay
A2: Sustain
A3: Release
A4: Filter cut-off
A5: Filter resonance
A6: Filter amount (with ADSR envelope)
A7: Tuning
Then, connect a push button to pin 2, The pin 2 is used to change the setting. The settings are:
	Legato: off / on / glide / legato+glide
	Waveshape: saw / square

a switch button to pin 11  used to choose between legato setting and waveshape setting. 
two leds to pin 3 and 4. The pin 3 and 4 are used to display settings.  
Finally, connect the midi input to the pin 1 following any arduino midi in schematic.

