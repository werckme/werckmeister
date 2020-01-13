Werckmeister Manual
===================

Vision
------
Consider the beauty of lead sheet music: you write a melody and some chords. Thats it.
<br>Later you can tell your band (or whoever is crazy enough to play your music) in which style the piece has to be played. You can say, after 4 beats play it as a bossa nova or a blues or whatever.

This is the idea behind Werckmeister. Of course it can not replace a band or a musician but it offers you a tool to write melodies and chords then you can define how to interpret the chords along your melody or even define your own interpretation templates.

Why text only and no GUI
------------------------
### Don't be afraid, it's just text
The motivation behind this decision cames with an experience which I had in the past with a professional scorewriter software I was using back in the early 2000.

This software was a bit clunky but I've got used to it. At some point the developer discontinued the support for my version and I've got more and more problems to get the software running on a modern system. The other problem was, that my project files were written in a proprietary format. So I realized: **not I am the owner of my created music, the company behind the scorewriter software owns it
 and if I want to continue using my projects I have to pay money.**
<br>So I've came to the conclusion, that readable text files are the way to go.

*What is so hard writing notes in a text edior such as `c d e f g` in comparison to an overloaded clunky editor software.
<br>(actually notes in a score have a lot advantages in comparison to a text file regarding readability etc., but that aside)*

I've discovered that [LilyPond](http://lilypond.org/) is a good way to write music using textfiles.
But for me there was a problem: LilyPond's main purpose is make score visible. So the LilyPond language is designed to describe how notes have to look on paper in the first place.
<br>Basically, I wanted the same but with the focus on how the notes sounds played by MIDI instruments.




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
I am a composer and I want to ...
--------------
[... setup a piece](Setup-A-Piece)

[... write melodies](#Write-Melodies)

[... use different instruments]()

[... accomp my melodies]()

[... define my own accompanying styles (templates)](#Define-Accompanying-Styles-(templates))

[... use MIDI expression mods]()

[... write my own MIDI expression mods]()

### Basics
The Werckmeister language knows only a few statement types. It's all about tracks, voices and events. All others have just a cofiguration purpose.
<br>A basic Werckmeister document will look like this:

```
-- document config statements:
using "lua/mods/staccato.lua";
device: SC1 midi 1;
tempo: 130;
instrumentDef: piano   SC1 2 0 0;

-- a track containing one voice with events
[
instrument: piano;
{
    /mod: staccato/ -- a meta event
    c4 d e f | g1 --note events
}
]
```
#### Tracks
#### Voices
#### Events
##### Note Events
##### Meta Events
##### Event Tags

### Setup A Piece
#### Tempo
```
tempo 120;
[
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

### Write Melodies
#### Notes
The syntax is loosely inspired by the [lilypond](http://lilypond.org) notaion syntax. A note is just a letter followed by a number for its duration. If a note has no duration, the last given duration will be used.

`c4 d e8 f`
<br>*c and d will be played as quarter, e and f as eight notes*

the end of a bar is marked by a pipe: `|`

`c d e f | g a b c'`
<br>*a c major scale, played in two bars*

#### Accidentals
`c# d# f# g# | bb db' eb' gb'`

#### Durations
`c2 c4 c8 c16 c32 c64 c128 r`

#### Octaves
`c,,  e,,  g,, | c, e, g, | c e g | c' e' g' | c'' e'' g''`

#### Intervals
`<g,, c e g bb>4 <c, c eb g bb>2.`

#### Augmentation Dots & Ties
`c2. c4 | c2~ c4 c`

#### Tuplets
`(c8 c c)4   (c8 c c c c)4  (c8 c c c c c c)4  (c8 c c c c c c c c)4`


### Define Accompanying Styles (templates)
Instead of writing absolute notes, you use relative degrees. For example `I4` means the first degree as a quarter note.


![This template playes the Ist IIIrd Vth and the VIIth degree as quarter note. It will be played two bars. Starting with a C7 followed by a C minor 7 chord](https://raw.githubusercontent.com/SambaGodschynski/werckmeister/master/assets/example2.gif)
<br>*This template playes the Ist, IIIrd, Vth and the VIIth degree as quarter note. It will be used for two bars. Starting with a C7 followed by a C minor 7 chord.*
