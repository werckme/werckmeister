ASolver = {}

function ASolver:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function ASolver:solve(chord, intervals, args)
    return self:_solveImpl(chord, intervals, args)
end

function ASolver:_solveImpl(chord, intervals, args)
    error("this is an abstract function")
end