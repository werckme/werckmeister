-- <command name="guitar" where="voicingStrategy" using="lua/voicings/guitar.lua">
-- <![CDATA[
-- Tries to simulate the chord voicing of an guitar.
-- ```language=Werckmeister\n
--using "lua/voicings/guitar.lua";\n
--using "chords/default.chords";
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--type: template;\n
--name: myTemplate;\n
--instrument: piano;\n
--{\n
--   /voicingStrategy: guitar/\n
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
require "solvers/guitar"

parameters = {
    table.unpack(ASolverDefaultParameter)
}

local solver = GuitarSolver:new()

function solve(chord, degrees, params, timeinfo)
    return solver:solve(chord, degrees, params)
end