local inspect = require "com/inspect"

function istable(t) return type(t) == 'table' end
function isnumber(t) return type(t) == 'number' end
function toString(x) return inspect(x) end