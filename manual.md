Werckmeister Manual
===================

![the editor](https://raw.githubusercontent.com/SambaGodschynski/werckmeister-editor/master/assets/sheet-example.png)

Werckmeister is a language designed to write sheet music.

Vision
======

## What it wants to be
* a fast and easy way to create sheet music
* that tool in your toolchain, between the initial idea and the details work with your favourite Sequencer/DAW
* a sequencer which can be used without a Graphical User Interface
* a tool which relies on human readable files


## What it doesn't want to be
* a replacement for your favourite Sequencer/DAW
* a score writer
* a programming language


Manual
======
I want to ...
--------------
[... setup a piece](Setup-A-Piece)

[... write melodies](#Write-Melodies)

[... use different instruments]()

[... accomp my melodies]()

[... define my own accompanying styles (templates)](#Define-Accompanying-Styles-(templates))

[... use MIDI expression mods]()

[... write my own MIDI expression mods]()

### Basics
#### Tracks
#### Voices
#### Meta Events

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


![This template playes the Ist IIIrd Vth and the VIIth degree as quarter note. It will be played two bars. Starting with a C7 followed by a C minor 7 chord](https://raw.githubusercontent.com/SambaGodschynski/werckmeister-editor/master/assets/example2.gif)
<br>*This template playes the Ist, IIIrd, Vth and the VIIth degree as quarter note. It will be used for two bars. Starting with a C7 followed by a C minor 7 chord.*