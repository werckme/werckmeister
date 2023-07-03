-- <command name="inversion" where="voicingStrategy" using="lua/voicings/inversion.lua">
-- <![CDATA[
-- creates an inversion by moving the lowest note an octave hihgher.
-- ```language=Werckmeister\n
--using "/lua/voicings/inversion.lua";\n
--using "/chords/default.chords";
--tempo: 120;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--type: template;\n
--name: myTemplate;\n
--instrument: piano;\n
--{\n
--   /voicingStrategy: inversion/\n
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
-- <param name="level" optional="1" type="number">level of inversion</param>
-- <param name="range" optional="1" type="[contrabass,bass,baritone,tenor,alto,mezzosoprano,soprano]">If given the strategy tries to keeps the range by shifting the octave of the chord.</param>

require "lua/com/com"
require "com"

parameters = {
    { name="level", default=1 },
    table.unpack(SolverDefaultParameter)
}

function solve(chord, pitches, parameters)
    CheckForLegacyParameters(parameters)
    local inversionLevel = tonumber(parameters.level)
    if inversionLevel == nil then
        inversionLevel = 1
    end
    for _, pitch in pairs(pitches) do
        pitch.octave = pitch.octave + 1
        inversionLevel = inversionLevel - 1
        if inversionLevel == 0 then
            break
        end
    end
    KeepRange(pitches, parameters.range)
    return pitches
end