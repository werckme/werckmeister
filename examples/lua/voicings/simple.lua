-- <command name="simple" where="voicingStrategy" using="lua/voicings/simple.lua">
-- <![CDATA[
-- The simple approach with no strategy.
-- ```language=Werckmeister\n
--using "/lua/voicings/simple.lua";\n
--using "/chords/default.chords";
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--type: template;\n
--name: myTemplate;\n
--instrument: piano;\n
--{\n
--   /voicingStrategy: simple/\n
--   <I II III IV V VI VII>1\n
--}\n
--]\n
--[\n
--type: accomp;
--{\n
--  /template: myTemplate/
--  C | F | G | C |\n
--}\n
--]\n
-- ```
-- ]]>
-- </command>
-- <param name="range" optional="1" type="[contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano]">If given the strategy tries to keeps the range by shifting the octave of the chord.</param>

require "lua/com/com"
require "com"

parameters = {
    table.unpack(SolverDefaultParameter)
}

function solve(chord, pitches, parameters)
    CheckForLegacyParameters(parameters)
    KeepRange(pitches, parameters.range)
    return pitches
end