require "com/com"
require "com/globals"
require "solvers/voicelead"

local solver = VoiceLeadSolver:new()

function solve(chord, intervals, args)
    return solver:solve(chord, intervals, args)
end