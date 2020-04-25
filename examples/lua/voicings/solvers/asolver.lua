require "config"
require "lua/com/com"

ASolver = {}


function ASolver:new(o)
    local o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

-- returns the a degree def from the given degree def collection
-- if more than one degree def defined, it will return the first one
function ASolver:getDefaultDegreeDef(degreeValue, degrees)
    local default = degrees[degreeValue]
    if default == nil 
    then
        return nil
    end
    return default[1]
end

-- creates a absolute pitch using the given chord
function ASolver:createPitch(chord, degreeDef, octave)
    if degreeDef == nil or degreeDef.degreeValue == nil
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

function ASolver:_addImportantDegree(degrees, degreeVal, semitone)
    local degreeInfo = self:getDefaultDegreeDef(degreeVal, degrees)
    if degreeInfo==nil then
        return
    end
    local degrObj = {
        degreeValue= semitone,
        octave= degreeInfo.octave
    }
    degrees[degreeVal] = {degrObj}
end

-- set important degrees if defined in args
-- more informations about important degrees:
-- https://github.com/werckme/werckmeister/issues/123
function ASolver:_setImportantDegreesIfExists(args, degrees)
    local strDegree = args["importantDegree"]
    if (strDegree == nil) then
        strDegree = args["importantDegrees"]
        if strDegree == nil then
            return
        end
    end
    local strDegrees = split(strDegree, " ")
    for i, degree in pairs(strDegrees) do   
        local degreeVal = DegreeStringValues[degree]
        if degreeVal ~= nil then
            local semitone = MajorScaleSemitones[degree]
            self:_addImportantDegree(degrees, degreeVal, semitone)
        end
    end
end

-- solves degrees into absolute pitches
-- @param chord: the current cord informations
-- @param degrees: the current degree event including the semitone informations from 
--                 the chord definition eg.: { II= {degreeValue, octave} }
--                 If the current chord has no information about a degree
--                 the value is empty eg.: II= {} 
function ASolver:solve(chord, degrees, args)
    args = tokeyvalue(args)
    self:_setImportantDegreesIfExists(args, degrees)
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