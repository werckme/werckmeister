require "_model"

local nodeinfo = Get_Node("xxx")
assert(nodeinfo == nil)

local nodeinfo = Get_Node("PRM-_FPART1")
assert(nodeinfo ~= nil)
assert(Nibble(0x19000000) == 52428800)
assert(nodeinfo.node.desc == "Temporary Tone (Studio Mode Part 1)");
assert(nodeinfo.node.addr == Nibble(0x19000000));

nodeinfo = Get_Node("PRM-_FPART1-_SNTONE-_SNTC-SNTC_MOD_PRM14");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.addr == Nibble(0x002F));
assert(nodeinfo.node.desc == "Modify Parameter 14");

nodeinfo = Get_Node("PRM-_SETUP-_STP-_RC2-NESTP_SND_MODE_SD1");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "Side 1 Sound Mode");

nodeinfo = Get_Node("PRM-_FPART1-_SNTONE-_SNTC-SNTC_TONE_LEVEL");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "Tone Level");
nodeinfo.node:setvalue(127)
local sysex = Create_SysexMessage(nodeinfo)
local sysexStr = Bytes_To_String(sysex)
assert(sysexStr == "f0 41 10 00 00 64 12 19 02 00 10 7f 56 f7")

nodeinfo.node:setvalue(121)
sysex = Create_SysexMessage(nodeinfo, 23)
sysexStr = Bytes_To_String(sysex)
assert(sysexStr == "f0 41 17 00 00 64 12 19 02 00 10 79 5c f7")

nodeinfo = Get_Node("PRM-_SYS-_SC-NESC_TUNE");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "Master Tune");
nodeinfo.node:setvalue(2024)
sysex = Create_SysexMessage(nodeinfo)
sysexStr = Bytes_To_String(sysex)
assert(sysexStr == "f0 41 10 00 00 64 12 02 00 00 00 00 07 0e 08 61 f7")

nodeinfo = Get_Node("PRM-_SYS-_SC-NESC_TEMPO");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "System Tempo");
nodeinfo.node:setvalue(250)
sysex = Create_SysexMessage(nodeinfo)
sysexStr = Bytes_To_String(sysex)
assert(sysexStr == "f0 41 10 00 00 64 12 02 00 00 26 0f 0a 3f f7")

nodeinfo = Get_Node("PRM-_FPART1-_PAT-_PC-RFPC_NAME");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "PCM Tone Name");
nodeinfo.node:setvalue("012345")
sysex = Create_SysexMessage(nodeinfo)
sysexStr = Bytes_To_String(sysex)
assert(sysexStr == "f0 41 10 00 00 64 12 19 00 00 00 30 31 32 33 34 35 20 20 20 20 20 20 78 f7")