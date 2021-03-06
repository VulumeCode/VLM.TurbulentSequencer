VLM.Turbulent Sequencer
===


    This repo contains the TypeScript and C code for the externals of the VLM.Turbulent Sequencer. The project underwent a few iterations going from only TS to TS+Java to finally TS+C. Now only the graphical elements use TS (compiled to JS) and all the rest is compiled C. All old code is kept for historical documentation and because TS is way easier to experiment with than C. 



Periodic colored noise based sequencer for monosynths.
====

Demo video https://www.youtube.com/watch?v=_ceRrZ5c4CQ  
Get it on Gumroad for updates https://gumroad.com/l/dRhWW  
Discuss on llllllllines https://llllllll.co/t/40271  
Github for the TypeScript and C code https://github.com/VulumeCode/VLM.TurbulentSequencer  

_.~"(_.~"(_.~"(_.~"(_.~"(_.~"(_.~"(_.~"(_.~"(_.~"(
---

A very tweakable deterministic random sequence generator where all parameters are automatable.

It uses FFT and distributions to generate a sequence, so everything is repeatable. You can save the device as a preset or with your project and it will load up the same sequence. Automate a parameter to generate variations of the sequence throughout a track, or map it to an LFO to make infinite slowly evolving ambient sequences.

- Set the length and seed for the random sequence. The sequence is reused for tones and dynamics.

- "Beta" sets the color of the noise. At 0 it is white noise (a note has no correlation with the next note). At 2 it is brown noise (the next note is never far, but still random). At 1 it is pink noise (a mix between the two). Higher than 2 will let the periodicity constraint dominate the noise. At 10 there is no more randomness (seed doesn't matter).

- "Period" sets the periodicity constraint of the FFT filter. It can be set as a percentage of the sequence length, and snapped to a whole number step count. This is best explored with "Beta" up to 10. It has no effect when "Beta" is set to 0.

- "Morph" rotates the complex-number sequence from which the sequence is pulled.

~^~^~Tone generation~^~^~^~^~^~
---

Tones are generated from a distribution of octaves and notes in the octave. It is kind of like a probability distribution, but there is no more randomness at this point. The filtered noise sequence is mapped to this distribution.

- With the piano you can easily filter out notes without losing the distribution. The shift and rotate knobs make it more playable.

- Can also be shifted with MIDI input (center = C3).

- Set a white-keys only scale and steep distribution curve for the first ever meaningful mode-based random generation. Ionian and Dorian actually sound different! There are actual tonics!

    ~^~^~Dynamics generation~^~^~^~^~

Dynamics are generated in a similar way. The sequence is split in 3 resizable regions which can be assigned Rest, Tie, and/or Accent.

MIDI input velocity is taken into account when in MIDI clock mode.

~^~^~Playback~^~^~^~^~^~^~^~^~
---

The resulting MIDI loop can be seen in the piano roll. Long notes tie to the next. Purple colored background notes are accented. Hollow notes are rests. "Fold" shows only possible notes, "Span" shows all from lowest to highest note.

The MIDI loop can be nudged left and right, and start and end points can be set.

There are 2 playback modes

- MIDI clock: Step on midi note input. Very tight and allow the use other MIDI sources and Live's groove pool etc. Resync on Live play. Prefer this option if you use MIDI note shifting.

- Live Sync: Sync with Live's clock. Sloppy but has timeline sync. Prefer this option if you automate other parameters.

_.~"~._.~"~._.~"~._.~"~._.~"~._.~"~._.~"~._.~"~._
---

The zip contains the .amxd MIDI device and a project with presets with ideas for acid house, chords, minimalist music...

v0.1  
Initial release.  
v0.2  
Fixed js dependencies.  
v1.0  
Ported to Java.  
MIDI input.  
Nudge/start/end loop controls.  
Documentation.  
v2.0  
Ported to C.  
