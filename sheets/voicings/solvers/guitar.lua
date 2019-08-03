require "solvers/asolver"
require "com/globals"

-- https://www.8notes.com 
-- https://www.oolimo.de/gitarrenakkorde/find
-- https://www.scales-chords.com/chord/piano/
VoicingMatrix = {
    ["1234567"] = nil,
    ["1010100"] = {I, V, I, III, I},            -- base
    ["1010101"] = {I, V, VII, III, I},          -- 7
    ["1110101"] = {I, III, VII, II },           -- 9
    ["1110100"] = {I, V, II, III },           -- add9
}


-- returns the degrees defined for a chord as a bit string.
-- e.g.: Xmaj's degrees are defined as I III V
--                1234567
-- as bitstring = 1010100
function chordToBitString(chord)
    bits = {}
    local degree = chord.degrees
    for i=1, 7 do
        if degree[i] == nil then
            bits[i] = 0
        else
            bits[i] = 1
        end
    end
    return table.concat(bits)
end

GuitarSolver = ASolver:new()

function GuitarSolver:degreeToPitch(degreeDef, chord)
    return chord.rootPitch + (degreeDef.degreeValue % 12)
end

function GuitarSolver:_solveImpl(chord, degrees, args)
    
    local d = function(degreeValue) return self:getDefaultDegreeDef(degreeValue, degrees) end
    local strDegrees = chordToBitString(chord)
    local voicing = VoicingMatrix[strDegrees]
    if voicing == nil then
        error("no voicing found defined for " .. chord.strBase .. chord.strOptions)
    end
    local result = {}
    local octave = -1
    local lastDegree = -1
    for idx, degree in pairs(voicing) do
        if degree < lastDegree then 
            octave = octave + 1
        end
        lastDegree = degree
        result[idx] = self:createPitch(chord, d(degree  ), octave)
    end
    return result
end