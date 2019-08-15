require "lua/voicings/solvers/asolver"
require "lua/com/globals"

Simple = ASolver:new()

function Simple:degreeToPitch(degreeDef, chord)
    return chord.rootPitch + degreeDef.degreeValue
end

function Simple:_solveImpl(chord, degrees, args)
    local pitches = {}
    for degree, degreeDefs in pairs(degrees)
    do
        if isnumber(degree)
        then
            for idx, degreeDef in pairs(degreeDefs)
            do
                table.insert(pitches, self:createPitch(chord, degreeDef, degreeDef.octave))
            end
        end
    end
    return pitches
end