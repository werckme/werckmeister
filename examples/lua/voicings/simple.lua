require "lua/com/com"
require "solvers/simple"
require "solvers/asolver"

parameters = {
    {name="x"},
    --table.unpack(ASolverDefaultParameter)
}

local solver = Simple:new()

function solve(chord, degrees, parameters)
    dump(parameters)
    return solver:solve(chord, degrees, parameters)
end