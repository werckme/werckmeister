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
X/V: -6 1 5 8 --quinte im bass
```

## Styles

## Sheet

```
-- document configs
@using 'Chords1.chdef';
@using 'simplePianoStyle.style';

[ -- track 1 begin
  /soundselect: 0 0/ -- meta informationen
  /channel: 1/
  c4 d4 e4 f4 | c4 d4 e4 f4 | -- notation ähnlich lilypond
] -- track 1 end
[
  /style: style1/
  Cmaj | Cmaj C7 |
]

```

