require "lua/com/com"

local function cmp(a, b)
    return math.abs(a-b) < 0.01
end

function execute(params, timeinfo)
    local testPassed = false
    if timeinfo.quarterPosition == 4.0 then
        assert(timeinfo.bpm == 180.5)
        assert(cmp(timeinfo.seconds, 1.329))
        testPassed = true
    end
    if cmp(timeinfo.quarterPosition, 7.844) then
        assert(timeinfo.bpm == 118.0)
        assert(cmp(timeinfo.seconds, 3.988))
        testPassed = true
    end
    if cmp(timeinfo.quarterPosition, 5.028) then
        assert(timeinfo.bpm == 30.0)
        assert(cmp(timeinfo.seconds, 10.056))
        testPassed = true
    end
    assert(testPassed == true)
    return {}
end