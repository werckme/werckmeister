
-- Implements a collection of different drum rudiments.
-- You can specify which notes are for L & R and the time measure.
-- This will be achived by using an chord as source material.
-- The Chord pitches will be used for L&R.
-- The length of the chord event defines in which speed the rudiment will be peformed.
-- The event tag specifies which rudiment will be performed.
-- Examples:
--
-- This plays a paradiddle with the length of 1 quarter
-- using sn1 for L and sn2 for R
-- /mod: rudiments /
-- "paradiddle"@<"sn1" "sn2">4
--
--                 L     R    L      R
-- "paradiddle"@<"sn1" "sn2" "tm1" "tm2">4
-- which results in   sn1(L)   sn2(R)  tm1(L)  tm2(R) sn1(L) ..
--
-- since the pitches of an chord event are distinct, this is not possible.
-- "paradiddle"@<"sn1" "sn1">4
-- tis problem also appears if two pitch aliases (e.g. "sn1", "sn2") are mapped
-- to the same picth

require "lua/com/com"
require "_drumRudimentsRepository"
require "_events"

parameters = {
}

SupportedRepeatTags = {
    ["2x"]  =  2,
    ["3x"]  =  3,
    ["4x"]  =  4,
    ["5x"]  =  5,
    ["6x"]  =  6,
    ["7x"]  =  7,
    ["8x"]  =  8,
    ["9x"]  =  9,
    ["11x"] = 11,
    ["12x"] = 12,
    ["13x"] = 13,
    ["14x"] = 14,
    ["15x"] = 15,
    ["16x"] = 16,
    ["17x"] = 17,
    ["18x"] = 18,
    ["19x"] = 19,
    ["20x"] = 20,
    ["21x"] = 21,
    ["22x"] = 22,
    ["23x"] = 23,
    ["24x"] = 24,
    ["25x"] = 25,
    ["26x"] = 26,
    ["27x"] = 27,
    ["28x"] = 28,
    ["29x"] = 29,
    ["30x"] = 30,
    ["31x"] = 31,
    ["32x"] = 32,
}

function perform(events, params, timeinfo)
    if #events == 0 or #events[1].tags == 0 then
        return events
    end
    if events[1].duration == 0 then
        return events
    end
    local rudimentPerformer = RudimentPerformer:new()
    rudimentPerformer:setSourceEvents(events)
    if rudimentPerformer.rudiment == nil then
        return events
    end
    local rudimentEvents = rudimentPerformer:perform();
    return rudimentEvents
end

RudimentPerformer = {}
function RudimentPerformer:new(o)
    local o = o or {}
    setmetatable(o, self)
    self.__index = self
    self.idxL = 1
    self.idxR = 1
   
    return o
end

function RudimentPerformer:findRudimentName(tags)
    for idx, tag in pairs(tags) do
        local val = Rudiments[tag]
        if val ~= nil then
            return tag
        end
    end
    return nil
end

function RudimentPerformer:findRepeatTagValue(tags)
    for idx, tag in pairs(tags) do
        local val = SupportedRepeatTags[tag]
        if val ~= nil then
            return val
        end
    end
    return nil
end

function RudimentPerformer:setSourceEvents(events)
    local event = events[1]
    local rudimentName = self:findRudimentName(event.tags)
    local source       = event.pitches
    self.rudiment = Rudiments[rudimentName]
    if self.rudiment == nil then
        return
    end
    if #source % 2 ~= 0 then
        -- not enough events
        error("not enough events for rudiment " .. rudimentName)
    end
    local repeatCount = self:findRepeatTagValue(event.tags)
    if repeatCount ~= nil then
        self:repeat_(repeatCount)
    end
    self.duration = event.duration
    self.ls = {}
    self.rs = {}
    self.velocity = event.velocity
    for idx, pitch in pairs(source) do
        if idx % 2 == 0 then
            table.insert(self.ls, pitch)
        else
            table.insert(self.rs, pitch)
        end
    end
end

function RudimentPerformer:repeat_(count)
    local sequence = {}
    for i=1,count do
        for idx, rudimentEvent in pairs(self.rudiment) do
            table.insert(sequence, rudimentEvent)
        end
    end
    self.rudiment = sequence
end

function RudimentPerformer:l()
    local l = self.ls[self.idxL]
    self.idxL = self.idxL + 1
    if self.idxL > #self.ls then
        self.idxL = 1
    end
    return l
end

function RudimentPerformer:r()
    local r = self.rs[self.idxR]
    self.idxR = self.idxR + 1
    if self.idxR > #self.ls then
        self.idxR = 1
    end
    return r
end

function RudimentPerformer:nextPitch(lOrR)
    if lOrR == nil then
        error("invalid L R value for rudiment: nil")
    end
    if lOrR == L then
        return self:l()
    elseif lOrR == R then
        return self:r()
    end
    error("invalid L R value for rudiment: " .. lOrR)
end

-- duration of the rudiment definition
function RudimentPerformer:defDuration()
    local duration = 0
    for idx, rudiment in pairs(self.rudiment) do
        duration = duration + rudiment.duration
    end
    return duration
end

function RudimentPerformer:perform()
    local events = {}
    local offset = 0
    local durationFactor = self.duration / self:defDuration()
    for idx, rudiment in pairs(self.rudiment) do
        local which          = rudiment.which
        local duration       = rudiment.duration
        local velocityFactor = rudiment.velocityFactor
        local note = Note:new()
        note.duration = duration * durationFactor
        note.velocity = self.velocity * velocityFactor
        note.offset = offset
        local pitch = self:nextPitch(which)
        note:addPitch(pitch.pitch, pitch.octave)
        table.insert(events, note)
        offset = offset + note.duration
    end
    return events
end

