#Eine Scriptsprache für Sheetnotation

 - ChordDefs: Definiere Akkorde
 - Styles: Rhythmen und Begleitung notiert Relativ zu Tonleiterstufen
 - Sheet: Leadsheet-Dokument verwendet unter Angabe styles, Akkorden und Melodien

#Beispiel
## ChordDefs

```
Xmaj: 1 5 8
X7: 1 5 8 10
Xmaj7: 1 5 8 11
```

## Styles

## Sheet

```
@using Chords1.def
@using simplePianoStyle.style

@voice: voice1 0 0 -- name, program, sound select

[ -- track 1 begin
  (voice: voice1) -- meta informationen
  c4 d4 e4 f4 | c4 d4 e4 f4 | -- notation ähnlich lilypond
] -- track 1 end
[
  (style: style1)
  Cmaj | Cmaj C7 |
]

```

