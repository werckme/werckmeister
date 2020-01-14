<img src="https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/Logo_med.png" style="display: block; margin: auto">

Werckmeister Manual
===================

Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords. Thats it.
<br>Later you can tell your band (or whoever is crazy enough to play your music) in which style the piece has to be played. You can say, after 4 beats play it as a bossa nova or a blues or whatever.

This is the idea behind Werckmeister. Of course it can not replace a band or a musician but it offers you a tool to write melodies and chords then you can define how to interpret the chords along your melody or even define your own interpretation templates.

Why text only and no GUI
------------------------
The motivation behind this decision cames with an experience which I had in the past with a professional scorewriter software I was using back in the early 2000.

This software was a bit clunky but I've got used to it. At some point the developer discontinued the support for my version and I've got more and more problems to get the software running on a modern system. The other problem was, that my project files were written in a proprietary format. 

So I realized: 
<br>**not I am the owner of my written music, the company behind the scorewriter software owns it
 and if I want to continue using my projects I have to pay money.**

So I've came to the conclusion, that readable text files is my way to go.

*What is so hard writing notes in a text edior such as `c d e f g` in comparison to an overloaded clunky editor software.
<br>(actually notes in a score have a lot advantages in comparison to a text file regarding readability etc., but that aside)*

I've discovered that [LilyPond](http://lilypond.org/) is a good way to write music using textfiles.
But for me there was a problem: LilyPond's main purpose is make score visible. So the LilyPond language is designed to describe how notes have to look on paper in the first place. Basically, I wanted the same but with the focus on how the notes sounds played by MIDI instruments.




## What it wants to be
* a fast and easy way to create lead sheet music
* that tool in your toolchain, between the initial idea of an piece and developing the details in your favourite Sequencer/DAW
* a way to create music without a Graphical User Interface
* a tool which relies only on human readable files


## What it doesn't want to be
* a replacement to your favourite Sequencer/DAW
* a scorewriter software
* a programming language


Manual
======
I want to ...
--------------
[... get an impression of what I have to face if I want to learn Werckmeister](#Don't-be-afraid,-it's-just-text)

[... setup a piece](#Setup-A-Piece)

[... write melodies](#Write-Melodies)

[... use different instruments]()

[... accomp my melodies]()

[... define my own accompanying styles (templates)](#Define-Accompanying-Styles-(templates))

[... use MIDI expression mods]()

[... write my own MIDI expression mods]()

## Don't be afraid, it's just text
The Werckmeister language knows only a few statement types. It's all about tracks, voices and events. That's it.
<br>The remaining statment types are just for cofiguration purpose.

A basic Werckmeister document will look like this:

```
-- CONFIG:
-- 1) configure your MIDI device
-- 2) define an instrument with the name piano
-- 3) set the tempo

-- 1)
--      name         type portnumber
device: MyMidiDevice midi 1;

-- 2)
--             name  device       ch pc cc  => (channel, programm change, control change)
instrumentDef: piano MyMidiDevice 1  0  0;

-- 3)
tempo: 130;

-- TRACKS, VOICES & EVENTS
-- a track containing one voice with some note events
-- the track is set up to use our piano instrument
[
instrument: piano;
{
    c4 d e f | g1 --note events
}
]
```
#### Tracks and Voices

A track in Werckmeister has excactly the same meanig as it has in a MIDI file.
It is related to an instrument and contains events.

A track begins with a `[` and ends with a `]`.
```
-- a track with one voice and an instrument setup

[
instrument: piano;
{
    c d e f | g a b c'
}
]
```

All events of a track are releated at least to one voice. A voice starts with `{` and ends with `}`.
If you want to write polyphonic melody lines, you can add more voices to your track. 



![a polyphonic melody](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/poly.png)

The melody above would be written like this:

```
-- a track with two voices

[
instrument: piano;
{
    g4 f#8 g a4 g
}
{
    d4 d d d
}
]

```

#### Events
tbd.
##### Note Events
tbd.
##### Meta Events
tbd.

## Setup A Piece
tbd.
#### Devices & Instruments
tbd.
#### Tempo
tbd.
```
tempo 120;
[
{
    c d e f | g a b c'
}
]

```

```
tempo 120;
[
{
    /tempo: double/
    c d e f | g a b c'
}
{
    /signature: 3 4/
    /tempo: 96/
    c e f | g b c'
}
{
    c d e f | g a b c'
}
]

```

#### Time Signatures
The time signature 4/4 by default.
If you want to setup a different time signature than the default, you can achieve this by using the signature [meta event](#Meta-Events).
With this you are able to setup different time signatures during a piece.
```
[
{
    /signature: 3 4/
    c e e | c e e |
    /signature: 4 4/
    c d e f | g a b c'
}
]
```

#### Key Signatures
key signatures are not supported.

## Write Melodies
#### Notes
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notaion syntax. A note is just a letter followed by a number for its duration. If a note has no duration, the last given duration will be used.

```
c4 d e8 f
```

<br>*c and d will be played as quarter, e and f as eight notes*

the end of a bar is marked by a pipe: `|`

```
c d e f | g a b c'
```
<br>*a c major scale, played in two bars*

#### Accidentals
tbd.

```
c# d# f# g# | bb db' eb' gb'
```

#### Durations
tbd.

```
c2 c4 c8 c16 c32 c64 c128 r
```

#### Octaves
tbd.

```
c,,  e,,  g,, | c, e, g, | c e g | c' e' g' | c'' e'' g''
```

#### Intervals and Chords

tbd.

*insert image!*

```
<g,, c e g bb>4 <c, c eb g bb>2.
```

#### Augmentation Dots & Ties
tbd.

*insert image!*

```
c2. c4 | c2~ c4 c
```

#### Tuplets
tbd.

*insert image!*

```
(c8 c c)4   (c8 c c c c)4  (c8 c c c c c c)4  (c8 c c c c c c c c)4
```

##### Nested Tuplets

You are also able to write nested tuplets, as in the example below.

![a example of nested tuplets](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/blackpages.png)

```
b2 ((b c#' c#' b d')  (f' a a bb c')  (e' a' bb' f' c' g))2
```

#### Pitchmaps

tbd.


## Define Accompanying Styles (templates)
tbd.

Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note.


![This template playes the Ist IIIrd Vth and the VIIth degree as quarter note. It will be played two bars. Starting with a C7 followed by a C minor 7 chord](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/example2.gif)
<br>*This template playes the Ist, IIIrd, Vth and the VIIth degree as quarter note. It will be used for two bars. Starting with a C7 followed by a C minor 7 chord.*

## Advanced techniques
##### Event Tags
##### Lua scripts