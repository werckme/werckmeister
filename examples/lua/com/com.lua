local _inspect = require "lua/com/inspect"

local MidiSchluesselCOffset = 60
ParameterValueNoTag = '__noTagValue'

function istable(t) return type(t) == 'table' end
function isnumber(t) return type(t) == 'number' end
function inspect(x) return print(_inspect(x)) end
function dump(x) print(inspect(x)) end


function checkLegacyNamedParams(params, ...)
    local args = table.pack(...)
    for k, v in pairs(params) do
        if contains(args, v) then
            local msg = "Error: legacy named parameter with the argument '" ..  v .. "'. "
            msg = msg .. " Use this expression instead: _" .. v .. "=..." 
            error(msg)
        end
    end
end

-- returns the minimum amount of semitones between the relative pitches x and y.
-- e.g.: from c(0) to b(11) = -1
function relpitchdiff(x, y)
    x = x % 12
    y = y % 12
    if math.abs(x-y) < 6 then
        return -(x-y)
    end
    if x < 6 then 
        x = x + 12 
    end
    if y < 6 then
        y = y + 12
    end
    return -(x-y)
end

-- [a, 1, b, 2, c, 3] => {a=1, b=2, c=3}
-- @deprecated use the native named parameter feature: _name=value
function tokeyvalue(args)
    assert(#args % 2 == 0)
    local result = {}
    for i=1, #args, 2 do
        result[ args[i] ] = args[i+1]
    end
    return result
end

function toMidiPitch(pitch)
    return MidiSchluesselCOffset + pitch.pitch + (pitch.octave * 12)
end

function deepcopy(o, seen)
    seen = seen or {}
    if o == nil then return nil end
    if seen[o] then return seen[o] end
  
    local no
    if type(o) == 'table' then
      no = {}
      seen[o] = no
  
      for k, v in next, o, nil do
        no[deepcopy(k, seen)] = deepcopy(v, seen)
      end
      setmetatable(no, deepcopy(getmetatable(o), seen))
    else -- number, string, boolean, etc
      no = o
    end
    return no
end

function contains(table, value)
    if table == nil then
        return false
    end
    for k, v in pairs(table) do
        if v == value then
            return true
        end
    end
    return false
end

-- https://stackoverflow.com/questions/1426954/split-string-in-lua#1426970
function split(pString, pPattern)
    local Table = {}  -- NOTE: use {n = 0} in Lua-5.0
    local fpat = "(.-)" .. pPattern
    local last_end = 1
    local s, e, cap = pString:find(fpat, 1)
    while s do
       if s ~= 1 or cap ~= "" then
      table.insert(Table,cap)
       end
       last_end = e+1
       s, e, cap = pString:find(fpat, last_end)
    end
    if last_end <= #pString then
       cap = pString:sub(last_end)
       table.insert(Table, cap)
    end
    return Table
 end

function FilterEventsByTagName(events, tagName)
    local result = {}
    for _, event in pairs(events) do
        if contains(event.tags, tagName) then
            table.insert(result, event)
        end
    end
    return result
 end