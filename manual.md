Werckmeister
===================


<img src="https://raw.githubusercontent.com/werckme/werckmeister/main/assets/hero.png" class="hero-image" alt="from sheet code via Werckmeister to MIDI music">
<br><br>

## An [open source](https://github.com/werckme/werckmeister) Sheet Music MIDI Compiler

Werckmeister compiles sheet music source code into a MIDI file.
<br>It allows you to write sheet music a lá "Real Book" and Werckmeister compiles it into MIDI music.

```language=Werckmeister,type=single,tempo=140
   c'4   g8 g   a4 g~  |   g  b c'
```

## Use Werckmeister if you want to
* translate sheet music into midi music
* prototype a song
* experiment with chord progressions or different accompaniment styles  
* compose using readable source code instead of proprietary files formats

## Features
 * open source
 * a fast sheet music MIDI compiler
 * a sheet music player
 * accompaniment template rendering engine
 * [Lua](https://www.lua.org/) script extensions


Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords, add some remarks, which instruments and which style.

Thats it.

This is the idea behind Werckmeister. Of course it can not replace a real musician or a band. 
But it offers you a tool to create music just by writing melodies and chords.

## Try it out online

### Autumn Leaves
This is the jazz standard "Autumn Leaves" written in werckmeister notation.
<br>Click play to start the song.

For fun sake try to replace the bass template 
```
I,,2. V,,4 |
```
with this 
```
I,,4 I,8 I,8   I,,4 I,8 I,8 |
````

```language=Werckmeister   
using "chords/default.chords";
tempo: 140;
device: MyDevice  midi _usePort=0;
instrumentDef:lead    _onDevice=MyDevice  _ch=0 _pc=0;
instrumentDef:rhtythm _onDevice=MyDevice  _ch=1 _pc=0;
instrumentDef:bass    _onDevice=MyDevice  _ch=2 _pc=0;

-- melody track
[
instrument: lead;
{
    \fff
    r4 e f# g  | c'1~       | c'4 d e f#  | b2 b2~ | 
    b4 c d e   | a1~        | a4 b, c# d# | g1     |
    r4 e f# g  | c'1~       | c'4 d e f#  | b2 b2~ | 
    b4 c d e   | a1~        | a4 f# a g   | e1~    |
    e4 r d# e  | f# b, f#2~ | f#4 f# e f# | g1~    |
    g4 g f# g  | a1~        | a4 d d' c'  | b1~    | 
    b4 r a# b  | c' c' a a  | f#2. c'4    | b2 b2~ | 
    b2. e4     | a2. g4     | f#2 g4 b,   | 
    e1 
}
]

-- accomp track
[
type: accomp;
{   
    r      | A-7    | D7    | Gmaj7  | 
    Cmaj7  | F#-7b5 | B7    | E-     | 
    E-     | A-7    | D7    | Gmaj7  | 
    Cmaj7  | F#-7b5 | B7    | E-     | 
    E-     | F#-7b5 | B7b9  | E-     | 
    E-     | A-7    | D7    | Gmaj7  |
    Gmaj7  | F#-7b5 | B7b9  | E-7 A7 | 
    D-7 G7 | F#-7b5 | B7b9  | E-     |
}
]

-- the templates defines what the accomp track has to play
[
type: template;
name: myAccomp;
instrument: rhtythm;
{
    \p
    <III, V, VII, II>1 | 
}
]

[
type: template;
name: myAccomp;
instrument: bass;
{
    \p
    I,,2. V,,4 |
}
]
```

Getting Started
============

## Download & Installation
Have a look [here](https://werckme.github.io/getwerckmeister).

Find the download and follow the installation instructions for your operation system.


## Installation is done, what now?
After the installation, two executables should be available:
* `sheetc` a sheet file compiler  
* `sheetp` a sheet file player

Open a command line shell: `cmd` on windows or `terminal` on Mac.

Type into the terminal:

```
sheetc --help
```

You should get an output like this:
```
  --help                produce help message
  --input arg           input file
  --output arg          output file
  ...
```
If not, please make sure that the werckmeister `bin` path of your installation is written into your `PATH` environment variable.

## Compiler
Run `sheetc aSheetFile.sheet` to compile a sheet file into a MIDI file.

## Player
To playback a sheet file you can do this by executing `sheetp`.
Just as with the compiler type in `sheetp` and then the file you want to playback.

```
sheetp aSheetFile.sheet
```

In order to get an sheet file played via MIDI, you have to configure set your MIDI ports. 

Have a look [here](/manual#adding-a-device) to see how it's done.

## Examples

To get a good impression of what is possible and how it can be achieved, it is recommended to have a look at the example sheets.

To can find them in your installation folder under `shared/werckmeister/examples`. Or download them from [here](https://github.com/werckme/werckmeister/tree/v0.1.53/examples).

You find the examples here:

| System  | Location |
| ------  | -------- |
| Windows |  C:\Program Files (x86)\werckmeister x.x.x\share\werckmeister\examples  |
| Mac |  /usr/local/share/werckmeister/examples  |
| Linux |  /usr/local/share/werckmeister/examples  |
*(it may differs to your machine, depending on where you installed werckmeister)*

## The Werckmeister Visual Studio Code Extension
The recommended way to edit Werckmeister sheet files, is using Visual Studio Code with the
 Werckmeister extension. It supports playback right from the editor. With visual feedback and error/warning highlighting.

Have a look [here](/code-extension).

## Using Werckmeister along with a DAW
All you need is to connect the `sheetp` player with the MIDI inputs of your DAW.

On Windows there is some third party software for that purpose. In my opinion [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) does a pretty good job.

Manual
======
I want to ...
--------------
[... get an impression of what I have to face if I want to learn Werckmeister](#its-just-text)

[... setup a piece](#setup-a-piece)

[... write melodies](#write-melodies)

[... write for drums](#write-for-drums)

[... applying articulation while remaining readable](#conduction-rules)

[... accomp my melodies](#accomp-my-melodies)

[... setup my own chord symbols](#chords)

[... see an overview of all commands](#commands)

[... see an overview of lua extensions](#lua-extensions)

## It's just text
The Werckmeister language knows only a few statement types. It's all about tracks, voices, and events. That's it. 
<br>The remaining statements are just for configuration purposes.

A basic Werckmeister document will look like this:

```language=Werckmeister
-- CONFIG STUFF:
-- 1) configure your MIDI device
-- 2) define an instrument with the name piano
-- 3) set the tempo

-- 1)
--      name         type portnumber
device: MyMidiDevice midi _usePort=1;

-- 2)
--             name         
instrumentDef: piano _onDevice=MyMidiDevice  _ch=0   _pc=0  _cc=0;
-- (ch= channel, pc= programm change, cc= control change)

-- 3)
tempo: 130;

