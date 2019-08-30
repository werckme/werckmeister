require "com/com"
require "com/globals"

function testpitchdiff(a, b, expected)
    local diff = relpitchdiff(a, b)
    if diff ~= expected then
        print("pitchdiff: " .. a .. "-" .. b .. "~=" .. expected .. "(" .. diff .. ")" )
    end
end

testpitchdiff( 0,  0,   0)
testpitchdiff( 0,  5,   5)
testpitchdiff( 5,  0,  -5)
testpitchdiff( 0,  6,   -6)
testpitchdiff( 6,  0,   6)
testpitchdiff( 7,  0,   5)
testpitchdiff( 0,  7,  -5)
testpitchdiff( 7,  1,   6)
testpitchdiff( 1,  7,  -6)
testpitchdiff( 7, 11,   4)
testpitchdiff(11,  7,  -4)
testpitchdiff( 0, 10,  -2)
testpitchdiff(10,  0,   2)
testpitchdiff( 5,  9,   4)
testpitchdiff( 9,  5,  -4)
testpitchdiff( 5, 11,  -6)
testpitchdiff(11,  5,   6)
testpitchdiff( 9, 11,   2)
testpitchdiff(11,  9,  -2)
testpitchdiff(11,  1,   2)
testpitchdiff( 1, 11,  -2)
testpitchdiff( 12, 24,  0)


x = tokeyvalue({"a", 100, "b", 200, "c", 300})
assert(x.a == 100)
assert(x.b == 200)
assert(x.c == 300)


assert( toMidiPitch({pitch=C,   octave=0}) == 60)
assert( toMidiPitch({pitch=C,   octave=1}) == 72)
assert( toMidiPitch({pitch=CIS, octave=1}) == 73)
assert( toMidiPitch({pitch=C,   octave=-1}) == 48)