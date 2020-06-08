-- A definition of drum rudiments.
-- The actual rudiment duration dosen't matter. At the end, the sequence
-- will be ajdusted, so that it fits into the duration of the related event.
-- The relations between the diffrent durations is what matters.
-- For example
-- A rudiment with four quarters, played over the duration of one eight:
-- every rudiment quarter will become eventually a 32th
R="R"
L="L"

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
local ac  = 1.0 -- accent
local un  = 0.8 -- unaccented

-- create a rudiment sequence event
local function C(which, duration, velocityFactor)
    if velocityFactor == nil then
        velocityFactor = 1
    end
    return { which=which, duration=duration, velocityFactor = velocityFactor }
end


Rudiments = {
    ["singleStrokeRoll"] =
        { C(R, _4), C(L, _4) },
    ["doubleStrokeRoll"] =
        { C(R, _4), C(R, _4), C(L, _4), C(L, _4) },
    ["trippleStrokeRoll"] =
        { C(R, _4), C(R, _4), C(R, _4), C(L, _4), C(L, _4), C(L, _4) },
    ["fiveStrokeRoll"] =
        { C(R, _32, un), C(R, _32, un), C(L, _32, un), C(L, _32, un), C(R, _8, ac) },
    ["singleStrokeFour"] =
        { C(R, _4t), C(L, _4t), C(R, _4t), C(L, _4) },
    ["singleStrokeSeven"] =
        { C(R, _8t), C(L, _8t), C(R, _8t), C(L, _8t), C(R, _8t), C(L, _8t), C(R, _4) },
    ["multipleBounceRoll"] =
        { C(R, _32, 1.0), C(R, _32, 0.9), C(R, _32, 0.8), C(R, _32, 0.7),
          C(L, _32, 1.0), C(L, _32, 0.9), C(L, _32, 0.8), C(L, _32, 0.7)}
}
