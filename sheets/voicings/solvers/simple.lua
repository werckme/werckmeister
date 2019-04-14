require "solvers/asolver"
require "com/globals"

Simple = ASolver:new()

function Simple:intervalToPitch(interval, chord)
    return chord.rootPitch + interval.interval
end

function Simple:_solveImpl(chord, degrees, args)
    local pitches = {}

    for degree, interval in pairs(degrees)
    do
        if isnumber(degree)
        then
            pitches[degree] = self:createPitch(chord, interval, interval.octave)
        end
    end
    return pitches
end