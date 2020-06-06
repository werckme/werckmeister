-- simulates the chord voicing of an guitar

require "lua/com/com"
require "lua/com/globals"
require "solvers/guitar"

parameters = {
    table.unpack(ASolverDefaultParameter)
}

local solver = GuitarSolver:new()

function solve(chord, degrees, params, timeinfo)
    return solver:solve(chord, degrees, params)
end