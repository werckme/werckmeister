require "_model"
require "_integra7"

local nodeinfo = Get_Node("xxx")
assert(nodeinfo == nil)

assert(Denormalize100(50, 210, 0) == 50)
assert(Denormalize100(50, 210, 100) == 210)

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

nodeinfo = Get_Node("PRM-_FPART1-_SNTONE-_SNTC-SNTC_TONE_LEVEL");
assert(nodeinfo ~= nil)
assert(nodeinfo.node.desc == "Tone Level");
nodeinfo.node:setvalue100(100)
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


local sysexMessages = Get_Set_Mfx_Type_Messages(Mfx_Types.SNA, 1, 14)
-- set mfx type
sysexStr = Bytes_To_String(sysexMessages[1])
assert(sysexStr == "f0 41 10 00 00 64 12 19 02 02 00 0e 55 f7")
-- set default values
sysexStr = Bytes_To_String(sysexMessages[2])
assert(sysexStr == "f0 41 10 00 00 64 12 19 02 02 0d 00 00 00 00 08 00 00 03 08 00 08 0c 08 00 05 00 08 00 07 0f 08 00 04 00 08 00 00 0f 08 00 00 0f 08 00 07 0f 2c f7")

sysexMessages = Get_Set_Mfx_Type_Messages(Mfx_Types.SNS, 2, 0)
-- set mfx type
sysexStr = Bytes_To_String(sysexMessages[1])
assert(sysexStr == "f0 41 10 00 00 64 12 19 21 02 00 00 44 f7")
-- set default values
sysexStr = Bytes_To_String(sysexMessages[2])
assert(sysexStr == "f0 41 10 00 00 64 12 19 21 02 0d 00 00 00 00 08 00 00 00 2f f7")

sysexMessages = Get_Set_Mfx_Type_Messages(Mfx_Types.SND, 3, 67)
-- set mfx type
sysexStr = Bytes_To_String(sysexMessages[1])
assert(sysexStr == "f0 41 10 00 00 64 12 19 43 02 00 43 5f f7")
-- set default values
sysexStr = Bytes_To_String(sysexMessages[2])
assert(sysexStr == "f0 41 10 00 00 64 12 19 43 02 0d 00 00 00 00 08 00 01 04 08 00 00 00 08 00 00 0a 08 00 01 02 08 00 01 0e 08 00 03 02 08 00 01 04 08 00 00 01 08 00 00 0a 08 00 01 02 08 00 02 08 08 00 04 0f 08 00 03 02 08 00 07 0f 34 f7")

sysexMessages = Get_Set_Mfx_Type_Messages(Mfx_Types.PCMS, 4, 16)
-- set mfx type
sysexStr = Bytes_To_String(sysexMessages[1])
assert(sysexStr == "f0 41 10 00 00 64 12 19 60 02 00 10 75 f7")
-- set default values
sysexStr = Bytes_To_String(sysexMessages[2])
assert(sysexStr == "f0 41 10 00 00 64 12 19 60 02 0d 00 00 00 00 08 00 00 00 08 00 00 01 08 00 05 00 08 00 00 09 08 00 06 00 08 00 00 0f 08 00 00 0f 08 00 07 0f 6f f7")


sysexMessages = Get_Set_Mfx_Type_Messages(Mfx_Types.PCMD, 5, 18)
-- set mfx type
sysexStr = Bytes_To_String(sysexMessages[1])
assert(sysexStr == "f0 41 10 00 00 64 12 1a 10 02 00 12 42 f7")
-- set default values
sysexStr = Bytes_To_String(sysexMessages[2])
assert(sysexStr == "f0 41 10 00 00 64 12 1a 10 02 0d 00 00 00 00 08 00 07 0f 08 00 00 00 08 00 01 0e 08 00 07 0f 08 00 07 0f 08 00 00 00 08 00 01 0e 08 00 00 00 08 00 07 0f 08 00 00 00 08 00 01 0e 08 00 07 0f 08 00 00 00 08 00 00 00 08 00 01 0e 08 00 00 00 08 00 00 01 08 00 00 0a 08 00 01 02 08 00 03 02 08 00 00 00 08 00 03 0c 08 00 00 00 08 00 00 00 08 00 07 0f 1d f7")
