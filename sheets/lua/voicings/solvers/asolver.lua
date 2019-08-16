require "config"
require "lua/com/com"

ASolver = {}


function ASolver:new(o)
    local o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function ASolver:getDefaultDegreeDef(degreeValue, degrees)
    local default = degrees[degreeValue]
    if default == nil 
    then
        return nil
    end
    return default[1]
end

function ASolver:createPitch(chord, degreeDef, octave)
    if degreeDef == nil
    then
        return nil
    end
    return { ["pitch"]= self:degreeToPitch(degreeDef, chord), ["octave"]=octave }
end

function ASolver:degreeToPitch(degreeDef, chord)
    return chord.rootPitch + degreeDef.degreeValue
end

function ASolver:getAbsolutePitch(pitch)
    return pitch.pitch + (pitch.octave * 12)
end

function ASolver:solve(chord, degrees, args)
    args = tokeyvalue(args)
    local result = self:_solveImpl(chord, degrees, args)
    if args.range ~=nil then
        self:_keepRange(result, args.range)
    end
    return result
end

function ASolver:_solveImpl(chord, degrees, args)
    error("this is an abstract function")
end

function ASolver:_transpose(pitches, numOctaves)
    for i, pitch in pairs(pitches) do
        pitch.octave = pitch.octave + numOctaves
    end
end

function ASolver:_keepRange(pitches, strRange)
    local destinationRange = PitchRanges[strRange]
    if destinationRange == nil then
        error("strRange " .. strRange .. " not defined")
    end
    local nbUnder = 0
    local nbOver = 0
    local mindiff = math.maxinteger
    local maxdiff = math.mininteger
    for i, pitch in pairs(pitches) do
        local midipitch = toMidiPitch(pitch)
        if midipitch > destinationRange.max then
            local diff = midipitch - destinationRange.max
            if diff > maxdiff then
                maxdiff = diff
            end
            nbOver = nbOver + 1
        end
        if midipitch < destinationRange.min then
            local diff = destinationRange.min - midipitch
            if diff < mindiff then
                mindiff = diff
            end
            nbUnder = nbUnder + 1
        end
    end
    if nbUnder > nbOver then
        local octaves = math.ceil(mindiff/12)
        self:_transpose(pitches, octaves)
    end
    if nbUnder < nbOver then
        local octaves = -math.ceil(maxdiff/12)
        self:_transpose(pitches, octaves)
    end
end