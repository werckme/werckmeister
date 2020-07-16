-- A definition of drum rudiments.
-- The actual rudiment duration dosen't matter. At the end, the sequence
-- will be ajdusted, so that it fits into the duration of the related event.
-- The relations between the diffrent durations is what matters.
-- For example
-- A rudiment with four quarters, played over the duration of one eight:
-- every rudiment quarter will become eventually a 32th
R="R"
L="L"

FlamType = "flam"
DragType = "drag"

-- durations
local _4    = 1
local _8    = 1/2
local _16   = 1/4
local _32   = 1/8 
local _4t   = _4 / 3
local _8t   = _8 / 3
local _16t  = _16 / 3
local _32t  = _32 / 3
-- velocity factors
Accent      = "accent"
Unaccented  = "unaccented"
GraceNote   = "grace note"

local ac = Accent
local un = Unaccented


-- create a rudiment sequence event
local function Stroke(which, duration, velocityClass)
    if velocityClass == nil then
        velocityClass = Accent
    end
    return { which=which, duration=duration, velocityClass = velocityClass }
end

local function Flam(which, duration, velocityClass)
    if velocityClass == nil then
        velocityClass = Accent
    end
    return { which=which, duration=duration, velocityClass = velocityClass, type = FlamType}
end

local function Drag(which, duration, velocityClass)
    if velocityClass == nil then
        velocityClass = Accent
    end
    return { which=which, duration=duration, velocityClass = velocityClass, type = DragType}
end

local S = Stroke

local function Diddle(which, duration)
    return {
        S(which, duration, un), S(which, duration, un)
    }
end

-- returns L for R and R for L.
function Alt(which)
    if which == L then
        return R
    elseif which == R then
        return L
    end
    return nil
end

-- alternates the "Hand" value (which).
-- "L" -> "R"
-- "R" -> "L"
function AlternateEvent(event)
    local copy = deepcopy(event)
    copy.which = Alt(event.which)
    return copy
end

-- Creates a alternating stroke sequence RL...n or LR...n.
-- @param startingEvent: the first event. All further created events are unaccented.
-- @param numberOfPerformances: the number of the total performances. One RL means one performance.
local function Alternate(startingEvent, numberOfPerformances)
    if numberOfPerformances == nil then
        numberOfPerformances = 1
    end
    local result = {startingEvent}
    local which = startingEvent.which
    for i = 0, (numberOfPerformances-1)*2, 1 do
        which = Alt(which)
        table.insert(result, S(which, startingEvent.duration, un))
    end
    return result
end

-- Creates a sequence of variable args.
-- An argument can be either a array of events or a single event
local function Seq(...)
    local args = table.pack(...)
    local result = {}
    for i=1,args.n do
        local arg = args[i]
        local isEvent = arg.which ~= nil
        if isEvent then
            table.insert(result, arg)
        else
            for key, event in pairs(arg) do
                table.insert(result, event)
            end
        end
    end
    return result
end

Rudiments = {
    ["singleStrokeRoll"] =
        { S(R, _4), S(L, _4) },
    ["doubleStrokeRoll"] =
        { S(R, _4), S(R, _4), S(L, _4), S(L, _4) },
    ["trippleStrokeRoll"] =
        { S(R, _4), S(R, _4), S(R, _4), S(L, _4), S(L, _4), S(L, _4) },
    ["fiveStrokeRoll"] =
        { S(R, _32, un), S(R, _32, un), S(L, _32, un), S(L, _32, un), S(R, _8, ac) },
    ["singleStrokeFour"] =
        { S(R, _4t), S(L, _4t), S(R, _4t), S(L, _4) },
    ["singleStrokeSeven"] =
        { S(R, _8t), S(L, _8t), S(R, _8t), S(L, _8t), S(R, _8t), S(L, _8t), S(R, _4) },
    ["multipleBounceRoll"] =
        { S(R, _32, un), S(R, _32, un), S(R, _32, un), S(R, _32, un),
          S(L, _32, un), S(L, _32, un), S(L, _32, un), S(L, _32, un)},
    ["sixStrokeRoll"] =
        Seq(S(R, _8, ac), Diddle(L, _16), Diddle(R, _16), S(L, _8, ac)),
    ["sevenStrokeRoll"] =
        Seq(Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), S(R, _8, ac)),
    ["nineStrokeRoll"] =
        Seq(Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), S(R, _8, ac)),
    ["tenStrokeRoll"] =
        Seq(Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), S(R, _8, ac), S(L, _8, ac)),
    ["elevenStrokeRoll"] =
        Seq(Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), S(L, _8, ac)),
    ["thirteenStrokeRoll"] =
        Seq(Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), S(R, _8, ac)),
    ["fifteenStrokeRoll"] =
        Seq(Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), S(L, _8, ac)),
    ["seventeenStrokeRoll"] =
        Seq(Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), Diddle(R, _16), Diddle(L, _16), S(R, _8, ac)),
    ["paradiddle"] =
        Seq(Alternate(S(R, _16, ac)), Diddle(R, _16)),
    ["doubleParadiddle"] =
        Seq(Alternate(S(R, _16, ac), 2), Diddle(R, _16)),
    ["trippleParadiddle"] =
        Seq(Alternate(S(R, _16, ac), 3), Diddle(R, _16)),
    ["paradiddlediddle"] =
        Seq(Alternate(S(R, _16, ac)), Diddle(R, _16), Diddle(L, _16)),
    ["flam"] =
        Seq(Flam(R, _4)),
    ["flamAccent"] =
        Seq(Flam(R, _8, ac), S(L, _8, un), S(R, _8, un)),
    ["flamTap"] =
        Seq(Flam(R, _16, ac), S(R, _16, un)),
    ["flamacue"] =
        Seq(Flam(R, _16, un), S(L, _16, ac), S(R, _16, un), S(L, _16, un), Flam(R, _4, un)),
    ["flamParadiddle"] =
        Seq(Flam(R, _16, ac), S(L, _16, un), Diddle(R, _16)),
    ["singleFlammedMill"] =
        Seq(Flam(R, _16, ac), S(R, _16, un), S(L, _16, un), S(R, _16, un)),
    ["pataflafla"] =
        Seq(Flam(R, _16, ac), S(L, _16, un), Diddle(R, _16), Flam(L, _16, ac)),
    ["swissArmytriplet"] =
        Seq(Flam(R, _16t, ac), S(R, _16t, un), S(L, _16t, un)),
    ["invertedFlamTap"] =
        Seq(Flam(R, _16, ac), S(L, _16, un)),
    ["flamDrag"] =
        Seq(Flam(R, _8, ac), S(L, _16, un), S(L, _16, un), S(R, _8, un)),
    ["drag"] =
        Seq(Drag(L, _4, ac)),
}
