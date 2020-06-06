-- rearranges the octaves of a chord 
-- so that the actual pict distance between the previous chord is kept minimal.

require "lua/com/com"
require "lua/com/globals"
require "solvers/voicelead"

parameters = {
    table.unpack(ASolverDefaultParameter)
}

local solver = VoiceLeadSolver:new()

function solve(chord, degrees, params)
    return solver:solve(chord, degrees, params)
end