require "lua/voicings/solvers/asolver"
require "lua/com/globals"

-- https://www.8notes.com 
-- https://www.oolimo.de/gitarrenakkorde/find
-- https://www.scales-chords.com/chord/piano/
VoicingMatrix = {
    ["1234567/bass"] = nil,
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
function chordToHashString(chord)
    bits = {}
    slashAppendings = {}
    local degree = chord.degrees
    for i=1, 7 do
        if degree[i] == nil then
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

GuitarSolver = ASolver:new()

function GuitarSolver:degreeToPitch(degreeDef, chord)
    return chord.rootPitch + (degreeDef.degreeValue % 12)
end

function GuitarSolver:_solveImpl(chord, degrees, args)
    
    local d = function(degreeValue) return self:getDefaultDegreeDef(degreeValue, degrees) end
    local strDegrees = chordToHashString(chord)
    local voicing = VoicingMatrix[strDegrees]
    if voicing == nil then
        error("no voicing found defined for " .. chord.strBase .. chord.strOptions .. " " .. strDegrees)
    end
    local result = {}
    local octave = -1
    local lastDegree = -1
    for idx, degree in pairs(voicing) do
        if degree <= lastDegree then 
            octave = octave + 1
        end
        lastDegree = degree
        result[idx] = self:createPitch(chord, d(degree  ), octave)
    end
    return result
end