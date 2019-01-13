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

## pitch aliases
```
-- file defaultMidiDrumMap.pitchmap
"bd": c,,
"sn": e, 
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

-- instrument definitions
--           uname ch cc pc
/instrument: bass 3 3 87/
/instrument: drums 10 0 17/

-- definitions with the same name will create unisono events
/instrument: piano 0 0 0/
/instrument: pinao 3 3 3/

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

## Voicing Strategy
#Direct
#SimpleGuitar
 Voicing I V I III: Basis
 	 I V VII III: 7
	 I III VII II V: 9
	 I IV VII II V: 11
	 I IV VII III VI: 13

 Min/Max Octavraum
 Keine Prüfung ob wirklich spielbar

## Meta Commands
# expressions

\fff --set all to fff
!fff --set only next note to fff

/dynamicsTarget: volume/
/dynamicsRemap: ffff 127/
/dynamicsRemap: ff 57/

 c,,4\ffff

/dynamicsTarget: velocity/
 c,,4\ffff

# channel
/channelVolume: 10 127/

#rhythmik
/upbeat: 8/ -- auftakt 1 8tel

## TODO:
andere Taktarten als 4/4

## TODO: weitere Lilypond Ausdrücke

Arpeggios

# Dynamik
Absolute Dynamikbezeichnung wird mit Befehlen nach den Noten angezeigt, etwa c4\ff. Die vordefinierten Befehle lauten: \ppppp, \pppp, \ppp, \pp, \p, \mp, \mf, \f, \ff, \fff, \ffff, fffff, \fp, \sf, \sff, \sp, \spp, \sfz, and \rfz. 

# Crescendo
Eine Crescendo-Klammer wird mit dem Befehl \< begonnen und mit \!, einem absoluten Dynamikbefehl oder einer weiteren Crescendo- oder Decrescendo-Klammer beendet. Ein Decrescendo beginnt mit \> und wird auch beendet mit \!, einem absoluten Dynamikbefehl oder einem weiteren Crescendo oder Decrescendo. \cr und \decr können anstelle von \< und \> benutzt werden.

# Espressivo
Der \espressivo-Befehl kann eingesetzt werden, um crescendo und decrescendo für die selbe Note anzuzeigen


## next features

### device config:
  ```
  -- device: name type [arg1 ... argn]
  /device: orch1 vst2 "c:\plugins\gpo.dll" "./violins.preset"/
  /device: roland1 midi 0 / -- 0 is the device number, as displayed with 'sheetp --list'

  /instrument: piano orch1 nChanel nCC nPC/

  ```


## Nice to have
- setStyle rename to switchStyle
- /setStyle: samba normal [startAtBar 3]/ 
- device vst support
- Lyrics:
```
  /mode: lyrics/
  "easy"4 "easy" "easy meat"2 |
```
  - compiler baut midi+srt file

- soundselect generic midisequences
  /soundSelect: cc 0 pc 1/
### Paralleles abspielen verschiedener Styles

{
  /style: disco normal/
  /exclude: all/
  /include: bass/
  C | C | E |
}
{
  /style: latin normal/
  /include: all/
  /exclude: bass/
  C | C | E |
}


## Bugs
 - purdie fail: (08e0b66f9f37a3e8704212108cc596615dc7d75f)
  C7#9
  /style: purdie ende/
  C7#9 | 
  C07  |
 - keine möglichkeit meta commands für style instruments zu überschreiben (voicing strategy)
 - Akkordnotationen mit r führen dazu dass keine Styles gespielt werden: r1 | C |
 - (falsch gesetzter)Haltebogen stoppt wiedergabe(c4 hat nicht die selbe oktave): r4 e f g | c'1~ | c4 d e f |
 - style rendering kommt ins stolpern wenn ungerade teiler z.b: 4/4 | C C C |  
 - style voice nur meta events -> bleibt hängen
 - sheet hat Länge 0 wenn kein Chordtrack vorhanden
 - sheet hat Länge 0 sobald reihenfolge tacks, chords nicht eingehalten
 - sheet hat Länge 0 oder ist fehlerhaft wenn erste zeile kein Kommentar ist
 - die Akkorde As und Es werden nicht erkannt. Workaround: Aes Ees
 - style-section switch legt neue tracks an, anstatt bestehende zu nutzen 
 - meta command style lädt keine files mit sonderzeichen
 - Degrees die in Akkorden nicht vorkommen konsumieren keine Zeit: C -> II4 II II II |

## Cool DAW-Editor
all character based 
sophisticated highligting such as position or markers
