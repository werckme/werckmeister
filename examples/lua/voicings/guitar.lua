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
require "com"

parameters = {
    table.unpack(SolverDefaultParameter)
}

-- https://www.8notes.com 
-- https://www.oolimo.de/gitarrenakkorde/find
-- https://www.scales-chords.com/chord/piano/
VoicingMatrix = {
  --  ["1234567/bass"] = {degrees},
    ["1010100"] = {I, V, I, III, I},            -- base
    ["1100100"] = {I, V, I, II },               -- sus2
    ["1001100"] = {I, V, I, IV, V },            -- sus4
    ["1001101"] = {I, V, VII, IV, V},           -- 7sus4
    ["1101101"] = {I, IV, VII, II, V},          -- 9sus4
    ["1010110"] = {I, VI, III, V},              -- 6
    ["1110110"] = {I, VI, III, V, II},          -- 6/9
    ["1010101"] = {I, V, VII, III, I},          -- 7
    ["1110101"] = {I, III, VII, II},            -- 9
    ["1110100"] = {I, V, II, III},              -- add9
    ["1011101"] = {I, IV, VII, III},            -- 11
    ["1011001"] = {I, VII, III, IV},            -- 7#11
    ["1010111"] = {I, V, VII, III, VI},         -- 13
    ["1010101/7"] = {VII, V, I, III},           -- /7
    ["1010100/5"] = {V, I, III, V},             -- /5
    ["1011100/4"] = {IV, V, I, III},            -- /4
    ["1010100/3"] = {III, V, I, III},           -- /3
    ["1110100/2"] = {II, V, I, III},            -- /2
}


-- returns the degrees defined for a chord as a bit string.
-- e.g.: Xmaj's degrees are defined as I III V
--                1234567
-- as bitstring = 1010100
-- if chord is a slash chord, the degree value will be appended as number.
-- e.g.: X/V = 1010100/5
function ChordToHashString(chord)
    local bits = {}
    local slashAppendings = {}
    local degrees = chord.degrees
    for i=1, 7 do
        if degrees[i] == nil or chord:isDegreeAdjunct(i) then
            bits[i] = 0
        else
            bits[i] = 1
            if chord.degrees[i] < 0 then
                table.insert(slashAppendings, i)
            end
        end
    end
    if #slashAppendings == 0 then
        return table.concat(bits)
    end
    return table.concat(bits) .. "/" .. table.concat(slashAppendings)
end


function solve(chord, pitches, params, timeinfo)
    dump(pitches)
    local strDegrees = ChordToHashString(chord)
    local voicing = VoicingMatrix[strDegrees]
    local result = {}
    local octave = -1
    local lastDegree = -1
    for idx, degree in pairs(voicing) do
        if degree <= lastDegree then 
            octave = octave + 1
        end
        lastDegree = degree
        table.insert(table, {
            degree = degree,
            octave = octave,
            pitch = 1
        });
    end
    return result
end