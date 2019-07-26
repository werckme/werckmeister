require "config"
ASolver = {}


function ASolver:new(o)
    o = o or {}
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

function ASolver:_keepRange(pitches, range)
    local orange = PitchRanges[range]
    if orange == nil then
        error("range " .. range .. " not defined")
    end
    local up = 0
    local down = 0
    local mindiff = math.maxinteger
    local maxdiff = math.mininteger
    for i, pitch in pairs(pitches) do
        local midipitch = toMidiPitch(pitch)
        if midipitch > orange.max then
            diff = midipitch - orange.max
            if diff > maxdiff then
                maxdiff = diff
            end
            down = down + 1
        end
        if midipitch < orange.min then
            diff = midipitch - orange.min
            if diff < mindiff then
                mindiff = diff
            end
            up = up + 1
        end
    end
    if up > down then
        self:_transpose(pitches, -math.ceil(mindiff/12))
    end
    if up < down then
        self:_transpose(pitches, -math.ceil(maxdiff/12))
    end
end