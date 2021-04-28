# botrytis
midi mono synth for daisy patch

* [Intro](#intro)
* [Philosophy](#philosophy)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Out-of-the-box](#out-of-the-box)
* [How to use](#how-to-use)
* * [Navigating the menu](#navigating-the-menu)
* * [Modulation sources and destinations](#modulation-sources-and-destinations)
* * [Main menu](#main-menu)
* * [Clock](#clock)
* * [Signal generators](#signal-generators)
* * * [Three oscillators](#three-oscillators)
* * * [Five envelopes](#five-envelopes)
* * * [Two LFOs](#two-lfos)
* * [Signal generators](#signal-generators)
* * * [Three effects](#three-effects)
* * * [Two mixers](#two-mixers)
* * [Outputs](#outputs)
* * [All mod sources](#all-mod-sources)
* [Getting a hang of it](#getting-a-hang-of-it)
* [Presets](#presets)

## Intro
Botrytis is firmware for the Daisy Patch. If you are unfamiliar with the daisy patch, you can checkout out the [product page](https://www.electro-smith.com/daisy/patch) and follow links from there. I have no affiliation with Electrosmith except as a satisfied customer.

Botrytis is a mono synth, primarily meant to be interfaced with a midi instrument. I personally use it with a midi keyboard, but you should be able to make good use of it with a sequencer, pads, or whatever. You can also do a decent amount without a midi controller, see [Note number sources](#node-number-sources) below.

## Philosophy
It's been fun to play around with the Daisy Patch, load a bunch of firmwares on it, and try different expirements. After all that though, I wanted a firmware that I could keep loaded on the hardware, and use as a solid musical tool without having to hook it up to the computer to load new firmware every time I wanted something different. I've designed botrytis to have many of the core functions I reach for, and to have enough opportunities for customization and experimentation to keep it from getting stale. I like to use my little eurorack system with a keyboard, so botrytis is centered around being a mono synth, but it has some functionality outside of that realm as well.

The Daisy Patch only has 5 knobs, so the "knob-per-function" design philosphy isn't really an option for a synth of any complexity. Also, because I want to keep the firmware from feeling stale as it stays loaded on the hardware, I didn't want to have to dedicate each knob or input or output to a single purpose. My solution to this is to make (almost) every input and output assignable. More about this in the explaination of Modulation Sources & Destinations below. Because this isn't knob-per-function, the general workflow I suggest is to use the menu to change parameters while experimenting, then assigning parameters to knobs or other controls if you want to use them when actually playing/performing.

## Prerequisites
You will of course need a Daisy Patch all powered up, and connected to a computer via USB to load the firmware. If you want to connect a midi device, you'll need some way to send out the midi signal through a TRS cable. I'm using a 5-pin cable out from my keyboard, into a midi-to-trs adapter, into the Patch.

## Installation
Follow the [daisy wiki](https://github.com/electro-smith/DaisyWiki/wiki) if you don't already know to flash firmware to the patch. Then you can just download the [botritus binary](build/main.bin) and flash it.

## Out-of-the-box
Out of the box, here's how botrytis is set up and how you can start hearing sound out of it. Plug your TRS midi cable into the midi-in port, and plug a jack into one out the audio out ports. The jack can be connected straight to you headphones if you are ready for it to be really loud, or into an audio interface. If you've got a hang of the menu navigation and want to use headphones, you can turn down the fourth output's volume by navigating to "mixer 1" then "master level". The first output is a simple sine wave with volume modulated by an envelope. The second is a triangle wave with volume modulated by a slightly different envelope, then passed through a phaser. The third is a square wave with a third envelope modulating its filter frequency instead of its volume. The fourth is a mix of all three, with some delay on the sine wave. Also, the gate out is dutifully sending a clock signal while you play. This is just a taste of what botrytis can do, and you are by no means stuck with any of these sounds.

## How to use

### Navigating the menu
All navigation of the menu is done with the single encoder knob. Rotating the knob will move between menu items if you are in a menu, or will increase/decrease the value if you are editing a parameter. "Clicking" the encoder is general the "okay" action. It will navigate into a deeper menu or toggle an on/off menu item. If you are editing a parameter, clicking the encoder allows you to change the value in pre-set increments, and clicking again toggles back to changing the value in a more continuous way. Clicking and holding the encoder triggers the "back" action. This will always bring you back to the previous menu.

### Modulation sources and destinations
Not unlike connecting two eurorack modules with a jack, the internals of botrytis allow for two components to modulate eachother in a generic way. As far as terminology goes, I'm calling anything that sends out a signal a "modulation source" and anything that listens to that signal a "modulation destination". To make a connection, you navigate to modulation destination, then choose a modulation source to listen to. Most modulation destinations have "Mod add" and "Mod subtract" parameters that control how much the source influences the destination. For example, if an LFO is a mod source, an oscillator's detune is the destination, the mod-add is 2 and the mod-subtract is 1, then when the LFO is in a positive position it will increase the oscilator's frequency by up to 2 semitones, and when the LFO is negative it will decrease the oscilator's frequency by up to 1 semitone. A modulation destination also has an assignable multiplier, which is a sort of modulation destination itself that scales the modulated value. Most modulation destinations have 4 slots, so each can have up to 4 things affecting it.

### Main menu
Botrytis is composed of a handful of various functions, modules, utilities, or whatever you want to call them. These are all accesible through the main menu, vaguely categorized by row.

### Clock
The clock determines how long a period of time is in the context of the synth engine. All time-based parameters in botrytis are relative to the clock, e.g. an envelope's attack time of 3% means it lasts 3% of the clock's period. The clock's period is defined in BPM (beats per minute), so the higher the BPM the shorter the period. The other parameters of the clock only matter if you are using it as a modulation source. As a modulation source, the clock acts as a low-frequency square wave (or in other words a repeating gate). The "division" parameter dictates how often to open the gate. By default it is set to 4, meaning the gate opens 4 times per period. The invert toggle swaps the open and closed states of the gate. The "reset gate" modulation destination can be set to some other gate (e.g. for me I send a gate out from the play button in my DAW) that restarts the phase of the clock, so you can get all your clocks in sync.

### Note number sources
Also in the top row is a toggle between "use midi notes" and "use note number params". Using midi notes means the base note for all oscillators is based on whatever note the midi device is sending, but you can still detune individual oscillators away from that root note. Toggling to "use note number params" lets you define the root note with the "note number" parameter that will appear to the right.

### Signal generators
The items in the second row of the main menu are  primarily signal generators, which is to say they create a signal rather than process it.

#### Three oscillators
Botrytis has 3 key-tracked oscillators. Each has the following parameters: level, wave morph, detune, phase shift, glide, and glide gate. Wave morph dictates the oscillator's wave shape (e.g. sine, saw, etc.). Detune changes the frequency of the oscillator relative to the midi note played. Detune values are in semitones. Phase shift does what it says, and is primarily useful as a modulation destination for another oscilator, resulting in FM sounds. Glide dictates the time it takes to glide from one note to another (remember time based-parameters are percentages relative to the clock period). Glide gate is a modulation destination that acts as a gate that determines whether or not to glide, so if the value is zero, no glide, but if the value is non-zero, glide. By default glide gate has the midi gate set as the modulation source, but could be set to anything else, like an envelope for example.

#### Five envelopes
There are 5 envelopes that can be configured and routed separately. Each has the standard attack, decay, sustain, and release parameters. The gate modulation destination is, well, the envelope's gate. By default the gate has the midi gate set as its modulation source, but could easily be hooked up to the clock or an external sequencer.

#### Two LFOs
The LFOs have some similarities with the oscillators, except that their frequencies are set by the "period" parameter that describes the time it takes to complete a cycle relative to the clock speed, rather than keytracked and detuned like the oscillators. Its gate modulation destination resets it to the start of the wave, which can be useful in combination with its phase shift parameter for example if you want the LFO to start at the same position every time a midi key is hit. It can also be toggled between bi-polar and uni-polar mode.

### Signal generators
The items in the third row of the main menu are primarily signal processors, which is to say they change an already-existing signal.

#### Three effects
The three effects Filter, Phaser, and Delay are all configurable in similar ways, but of course have very different sounds. Each has a "source" modulation destination, which you set to the signal you want to process (e.g. the output of an oscillator). Each has an active/bypassed toggle, which should be self-explanitory. The other parameters are specific to each effect.

#### Two mixers
Each of the two mixers have 4 inputs. Each input is a modulation destination for a signal, and has a corresponding level to scale the signal. There is a master level that further scales the mixture.

### Outputs
The items in the fourth row of the main menu are modulation destinations for the different physical outputs on the Daisy patch. For example, send what you want to hear to the audio outs, the clock signal to the gate out, an LFO to one CV out and an envelope to the other.

### All mod sources
On the last row is the "modulation sources" menu which lets you view which modulation destinations a modulation source has set. Mod sources only show up in this list if they have destinations assigned to them. You will see the full list of mod sources when accessing a list via a mod destination. Below are descriptions of each mod source:

* **Controls 1-4**: These refer to the four CTRL knob-jack combos on the Daisy Patch. When nothing is plugged into the jack, this mod source sends the knobs position, and when something is plugged in the mod source sends the value of the signal coming through the jack. You can use the knobs as macros, or the jacks as external CV controls, or do whatever you want to do with them.
* **Audio Ins 1-4**: These refer to the four audio-in jacks on the Daisy Patch. You can use this to send external sounds signals through the effects, or use them to FM one of the internal oscillators, or do whatever you want to do with them.
* **Gates 1-2**: These refer to the two gate-in jacks on the Daisy Patch. You can use them to send sequencer gates in, or do whatever you want to do with them.
* **Clock out**: This is the square wave produced by the clock. I like to send it out of the gate to clock a sequencer.
* **Oscillators 1-3**: These are the outputs of each oscillator. You can FM another oscillator with it, send it to an effect or mixer, or do whatever you want to do with it.
* **Filter out, phaser out, and delay out**: These are the processed signals coming out of these effects. Send them to a mixer, to an audio out, or whatever.
* **Midi gate**: This is a gate that's on when a midi key is pressed and off otherwise. Send it to the gate of an envelope, the reset gate of an LFO, or whatever.
* **Velocity**: This is the midi velocity of the active midi key. Send it to an envelope's mod multiplier, or do whatever you want with it.
* **Channel pressure**: If your midi device supports channel pressure, this is that.
* **Envelopes 1-5**: These are the envelope outputs as dictated by the gate and the ADSR values. Send them to an oscillator level, filter frequency, or do whatever.
* **LFOS 1-5**: These are the LFO outputs.
* **Mixers 1-2**: These send out the mixed sources.
* **None**: This clears mod source for the selected mod destination.

## Getting a hang of it
I'm aware that it might be a bit odd to get used to at first, but once you get used to routing things around as modulation sources and destinations, you've pretty much learned all you need to. I suggest looking through the mod source list for the initiliazed state, and seeing how signals are routed around and how that results in the sounds it produces, and then going on from there to tweak parameters and modulation sources.

## Presets
Nope. I had some trouble reading from and writing to the SD card. If someone wants to help me out with that let me know, and then I can move forward with implementing presets. In the meantime, embrace impermanence.
