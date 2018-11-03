#Eine Scriptsprache für Sheetnotation

 - ChordDefs: Definiere Akkorde
 - Styles: Rhythmen und Begleitung notiert Relativ zu Tonleiterstufen
 - Sheet: Leadsheet-Dokument verwendet unter Angabe styles, Akkorden und Melodien

#Beispiele
## ChordDefs

```
Xmaj: 1 5 8
X7: 1 5 8 10
Xmaj7: 1 5 8 11
X/V: -6 1 5 8 --quinte im bass
```

## Styles
```
-- file: simplePianoStyle.style

section intro -- section
[ -- track 1 begin
  /name: piano / -- meta informationen
  /soundselect: 0 0/
  /channel: 1/
  { I4 II4 III4 IV4 | } -- a separate voice / similar to lilypond
  { I,4 II,4 III,4 IV,4 } 
] -- track 1 end
[
  /name: bass /
  /soundselect: 0 0/
  /channel: 2/
   I4 I'4 III4 I'4
]
end

section normal
[ -- track 1 begin
  /name: piano / -- meta informationen
  /soundselect: 0 0/
  /channel: 1/
  /signature: 4|4/
  { I4 II4 III4 IV4 | } -- a separate voice / similar to lilypond
  { I8 <III V VII>2.. |}    
] -- track 1 end
[
   /name: bass /
   I4 I'4 III4 I'4
]
end

```

## Sheet

```
-- document configs
@load 'Chords1.chdef';
@load 'simplePianoStyle.style';

[
{ 
  /soundselect: 0 0/ 
  /channel: 1/
  c4 d4 e4 f4 | c4 d4 e4 f4 | 
}
{ 
  f4 f4 f4 f4 | h4 h4 h4 h4 | 
}
] 
-- the sheet, no voices here
/style: simplePianoStyle:intro/
/voicingStrategy: asNotated/
Cmaj | Cmaj C7 |


```

