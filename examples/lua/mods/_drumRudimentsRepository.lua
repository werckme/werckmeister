-- A definition of drum rudiments.
-- The base measure for durations is a quarter.
-- For example
-- A rudiment with four quarters, played over the duration of one eight,
-- every rudiment quarter will become a eventually a 16th
R="R"
L="L"

-- durations
_4   = 1
_8   = 1/2
_16  = 1/4

RudimentDurationMeasure = _4

function C(which, duration)
    return { which=which, duration=duration }
end

Rudiments = {
    ["singleStrokeRollR"] =
        { C(R, _4), C(L, _4), C(R, _4), C(L, _4) },
    ["singleStrokeRollL"] =
        { C(L, _4), C(R, _4), C(L, _4), C(R, _4) }
}