-- TRACKS, VOICES & EVENTS
-- now the actual musical statement, which is a track containing one voice with some note events
-- (the track is set up to use our piano instrument from above)
[
instrument: piano;
{
    c4 d e f | g1 --note events
}
]
```
## Tracks and Voices

A track in Werckmeister has exactly the same meaning as it has in a MIDI file.
It is related to an instrument and contains events.

A track begins with a `[` and ends with a `]`.
```language=Werckmeister,type=partial
-- a track with one voice and an instrument setup

[
{
    c d e f | g a b c'
}
]
```

All events of a track are related at least to one voice. A voice starts with `{` and ends with `}`.
If you want to write polyphonic melody lines, you can add more voices to your track. 



![a polyphonic melody](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/poly.png)

The melody above would be written like this:

```language=Werckmeister,type=partial
-- a track with two voices

[
{
    g4  f#8 g  a4   g
}
{
    d4   d     d    d
}
]

```

## Events
There are a few event types:

### Note Events
have a pitch and a duration, to be performed by a MIDI instrument.
```language=Werckmeister,type=mute
c d e f g
```

### Degree Events
have a relative pitch degree and a duration. In combination with a chord, degree events can be transformed into regular note events.
```language=Werckmeister,type=mute
I II III IV V VI VII
```

### Chord Events
Events which are related to a name. They also store the information which interval is used for a degree. These informations are stored in a [chord definition file](#chords).
```language=Werckmeister,type=mute
C7 D-7 E-7 F7 Gmaj7 A-7
```


### Meta Events
Events to set up something or loading a mod script.

*Meta events are also called [Commands](#commands)*

<br>You can recognize meta events by its slashes at the beginning and the end.
```language=Werckmeister,type=single
/tempo: 100/ -- meta event: set tempo to 100
c d e f |
/tempo: 50/
g a b c'


```

## Parameters
[Meta Events](#metaevents) or Config Statements will be configurated with parameters. Parameters can be defined in two ways:

#### Positional
`/volume: 50/` 

#### Named
`/volume: _to=50/`

For sake of understanding the meaning of statements, it's recommended to use named parameters if the meaning of a parameter isn't obvious.

**don't**<br>
`instrumentDef: piano MyMidiDevice 1 2 3;`

**do**<br>
`instrumentDef: piano MyMidiDevice  _ch=1  _pc=2 _cc=3;`



## Setup A Piece
### Using 
The first thing you want to do in a Werckmeister document, is to load include other files to your project. This may be chord definitions, accomp. templates or modulation scripts.

To include a file use the `using` command.
```language=Werckmeister,type=mute
using "./chords/default.chords";
```

The path of your file is written between double quotes (`"`).

The path can be absolute or relative to the current document.
*(It is recommended to use relative path values)*

### Devices & Instruments
#### Adding a device

