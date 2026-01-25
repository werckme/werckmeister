require "lua/com/com"
require "com"

function Init()
    local midiEvents = GetMidiEvents()
    for _, track in ipairs(midiEvents) do
        print(track.name, #track.events)
    end
end

function OnMidiEvent(event)
    if event.type == NoteOn then
        print(event.position)
    end
    -- if event.position >=4 then
    --     JumpToPosition(0)
    -- end
    if event.type == MetaEvent and event.metaType == MetaCuePoint then
        JumpToPosition(0)
    end
end