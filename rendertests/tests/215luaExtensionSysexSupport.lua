
function execute(params, timeinfo)
    return {
       {
            ["type"] = "sysex",
            -- integra 7, set guitar tone variation to mute
            ["sysexData"]  = {0x41, 0x10, 0x00, 0x00, 0x64, 0x12, 0x19, 0x02, 0x00, 0x2B, 0x01, 0x39}
       }
    }
end