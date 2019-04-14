require "com/com"
require "com/globals"
require "solvers/voicelead"

local solver = VoiceLeadSolver:new()

function solve(chord, degrees, args)
    return solver:solve(chord, degrees, args)
end