require "lua/com/com"
require "solvers/simple"
require "solvers/asolver"

parameters = {
    table.unpack(ASolverDefaultParameter)
}

local solver = Simple:new()

function solve(chord, degrees, parameters)
    return solver:solve(chord, degrees, parameters)
end