-- <command name="voicelead" where="voicingStrategy" using="lua/voicings/voicelead.lua">
-- <![CDATA[
-- Tries to simulates a voice leading approach:
-- the octaves of a chord are rearranged, so that the actual pitch distance between the previous chord is kept minimal.
-- ```language=Werckmeister\n
--using "lua/voicings/voicelead.lua";\n
--using "chords/default.chords";
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--type: template;\n
--name: myTemplate;\n
--instrument: piano;\n
--{\n
--   /voicingStrategy: voicelead/\n
--   <I II III IV V VI VII>1\n
--}\n
--]\n
--[\n
--type: accomp;
--{\n
--  C | F | G | C |\n
--}\n
--]\n
-- ```
-- ]]>
-- </command>
-- <param name="range" optional="1" type="[contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano]">If given the strategy tries to keeps the range by shifting the octave of the chord.</param>

require "lua/com/com"
require "lua/com/globals"
require "solvers/voicelead"

parameters = {
    table.unpack(ASolverDefaultParameter)
}

local solver = VoiceLeadSolver:new()

function solve(chord, degrees, params)
    return solver:solve(chord, degrees, params)
end