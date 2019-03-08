require "solvers/asolver"
require "com/globals"

VoiceLeadSolver = ASolver:new()

function VoiceLeadSolver:_solveImpl(chord, intervals, args)
    local pitches = {}
    for degree, interval in pairs(intervals)
    do
        if isnumber(degree)
        then
            pitches[degree] = self:createPitch(chord, interval, interval.octave)
        end
    end
    self.previous = pitches
   -- dump(pitches)
    return pitches
end