To add a MIDI device to your project, use the [device](#device) statement. With that statement, you have to choose a name and set a MIDI port for your new device.

To see which devices with what port are connected to your device run the player with the `--list` argument: 

```
sheetp --list
```

you will get a output like this:

```
0: Midi Through
1: SC-8850 Part A
2: SC-8850 Part B
3: SC-8850 Part C
4: SC-8850 Part D
5: USB-Midi Port
```

where the first number means the port number.

So lets say we want to add the `SC-8850 Part A` to our project, we write:

```language=Werckmeister,type=mute
device: MyRoland midi _usePort=1;
```

To add a further MIDI port, for example a DX7 on the USB-Midi Port we add another line:
```language=Werckmeister,type=mute
device: MyRoland midi _usePort=1;
device: MyDX7    midi _usePort=5;
```

Imagine your USB-Midi Port is a bit cheap and laggy. Every signal to your USB-Midi port has a delay of about 20 milliseconds. We can compensate this by adding a 20 milliseconds delay to the `MyRoland` device, so that this device and your laggy USB-Midi Port should be in sync. We do this by using an optional `offset` argument.

```language=Werckmeister,type=mute
device: MyRoland midi _usePort=1 _withOffset=20;
device: MyDX7    midi _usePort=5;
```

#### Adding instruments
To adding instruments we using the [instrumentDef](#instrumentdef) statement, which is constructed like this:

Let's add an organ instrument which uses our Roland device from above:

The Roland SC-8850 offers, for example, a `Farf Organ`. So if I want to use this instrument on channel 0, I have to send a `cc=19 pc=17` sequence to the channel on this device.   
A suitable `instrumentDef` statement would be look as follows: 

```language=Werckmeister,type=mute
--             name      device              
instrumentDef: myFarfisa _onDevice=MyRoland  _ch=0   _cc=19   _pc=17;
```


#### Configuring instruments
With an instrument added, there are further options to configure it.
A [config statement](#instrumentconf) has as first argument the instrument name you want to configure, followed by several *optional* config aruments.

As an example we set a volume value of 80 to our Farfisa organ, also we set the pan more to the left.

*(the final result of these settings is strongly dependent by your actual MIDI device)* 

```language=Werckmeister,type=mute
instrumentConf: myFarfisa volume 80 pan 20;
```

### Tempo
To set the tempo of your piece, write `tempo: aNumber`.


```language=Werckmeister,type=partial
tempo: 120;
[
{
    c d e f | g a b c'
}
]
```

Sometimes you want to setup different tempi for for several voices.
Then you have to add a tempo [Meta Event](#meta-events) to your voice.

*since MIDI doesn't support simultaneous tempo, we achieve this effect by stretching or shrinking the note durations in relation to the main tempo (Augmentation/Diminution). As an simple example: on double time we pay eights instead of fourths.*

Here we play four voices on one track, with four different tempo configurations:

```language=Werckmeister,type=partial
tempo: 120;
[
{
    /tempo: double/
    c d e f | g a b c' | c d e f | g a b c'
}
{
    /tempo: half/
    c, d, e, f |
}
{
    /signature: 3 4/
    /tempo: 96/
   c e f | g b c'
}
{
    c,, d,, e,, f,, | g,, a,, b,, c,
}
]

```

Also it is possible to change the tempo in between events:
```language=Werckmeister,type=partial
tempo: 120;
[
{
    /tempo: double/
    c d e f | g a b c' |
    /tempo: half/
    c d e f | g a b c' |
    /signature: 3 4/
    /tempo: 96/
    c d e | f g a |
    /signature: 4 4/
    /tempo: normal/
    c d e f | g a b c' |
}
]

```


### Time Signatures
The time signature 4/4 by default.
If you want to setup a different time signature than the default, you can achieve this by using the signature [meta event](#meta-events).
With this you are able to setup different time signatures during a piece.
```language=Werckmeister,type=partial
[
{
    /signature: 3 4/
    c e e | c e e |
    /signature: 4 4/
    c d e f | g a b c'
}
]
```

### Key Signatures
key signatures are not supported.

## Write Melodies
### Notes
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notation syntax. A note is just a letter followed by a number for its duration.

*If a note has no duration, the last assigned duration will be used.*

```language=Werckmeister,type=single
c4 d e8 f
```

<br>*c and d will be played as quarter notes, e and f as eighth notes*

the end of a bar is marked by a pipe: `|`

```language=Werckmeister,type=single
c d e f | g a b c'
```


### Octaves

Lower octaves are marked by a comma (`,`) after the note, upper octaves by a single quote (`'`).

```language=Werckmeister,type=single
c,,  e,,  g,, b,, | c, e, g, b, | c e g b | c' e' g' b' | c'' e'' g'' b''
```

### Accidentals

`#` raises a pitch by a half step.
<br> A `b` lowers it. 

```language=Werckmeister,type=single
c# d# f# g# | bb db' eb' gb'
```

### Durations

A numeric value after a note marks its duration.

```language=Werckmeister,type=single
c2 c4 c8 c16 c32 c64 c128 r
```

### Intervals and Chords

Notes that have to sound simultaneous are written in angle brackets. The duration is written after the brackets.

![two chords](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/chords.png)

```language=Werckmeister,type=single,tempo=80
<g,, c e g bb>4   <c, c eb g bb>2.
```

### Augmentation Dots & Ties

To write a augmentation dot (`.`) append a dot after a note duration value.
<br>For ties use a tilde (`~`) instead. 

![dots and ties](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/dotsties.png)


```language=Werckmeister,type=single
c2. c4 | c2~ c4 c
```

### Tuplets

To write tuplets of any kind you just have to write notes enclosed by parentheses followed by a duration value. For example: `(c c c)4`.
These three notes will now be played in the time of one quarter.

It doesn't matter if you write `(c2 c2 c2)4` or `(c8 c8 c8)4` because you have three notes with equal durations to be played in the time of one quarter.

*The relations between the notes are vital, not their absolute durations.*

![n-tole examples with 3,7 and 9](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/tuplets.png)

```language=Werckmeister,type=single,tempo=60
(c8 c c)4   (c8 c c c c)4  (c8 c c c c c c)4  (c8 c c c c c c c c)4
```

### Nested Tuplets

You are also able to write nested tuplets:

![a example of nested tuplets](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/main/assets/blackpages.png)

```language=Werckmeister,type=single,tempo=60
b2 ((b c#' c#' b d')  (f' a a bb c')  (e' a' bb' f' c' g))2
```

### Repeats
```language=Werckmeister,type=single,tempo=200
c4 d e f :| g a b c' |
```

```language=Werckmeister,type=single,tempo=200
c4 d e f |: g a b ab :| c'1
```

##### Volta
```language=Werckmeister,type=single,tempo=200
c4 d e f |^1 g a b ab :|^2 c'1
```

```language=Werckmeister,type=single,tempo=200
c2 d2 |^1 c#2 d# :|^2 e2 f2 :|^3 c#'2 d#'2 | c'2 d'2 | c,,2 d,, :|
```

##### Mark/Jump
For more complex musical navigation you can use the [mark](#mark)/[jump](#jump) commands.
In combination with its ignore argument you are able to implement - for example - a coda:
```language=Werckmeister,type=single,tempo=200
/mark: begin/
c d e f | 
/jump: coda _ignore=1/
g a b c' |
d' e' f' g' |
/mark: coda/
g' f' e' d' |
/jump: begin/
c'1 |
```

## Write for drums
### Pitchmaps
With pitchmaps you are able to define own note symbols. This is useful espacially for drum notes.<br>
In a pitchmap file you can define note symbols like that:
```
"sn": c'  -- snare
"bd": c,  -- bass drum
```
*(a custom note symbol is also called alias)*


Since a delimited string like "bd" is harder to read than a single character, there are the following rules:

* every single lowercase alphabetic character is allowed as note
* a note can also have more than one character but then a delimiter is neccessary
* single character notes can be followed by the octave symbols `'` or `,`
* the characters c, d, e, f, g, a, b, r, t are predefined events
* predefined events can **not** be overwritten in a pitchmap

## Conduction Rules
### The Problem
A score document contains basically two informations:
* what notes to play
* and how to perform these notes

The regular score notation has a set of articulations to apply to a note.

![examples of articulation.](https://upload.wikimedia.org/wikipedia/commons/0/0e/Notation_accents1.png)

In werckmeister you also have such options, for example: `!ffff c ` to play the note c louder.


Unfortunately these kind of notation has the potential to destroy the readability of a source file.

A good example is this 16th note high hat figure:

*(`h` = `high hat`, see [Pitchmaps](#pitchmaps))*
```
h16 h h h  h h h h  h h h h  h h h h | 
```

If you want to add some articulation to that, the result is much more harder to read:

```
!pph16 !pph !ffh !pph  !pph !pph !ffh !pph  !pph !pph !ffh !pph  !pph !pph !ffh !pph |
```


### Conduction Rules trying to fix that.

The conduction rules separate between **what** and **how**. 

>If you are familiar with HTML and CSS, you already know the concept. Conduction rules are what CSS is for HTML.

You can achieve the same result, from above, using these rules:
```
instrument(drums) pitch(h) { 
    velocity = 51;
}
instrument(drums) pitch(h) onBeat(1.5 2.5 3.5 4.5) { 
    velocity = 89;
}
```
Find the full example [here](https://werckme.github.io/editor?wid=conductor16thHighHat).

A conduction rule starts with a list of selectors, followed by a set of declarations.

A **selector** defines on which notes the rule applies. 

A **declaration** describes what needs to be changed.

Referring to the rules above the selector `instrument(drums) pitch(h)` selects every `h` pitch of the instrument `drums`.

The declaration `velocity = 51;` set the velocity to the value `51` to the selected notes.

Find [here](#conductor-rules) a complete list of all supported selectors and declarations.

A declaration knows 3 types of value assignment:

### Assign
```
  aDeclaration = newValue; 
```

### Add
```
  aDeclaration = newValue; 
```
### Substract
```
  aDeclaration -= newValue; 
```

### Follow Up
```
  aDeclaration = & - newValue; 
```
The `Follow Up` assignment allows you to refer to the prevoious event of the same pitch. 

Example:
```
velocity = & - 10;
```
Means: the new velocity value is 10 units less than the velocity of its predecessor.

This usefull if you want to achieve something like this:
![linear decreasing velocity values](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/follow-up-velocity.png)

Find the full example [here](https://werckme.github.io/editor?wid=conductor16thHighHatFollowUp).


## Accomp My Melodies

Imagine you give your bass player some chords, and he or she asks you how to play these.
<br>You would say something like: play the 1st degree on the first beat and the 5th degree on the fourth beat. 

And this is what you doing when writing style templates.
<br>So in our example above you would write:

```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "chords/default.chords";

device: MyDevice  midi 0;
--            name  device       
instrumentDef:bass  MyDevice _ch=0  _pc=0;

-- here is the template:
[
type: template;
name: simpleBass;
instrument: bass;
{
    I,,2. V,,4 |
}
]

-- here are some chords:
[
type: accomp;
{
    /template: simpleBass/
    C | F | G | C |
}
]

```

Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note. 

*If a degree does not appear in a chord, the degree will not played.*

There are the same rules as for writing absolute [notes](#notes):

such as:

* use `#` or `b` for sharp or flat
* to perform intervals use angle brackes. For example a triad: `<I III V>`
* use `~` for tied and `.` for dotted notes
* create tuplets using parentheses


```language=Werckmeister,type=full
-- make sure that you have a chord definition loaded
using "chords/default.chords";

device: MyDevice  midi 0;
--             name  device       ch pc cc
instrumentDef:bass  MyDevice _ch=0  _pc=0;

[
type: template;
name: simpleBass;
instrument: bass;
{
    I,,2. V,,4 | <I,, III, V,>1
}
]

[
type: accomp;
{
    /template: simpleBass/
    C | F | G | C |
}
]
```
> the `template` command should always appear at the begining of a bar, not at the end.



## Advanced techniques

## Mods
If you want do write your own modification you need to:
* create a lua file
* define parameters
* implement a `perform` function
* include mod file
* apply mod

[find some example implementations](https://werckme.github.io/examples?tag=custom-lua-mod)

### create a lua file
Since the name of the lua file is also the name of the modification in werckmeister you can not use any name you want for that file.
* don't use special characters
* avoid naming conflicts

### Define Mod Parameters
If you want to use parameters in your modification, you need to define a global `parameters` table in your lua file. You can make a parameter optional by adding a default value.
```lua
parameters = {
    { name="myMandatoryParameter" },
    { name="myOptionalParameter", default="50" }
}
```

```
/mod: myMod _myMandatoryParameter=100/
```

### The Perform Function
In the `perform` function you can implement your modification. Werckmeister expects a table with event informations to be returned from that function. This is the most minimalistic implementation you could write:

```lua
function perform(events, params, timeinfo)
    return events
end
```

The perform function has 3 arguments:

### `events` argument
This table contains all input events.
```lua
{
    {
        -- the duration of the event
        duration = number,

        -- whether event is tied or not
        isTied = boolean,

         -- offset in quarters, affecting the position, 
         -- can be used to let events appear earlier or later  
        offset = number, 

        -- a table containing event tags (see Event Tags)
        tags = { tag1, ..., tagN },

        -- a table containing the events pitches, 
        -- contains several if event is a chord
        pitches = {
            {
                -- pitch alias if any. (see Pitchmaps)
                alias = string,
                -- the octave number of the pitch
                octave = number,
                -- the pitch number
                pitch = number
            },
            ...
        },

        -- if notes are tied together the tiedDuration gives you the remaining
        -- duration of the current event in that "tie chain"
        -- for example: "c4~c" 
        -- the tiedDuration here would be 2 for the first event 
        -- and 1 for the second
        tiedDuration = number, 

        -- the total duration of a "tie chain"
        totalTiedDuration = number,

        -- the event type
        type = string,

        -- the events velocity value in a range from 0 to 1
        velocity = number   
    }
    ...
}
```
### `params` argument
Contains the params if any. See [Define Mod Parameters](#define-mod-parameters)

### `timeinfo` argument
Contains a table with informations about the current musical time.
```lua
{
    -- the position of the current event in quarters
    quarterPostion = number,

    -- the numerator of the current time signature
    signatureNumerator = number,

    -- the denominator of the current time signature
    sinatureDenominator = number
}
```

### return value
Returns a table containing events. See [events](#events-argument)


### include mod file
To use your modification you need to include your lua file.
```lua
using "<<path to lua file>>"; -- path can be relative or absolute
```
### apply mod
You have two options to apply a mod: 
* apply to a voice
* apply to an instrument

#### apply to a voice
Use the mod command with the mod name. The mod name is also the [file](#create-a-lua-file) name of the lua script.
**syntax**
```
[
{
    /mod: <<modName>> [mod arguments]/
}
]
```

#### apply to an instrument
**syntax**
```
instrumentConf <<instrumentName>> mod <<modName>> [mod arguments]
```

**example**
```
instrumentConf: myInstrument 
    mod myModX _modXParameter=1 
    mod myModY _modYParameter=2;
```

## Event Tags
You can add additional annotations to an event in werckmeister. These informations can be used in [conduction rules](#conduction-rules) or [modifications](#mods).

**syntax**
```
"tag1 tag2 ..."@<<event>>
```
**example**

sheet file:
```
    c d e f | g a b "mytag"@c'
```

conduction rules file:
```
withTag(myTag) {
    velocity = 127;
}
```

#### TBD
##### Chords
##### Voicing strategies


## References





## Commands
### `cue`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| text | - | the text which appears in the MIDI message | text |

adds a cue MIDI meta message to the corresponding midi track

 #### examples

 `/cue: cueText/`

 > If a cue was visited several times during a repeat or jump an additional repeat index will be added.
 > For example:
 > `/cue: myCue/ c d e f :|` will create two cue MIDI messages: "myCue" and "myCue1"

 >
<br><br><br>

### `device`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| setName | 1 | An arbitary name. | text |
| isType | 2 | The type of the device. | [midi,fluidSynth] |
| usePort | 3 | The midi port id of your device. You can get a list of your connected devices, by executing `sheetp --list` | 0..N |
| withOffset |  | Defines an offset in milliseconds. Can be used to keep different devices in sync. | 0..N |
| useFont |  | Only valid if isType=`fluidsynth`. Sets the location of the soundfont file, which will be used by FluidSynth | a file path |

Defines a device which can be used when adding instruments (see [instrumentDef](#instrumentDef))

 see [instrumentDef](#instrumentDef), [instrument](#instrument)

 #### example

 define an device, an instrument and assign it to a track.

 ```

 device: MyDevice midi 0;

 instrumentDef: organ _onDevice=MyDevice _ch=0 _pc=16; 
 [ 
 instrument: organ; 
 { 
    c d e f 
 } 
 ] 
 ``` 
 **Fluidsynth**

 If you want to use [fluidSynth](http://www.fluidsynth.org) as output device, you can achieve this with a command like this:

 `device: MyDevice fluidSynth _useFont="PATH OF A SOUNDFONT FILE";`

 On mac, make sure that you have fluidsynth installed.
<br><br><br>

### `do`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

Such as [mod](#mod)&nbsp;`do` adds a modification to the track.

 The difference is: every `mod` command adds a further modification to a voice.

 `do` replaces the previous modification.

 #### example

 `/do: arpeggio/`
<br><br><br>

### `doOnce`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

Like [do](#do). But with the difference, that the loaded mod will be only executed once for the following event.

 #### example

 `/doOnce: arpeggio/`
<br><br><br>

### `fade`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| duration | 1 | the duration in quarters. | 0..N |
| from | 2 | The source volume value. | 0..100 |
| to | 3 | The target volume value. | 0..100 |
| curve | 4 | The fade curve type. | lin,quad,cub,quart,quint,exp |

Fades the volume over a given duration in quarters.

 #### example

 ```language=Werckmeister,type=full

 device: myDevice midi _usePort=0;
 instrumentDef: organ  myDevice _ch=0 _pc=16;
 tempo: 75;
 [
 instrument: organ;
 {
     \fff
     /fade: _from=0 _to=100 _duration=4/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="quad"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="cub"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="quart"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="quint"/
     c1 |
     /fade: _from=0 _to=100 _duration=4 _curve="exp"/
     c1 |
 }
 ]
 ```

 ![supported curve types](https://raw.githubusercontent.com/werckme/werckmeister/main/assets/curve-types.png)
<br><br><br>

### `fill`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| replace | - | the name of the template to be replaced by the fill | text |

Plays a template only once. Is also able to replace the performance of another template during its playback.

 Useful for fill ins.

 

 ```language=Werckmeister
using "chords/default.chords";
tempo: 90;
device: MyDevice  midi _usePort=0;
instrumentDef:drums    _onDevice=MyDevice  _ch=9 _pc=0;

[
type: template;
name: drums.fill;
instrument: drums;
{
  (c, & &)4 (b,, & &)4 (a,, & &)4 (g,, & &)4 |
}
]

[
type: template;
name: drums.main;
instrument: drums;
{
  r8 f#,, r & r & r & |
}
{
   r4 d,, r4 & | 
}
{
  c,,4 r & r |
}
]

[
type: accomp;
{
 /template: drums.main/
 C |
 /fill: drums.fill/ -- play fill and drum beat together
 C | C |
 /fill: drums.fill _replace="drums.main"/ -- play only the fill in
 C | C |
}
]
 ```
<br><br><br>

### `instrument`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The instrument name. | text |

Set or change the instrument of a track.

 #### examples

 **set an instrument for a track**

 ```

 [ 
 instrument: myInstrument; 
 { 
   c d e f 
 } 
 ] 
 ```

 **change current instrument within a track**

 ```

 [ 
 { 
   c d e f | 
   /instrument: myOtherInstrument/

   c d e f

 } 
 ] 
 ```
<br><br><br>

### `instrumentConf`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| for | 1 | The name of the target instrument. This is the only "unique" parameter for this command. All further parameters are specific to its related setting. | - |

With `instrumentConf` you are able to setup a specific instrument.

 Following settings can be applied:

  * volume

  * pan

  * voicing strategy

  * mod

  * velocity remap

 #### examples

 ##### mixed settings

 `instrumentConf: piano volume 100 pan 50;`

 ##### setup a mod

 `instrumentConf: piano mod myLuaMod someValue; --the `someValue` argument belongs to "myLuaMod"`

 ##### setup a velocity remap

 With `remapVelocity` you are able to change the velocity values behind `ppppp..fffff` for a specific instrument.

 In the example below the velocity for `p` will be set to 100 and the value for `f` will be set to 10.

 The value range is 0..100. (100=127 Midi velocity)

 `instrumentConf: _set=remapVelocity _p=100 _f=10;`
<br><br><br>

### `instrumentDef`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| setName | 1 | An arbitary name. | text |
| onDevice | 2 | The device which to use (The name of the device, see [device](#device)). | text |
| ch | 3 | The MIDI channel. | 0..15 |
| bankMsb | 4 | A MIDI `bank select MSB` value. | 0..127 |
| pc | 5 | A MIDI `program change` value. | 0..127 |
| bankLsb | - | A MIDI `bank select LSB` value. | 0..127 |
| cc | - | A MIDI `bank select MSB. Deprecated use bankMsb instead` value. | 0..127 |

Adds a new MIDI instrument.

 see [instrument](#instrument), [device](#device)

 #### example

 define an device, an instrument and assign it to a track.

 ```

 device: MyDevice midi 0;

 instrumentDef: organ _onDevice=MyDevice _ch=0 _pc=16; 
 [ 
 instrument: organ; 
 { 
    c d e f 
 } 
 ] 
 ```
<br><br><br>

### `instrumentSection`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| setName | 1 | An arbitary name. | text |

Layers arbitrary instruments into one.

 see [instrumentDef](#instrumentDef), [instrument](#instrument), [device](#device)

 #### example

 create 3 instruments and assign them to the section `myNewInstrument`.

 

 ```language=Werckmeister
 tempo: 120;
 device: MyDevice  midi _usePort=0;
 instrumentDef:piano    _onDevice=MyDevice  _ch=0 _pc=0;
 instrumentDef:guitar   _onDevice=MyDevice _ch=1  _pc=29;
 instrumentDef:organ   _onDevice=MyDevice _ch=2  _pc=16;
 instrumentSection: myNewInstrument piano guitar organ;
 
 [
 instrument: myNewInstrument;
 {
  a,,1 | a, | d#,~ | &
 }
 ]
 ```
<br><br><br>

### `jump`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| to | 1 | the destination marker | text |
| ignore |  | Ignores the jump N times | 0..100 |
| repeat |  | Repeats the jump N times. (A repeat value of 1 performs 2 jumps) | 0..100 |

Jumps to a previous defined mark See [mark](manual/#mark).

 #### example

 

 ```language=Werckmeister
 device: _usePort=0 _setName=MyDevice  _isType=midi;
 instrumentDef: _setName=piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;
 [
 instrument: piano;
 {
     /signature: 5 4/
     /mark: A/       -- valid & unique to this voice
     c4 d e f g |
     /jump: A/       -- jumps to A once
     a b c'2
 }
 ]
 ```
<br><br><br>

### `mark`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| name | 1 | the name of the mark | word |

adds a mark to the voice. Us it in combination with [jump](manual/#jump).

 #### examples

 

 ```language=Werckmeister
 device: _usePort=0 _setName=MyDevice  _isType=midi;
 instrumentDef: _setName=piano _onDevice=MyDevice _ch=0 _pc=1 _cc=8;
 [
 instrument: piano;
 {
     /signature: 5 4/
     /mark: A/       -- valid & unique to this voice
     c4 d e f g |
     /jump: A/       -- jumps to A once
     a b c'2
 }
 ]
 ```
<br><br><br>

### `mod`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

Adds a modification to the track. Every `mod` statement adds a further modification to the related voice.

 If you want to use ony one modification at the time use [do](#do) instead.

 #### examples

 `/mod: arpeggio/`

 Mods can be external lua scripts, or one of these internal mods:

 ##### Bend

 Performs a pitch bend from or to the pitch of a note. The value range is 0..100 where 50 means the pichbend center.

 ```language=Werckmeister,type=single,tempo=140

/doOnce: bend _from=0/ c1 | /doOnce: bend _to=0/ c1

 ```

 ##### Arpeggio

 Performs an arpeggio over a given chord.

 **Arpeggio is deprecated. Use the lua mod [myArpeggio](#myarpeggio) instead.**
<br><br><br>

### `modOnce`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the modification to load. This is the only "unique" parameter for this command. All further parameters are specific to its related modification. | text |

Like [mod](#mod). But with the difference, that the loaded mod will be only executed once for the following event.

 #### examples

 `/modOnce: arpeggio/`
<br><br><br>

### `pan`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| to | 1 | The pan value. Where 50 means the center | 0..100 |

set the pan of the current track

 #### example

 `/pan: 50/`
<br><br><br>

### `signature`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| upper | 1 | The upper value of the signature. | - |
| lower | 2 | The lower value of the signature. | - |

Set the time signature of the current track.

 #### example

 `/signature: 3 4/`
<br><br><br>

### `tempo`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| bpm | 1 | The tempo bpm value. | - |

`tempo` defines or changes the current tempo.

 #### example

 `tempo: 120;`

 It is also possible to set different tempo values for several tracks:

 [see here](/manual#tempo)
<br><br><br>

### `voicingStrategy`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| use | 1 | The name of the strategy to load. This is the only "unique" parameter for this command. All further parameters are specific to its related strategy. | text |

Adds a modification to the track.

 #### example

 `/voicingStrategy: asNotated/`

 Voicing strategies can be external lua scripts, or one of these internal strategies:

 * asNotated

 * simpleGuitar
<br><br><br>

### `volume`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| to | 1 | The volume value. | 0..100 |

set the volume of the current instrument

 #### examples

 `/volume: 50/`
<br><br><br>


## Modifications
### `drumRudiments`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| velocityFactorAccented | - | The velocity factor for accented notes (Default=1) | 0..1 |
| velocityFactorUnaccented | - | The velocity factor for unaccented notes (Default=0.7) | 0..1 |
| velocityFactorGraceNote | - | The velocity factor for accented notes (Default=1) | 0..1 |
| flamOffset | - | The offset in quartes when to perform the grace note before the actual note (Default=0.05) | 0..N |
| dragOffset | - | The offset in quartes when to perform the two grace notes before the actual note (Default=0.05) | 0..N |

#### include extension
`using "lua/mod/drumRudiments.lua";`

Implements a collection of different drum rudiments. ([see Wikipedia](https://en.wikipedia.org/wiki/Drum_rudiment))

 You can specify which notes are for `L` and `R` and performing duration.

 This will be achieved by using an chord as source material.

 The chord pitches will be used for `L` and `R`.

 The length of the chord event defines over which time the rudiment will be peformed.

 An event tag specifies which rudiment will be performed.

 For example:

 This plays one paradiddle over a length of 1 quarter

 using `sn1` for R and `sn2` for L:

 ```

 /mod: drumRudiments/
                 R     L 
 "paradiddle"@<"sn1" "sn2">4
 ```

 Examples:

 **a single paradiddle**

 ```language=Werckmeister
using "lua/mods/drumRudiments.lua";
tempo: 150;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=118;
[
instrument: piano;
{
   /mod: drumRudiments/
   -- a paradiddle (RLRR) with c' for R and c for L
   "paradiddle"@<c' c>1
}
]
 ```

 **repeating**

 ```language=Werckmeister
using "lua/mods/drumRudiments.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=118;
[
instrument: piano;
{
   /mod: drumRudiments/
   -- performs 4 paradiddles (RLRR LRLL RLRR LRLL) with c' for R and c for L
   "4x paradiddle"@<c' c>1
}
]
 ```

 **orchestration**

 If you want to orchestrate a rudiment over a drum set,

 you are able to define more R & L notes. *(The total number of the notes has to be even.)*

 ```language=Werckmeister
using "lua/mods/drumRudiments.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=118;
[
instrument: piano;
{
   /mod: drumRudiments/
   -- performs 4 paradiddles (RLRR LRLL RLRR LRLL) with 
   --      c' for R1 and c for L1
   -- and  c, for R2 and c,, for L2.
   -- The final sequence will be: c'(R1) c(L1) c,(R2) c,,(R1) ... 
   "4x paradiddle"@<c' c c, c,,>1

}
]
 ```

 **Supported Rudiments**

<ul style="font-size: 0.9em;">

<li>singleStrokeRoll</li>

<li>doubleStrokeRoll</li>

<li>trippleStrokeRoll</li>

<li>fiveStrokeRoll</li>

<li>singleStrokeFour</li>

<li>singleStrokeSeven</li>

<li>multipleBounceRoll</li>

<li>sixStrokeRoll</li>

<li>sevenStrokeRoll</li>

<li>nineStrokeRoll</li>

<li>tenStrokeRoll</li>

<li>elevenStrokeRoll</li>

<li>thirteenStrokeRoll</li>

<li>fifteenStrokeRoll</li>

<li>seventeenStrokeRoll</li>

<li>paradiddle</li>

<li>doubleParadiddle</li>

<li>trippleParadiddle</li>

<li>paradiddlediddle</li>

<li>flam</li>

<li>flamAccent</li>

<li>flamTap</li>

<li>flamacue</li>

<li>flamParadiddle</li>

<li>singleFlammedMill</li>

<li>pataflafla</li>

<li>swissArmytriplet</li>

<li>invertedFlamTap</li>

<li>flamDrag</li>

<li>singleDragTap</li>

<li>doubleDragTap</li>

<li>lesson25</li>

<li>singleDragadiddle</li>

<li>dragParadiddle1</li>

<li>dragParadiddle2</li>

<li>singleRatamacue</li>

<li>doubleRatamacue</li>

<li>trippleRatamacue</li>

</ul>
<br><br><br>

### `guitarStroke`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| direction | - | Specifies the start direction of the stroke | [up,down] |
| value | - | the duration of one arpeggio event. (Default=64) | [1,2,4,8,...] |
| mode | - | Perform only one stroke direction (normal) or alternates between up and down. (Default=normal) | [normal,alternate] |

#### include extension
`using "lua/mod/guitarStroke.lua";`

Simulates guitar strokes as mini arpeggios.

 *Note: the alternate mode works only if this mod is set via instrumentConf.*

 ```

 -- does not work: 
 [ 
 { 
   /mod: guitarStroke _mode=alternate/
 } 
 ] 
 ```

 ```

 -- does work: 
 instrumentConf: myInstrument mod guitarStroke _mode=alternate; 
 ```

 **Example**

 ```language=Werckmeister
using "lua/mods/guitarStroke.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:  piano  MyDevice  _ch=0 _pc=24;
instrumentConf: piano mod guitarStroke _mode=alternate _value=16; 
[
instrument: piano;
{
  <c e g b>4 <c e g b>4 <c eb g bb>2 |  <c eb g bb>2 <c eb g bb>2
}
]
 ```
<br><br><br>

### `legato`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| forTag | - | Specifies a tag name. If set only events with this tag name will be affected by the legato mod. | text |
| amount | - | The ammount of the legato effect | 0..100 |

#### include extension
`using "lua/mod/legato.lua";`

Performs every note legato. It is also possible to tag single notes.

 If a tag is given, only the tagged notes are performed legato.

 ```language=Werckmeister
using "lua/mods/legato.lua";
tempo: 140;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
   /mod: legato _forTag=leg _amount=100/
   -- only the `d` will be performed legato. 
   c "leg"@d e f
}
]
 ```
<br><br><br>

### `myArpeggio`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| style | - | Performs the arpeggio normal or legato | [normal,legato] |
| direction | - | the direction of the arpeggio. (Default=up) | [up,down] |

#### include extension
`using "lua/mod/myArpeggio.lua";`

A simple arpeggio implementation.

 **Example**

 ```language=Werckmeister
using "lua/mods/myArpeggio.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:  piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
 /do: myArpeggio/ 
 <c e g b>2 
 /do: myArpeggio _direction=down/ 
 <c eb g bb>2 
}
]
 ```
<br><br><br>

### `staccato`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| forTag | - | Specifies a tag name. If set only events with this tag name will be affected by the staccato mod. | text |
| amount | - | The ammount of the staccato effect | 0..100 |

#### include extension
`using "lua/mod/staccato.lua";`

Performs every note staccato. It is also possible to tag single notes.

 If a tag is given, only the tagged notes are performed staccato.

 ```language=Werckmeister
using "lua/mods/staccato.lua";
tempo: 140;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
   /mod: staccato _forTag=stac _amount=100/
   -- only the `d` will be performed staccato. 
   c "stac"@d e f
}
]
 ```
<br><br><br>

### `swing`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| grid | - | Performs the swing offset either on 8th or 16th. (Default=8) | [8, 16] |
| offset | - | The ammount of the swing offset | 0..100 |

#### include extension
`using "lua/mod/swing.lua";`

Performs a swing groove.

 ```language=Werckmeister
using "lua/mods/swing.lua";
tempo: 120;
device: MyDevice  midi 0;
instrumentDef:  piano  MyDevice  _ch=0 _pc=0;
[
instrument: piano;
{
   /mod: swing/
   c8  c   c   c   c   c   c   c

}
]
 ```
<br><br><br>


## Voicing Strategies
### `guitar`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| range | - | If given the strategy tries to keeps the range by shifting the octave of the chord. | [contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano] |

#### include extension
`using "lua/voicings/guitar.lua";`

Tries to simulate the chord voicing of an guitar.

 ```language=Werckmeister
using "lua/voicings/guitar.lua";
using "chords/default.chords";

tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
type: template;
name: myTemplate;
instrument: piano;
{
   /voicingStrategy: guitar/
   <I II III IV V VI VII>1
}
]
[
type: accomp;

{
  C | F | G | C |
}
]
 ```
<br><br><br>

### `simple`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| range | - | If given the strategy tries to keeps the range by shifting the octave of the chord. | [contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano] |

#### include extension
`using "lua/voicings/simple.lua";`

The simple approach with no strategy.

 ```language=Werckmeister
using "lua/voicings/simple.lua";
using "chords/default.chords";

tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
type: template;
name: myTemplate;
instrument: piano;
{
   /voicingStrategy: simple/
   <I II III IV V VI VII>1
}
]
[
type: accomp;

{
  C | F | G | C |
}
]
 ```
<br><br><br>

### `voicelead`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| range | - | If given the strategy tries to keeps the range by shifting the octave of the chord. | [contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano] |

#### include extension
`using "lua/voicings/voicelead.lua";`

Tries to simulates a voice leading approach:

 the octaves of a chord are rearranged, so that the actual pitch distance between the previous chord is kept minimal.

 ```language=Werckmeister
using "lua/voicings/voicelead.lua";
using "chords/default.chords";

tempo: 120;
device: MyDevice  midi 0;
instrumentDef:piano  MyDevice  _ch=0 _pc=0;
[
type: template;
name: myTemplate;
instrument: piano;
{
   /voicingStrategy: voicelead/
   <I II III IV V VI VII>1
}
]
[
type: accomp;

{
  C | F | G | C |
}
]
 ```
<br><br><br>


## Conductor Selectors
### `all`

Selects all events.

 ```
 all() {...}
 ```
<br><br><br>

### `channel`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| channel | - |  | midi channel+ |

Selects an event where its channel is equal to one the given numbers.

 ```

 channel(0 1 2) {...}

 ```
<br><br><br>

### `chord`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| chord name | - |  | string+ |

Selects a MIDI event which was created during rendering a specific chord.

 ```
 chord(Cmaj7) {...}
 ```

 It is also possible to select "chord qualities". For example, if you want to select

 every 7th chord you write:

 ```
 chord(X7) {...}
 ```
<br><br><br>

### `degree`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| degree | - |  | degree+ |

Selects a MIDI event which was created during rendering a template and its degrees.

     Octaves of a degree will not be considered.

 ```

 degree(I III) {...}

 ```
<br><br><br>

### `expression`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| expression | - |  | expression+ |

Select events via its applied expressions.

 ```

 expression(p) {...}

 expression(p f ff) {...}

 ```
<br><br><br>

### `fromBar`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| bar | - |  | bar number |

Selects an event where its bar time is equal or greater.

 ```

 fromBar(1) {...}

 ```

 see also: [toBar](#toBar), [onBar](#onBar), [nthBar](#nthBar).
<br><br><br>

### `fromBeat`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | quarters |

Selects an event where its beat time is equal or greater.

 ```

 fromBeat(1) {...}

 ```
<br><br><br>

### `fromPitch`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| pitch | - |  | pitch |

Selects any event where its pitch is higher or equal than the given pitch.

 see also: [toPitch](#toPitch), [pitch](#pitch).

 ```

 fromPitch(c,) {...}

 ```
<br><br><br>

### `fromPosition`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| position | - |  | quarters|cuename |

Selects any event where its position is equal or after a given position.

 see also: [toPosition](#toPosition).

 #### example, select the events postion >=8 quarters:

 ```
 fromPosition(8) {...}
 ```
 #### using cue markers instead of quarter numbers

 ```
 c d e f | /cue: myMark/ g a b 'c
 ```
 ```
 fromPosition(@myMark) {...}
 ```
 > In order to work properly, a cue name must occur only once in a document.

 > Please mind the [cue](#cue) behaviour in combination with repeat and jumps.
<br><br><br>

### `instrumentSelector`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| instrument name | - |  | instrumentName+ |

Selects any event which belongs to one of the given instruments.

 *there is a known [issue](https://github.com/werckme/werckmeister/issues/230) related to instrumentSection*

 #### example:

 ```

 instrument(bass) {...}
 instrument(bass drums) {...}
 ```
<br><br><br>

### `notOnBeat`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | quarters+ |

Selects an event which does not appear on a given beat or a list of beats

 ```

 notOnBeat(1 3) {...}

 ```
<br><br><br>

### `nthBar`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | bar number+ |

Selects an event which is located within in one of the give n-th bar.
 > Heads Up:  The nth bar is not affected by any other selector.
 > Here for example: `fromBar(1) nthBar(2) ...`, the nth bar selector
 will consider its decision counting from bar 0, not from the selected bar 1.
 ```

 nthBar(1 3 5) {...}

 ```

 see also: [fromBar](#fromBar), [toBar](#toBar), [onBar](#onBar).
<br><br><br>

### `octave`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| an octave number from 0 to 10 | - |  | number+ |

Selects any event where its pitch is in the range of a given octave. The middle c is located in octave 5.

 ```

 octave(4) {...}

 ```
<br><br><br>

### `onBar`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | bar number+ |

Selects an event on a given bar number or a list of bar numbers

 ```

 onBar(1 3) {...}

 ```

 see also: [fromBar](#fromBar), [toBar](#toBar), [nthBar](#nthBar).
<br><br><br>

### `onBeat`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | quarters+ |

Selects an event which appears on a given beat or a list of beats

 see also: [fromBeat](#fromBeat), [toBeat](#toBeat).

 #### example, select the events at beat 1 and 3:

 ```

 onBeat(1 3) {...}

 ```
<br><br><br>

### `pitch`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| pitch | - |  | pitch+ |

Selects any event where its pitch is equal to one of the given pitches.

 see also: [fromPitch](#fromPitch), [toPitch](#toPitch).

 ```

 pitch(c) {...}

 pitch(c, c c') {...}

 ```
<br><br><br>

### `toBar`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| bar | - |  | bar number |

Selects an event where its bar time is equal or less.

 ```

 toBar(4) {...}

 ```

 see also: [fromBar](#toBar), [onBar](#onBar), [nthBar](#nthBar).
<br><br><br>

### `toBeat`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | quarters |

Selects an event where its beat time is equal or less.

 see also: [fromBeat](#fromBeat), [onBeat](#onBeat).

 ```

 toBeat(4) {...}

 ```
<br><br><br>

### `toPitch`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| pitch | - |  | pitch |

Selects any event where its pitch is lower or equal than the given pitch.

 see also: [fromPitch](#fromPitch), [pitch](#pitch).

 ```

 toPitch(c,) {...}

 ```
<br><br><br>

### `toPosition`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| beat | - |  | quarters |

Selects any event where its position is before a given position.

 see also: [fromPosition](#fromPosition).

 #### example, select the events position < 8 quarters:

 ```
 toPosition(8) {...}
 ```
 #### using cue markers instead of quarter numbers

 ```
 c d e f | /cue: myMark/ g a b 'c
 ```
 ```
 toPosition(@myMark) {...}
 ```
 > In order to work properly, a cue name must occur only once in a document.

 > Please mind the [cue](#cue) behaviour in combination with repeat and jumps.
<br><br><br>

### `withTag`

#### parameters
| name | position | description | type |
|:--- |:--- |:--- |:--- |
| tag | - |  | text |

Selects any event which has a tag assigned matching to the given tag.

 #### example, select the event with the tag "myTag" assigned:

 ```
 withTag(myTag) {...}
 ```
 ```
 c d e "myTag"@f |
 ```
<br><br><br>


## Conductor Declarations
### `duration`

#### Value Types
| name | description | type |
|:--- |:--- |:--- |
| absolute |  | 0..N |
| percent | percent base value is the events length | 0..N% |

Changes the duration of an event.

 #### example:

 ```

 onBeat(1) {

   duration += 0.125; -- 1/8 longer

 }

 onBeat(3) {

   duration -= 25%;

 }

 ```
<br><br><br>

### `pitch`

#### Value Types
| name | description | type |
|:--- |:--- |:--- |
| absolute |  | 0..127 |
| percent | percent base value is events pitch | 0%..100% |

Changes the pitch of an event.

 #### example:

 ```

 onBeat(1) {

   pitch = 60;

 }

 onBeat(3) {

   pitch += 25%;

 }

 ```
<br><br><br>

### `timeOffset`

#### Value Types
| name | description | type |
|:--- |:--- |:--- |
| absolute |  | 0..N |
| percent | percent base value is the events length | 0%..N% |

Changes the time position of an event.

 **Since this is an offset value, the set operation(=) has the same effect as the add(+=) or substract(-=) operation.**

 #### example:

 ```

 onBeat(1) {

   timeOffset += 0.125; -- 1/8 offset

 }

 onBeat(3) {

   timeOffset -= 25%;

 }

 ```
<br><br><br>

### `velocity`

#### Value Types
| name | description | type |
|:--- |:--- |:--- |
| absolute |  | 0..127 |
| percent | percent base value is events velocity | 0%..100% |

Changes the velocity of an event.

 #### example:

 ```

 onBeat(1) {

   velocity = 127;

 }

 onBeat(3) {

   velocity += 25%;

 }

 ```
<br><br><br>




Visual Studio Code Extension
======

## Features

* MIDI Playback from source
* Playback visualization
* Syntax highlighting
* Error & Warning highlighting
* Start playback at random position

**Playback visualization**

<img src="https://raw.githubusercontent.com/werckme/werckmeister-codext/master/assets/features/playbackvis.gif">


## Installation

At first you need to install [Visual Studio Code](https://code.visualstudio.com/). You should find a version for MacOs, Windows and Linux.
Is the installation done, open Visual Studio Code. On the left sidebar you find a button which opens the extensions dialog.
Open it and use the search input to find the Werckmeister extension. Select it and hit install.


Of course, you need also an installed Werckmeister compiler on your machine.
With that Werckmeister installation, the Werckmeister compiler should be accessable system wide. So normally you have to do nothing.
If Visual Studio Code is not able to execute the Werckmeister compiler you can set the correct installation path via the extension setup:

*Preferences -> Settings -> Extensions -> Werckmeister Binary Directory*

<img src="https://raw.githubusercontent.com/werckme/werckmeister-codext/master/assets/pathsetup.png">
