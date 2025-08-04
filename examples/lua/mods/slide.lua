-- <command name="slide" where="mod" using="lua/mod/chromaticSlide.lua">
-- Performs a chromatic slide to the target note
-- ```language=Werckmeister\n
--using "lua/mods/chromaticSlide.lua";\n
--tempo: 140;\n
--device: MyDevice  midi 0;\n
--instrumentDef:piano  MyDevice  _ch=0 _pc=0;\n
--[\n
--instrument: piano;\n
--{\n
--   /modOn: chromaticSlide _forTag=slide _numNotes=-4 _duration=0.25/\n
--   -- only the `d` will be performed slide. \n
--   c "slide"@d e f\n
--}\n
--]\n
-- ```
-- </command>
-- <param name="forTag" optional="1" type="text">Specifies a tag name. If set only events with this tag name will be affected by the slide mod.</param>
-- <param name="numNotes" optional="0" type="0..N">number of slide nodes. Can be positive or negative</param>
-- <param name="duration" optional="0" type="0..N">the duration of the slide in quarters</param>
-- <param name="velocity" optional="1" type="0..1">the slide velocity</param>
-- <param name="velocityInc" optional="1" type="0..1">a velocity incremental value</param>

require "lua/com/com"
require "_events"

local defaultAmountValue = 50
local noValue = "0"

parameters = {
    -- specify a tag name. If set only events with this tag name will be affected by the staccato mod. 
    { name="forTag", default=ParameterValueNoTag },
    -- the ammount of the staccato mod. Can be between 0 and 100
    { name="numNotes",           default=noValue },
    { name="duration",           default=noValue },
    { name="velocity",           default=noValue },
    { name="velocityInc",        default=0 }
}

function perform(events, params, timeinfo)
    if (#events == 0) then
        return events
    end
    if (#events[1].pitches == 0) then
        return events
    end
    local tag = params.forTag
    if tag == ParameterValueNoTag then
        tag = nil
    end
    local event = events[1]
    if tag ~= nil then
        event = FilterEventsByTagName(events, tag)[1]
        if event == nil then
            return events
        end
    end
    if params.numNotes == noValue then
        error("missing numNotes value")
    end
    if params.duration == noValue then
        error("missing duration value")
    end
    local velocity = event.velocity
    if params.velocity ~= noValue then
        velocity = tonumber(params.velocity)
    end
    local velocityInc = tonumber(params.velocityInc)
    local numNotes = tonumber(params.numNotes)
    local absNumNotes = math.abs(numNotes)
    local duration = tonumber(params.duration)
    local durationOfOneNote = duration / absNumNotes
    local targetPitch = toMidiPitch(event.pitches[1])
    local direction = 1
    local pitch = targetPitch + numNotes
    local offset = -absNumNotes * durationOfOneNote
    if numNotes > 0 then
        direction = -1
    end
    for i = 1, absNumNotes, 1 do
        local note = Note:new()
        note.duration = durationOfOneNote
        note.velocity = velocity
        note.offset = offset
        local pitchObj = fromMidiPitch(pitch)
        note:addPitch(pitchObj.pitch, pitchObj.octave)
        table.insert(events, note)
        pitch = pitch + direction
        offset = offset + durationOfOneNote
        velocity = velocity + velocityInc
    end
    return events
end