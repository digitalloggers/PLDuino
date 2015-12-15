--This module contains code which is used to communicate with Arduino.

-- Length of packets sent by Arduino on state requests
local OUTPUTS_COUNT = 8
local INPUTS_COUNT = 8
local ANALOGS_COUNT = 8
local RELAYS_COUNT = 6
local TIME_LENGTH = 2+2
local DATE_LENGTH = 4+2+2
local STATE_PACKET_SIZE =
	RELAYS_COUNT +
	INPUTS_COUNT +
	OUTPUTS_COUNT +
	ANALOGS_COUNT*4 +
	TIME_LENGTH +
	DATE_LENGTH +
	1 -- 'e'


-- Utility functions.

local function genarr(num, item)
	local a = {}
	for i=1,num,1 do a[i]=item end
	return a
end

local function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end


-- This variable stores current snapshot of PLDuino's outlets.
local state = {
	analog = genarr(8, 0),
	output = genarr(8, false),
	input  = genarr(8, false),
	relay  = genarr(6, false),
	time   = { hour=0, minute=0 },
	date   = { year=1980, month=1, day=1 }
}


-- Parses Arduino's answer to State request.
local function parseState(data)
	--[[

		Each packet ends with "e" char.
		Structure of the data:

			RELAYS:  "000000" +
			INPUTS:  "00000000" + 
			OUTPUTS: "00000000" +
			ANALOGS: "11112222333344445555666677778888" +
			TIME:	 "hhmm" +
			DATE:    "yyyymmdd"
					 "e"

	--]]

	if #data == STATE_PACKET_SIZE then

		-- Packet seems to be valid, try to parse it.
		
		-- The updated state is stored in this variable
		local newstate = deepcopy(state)
		
		-- Retrieve relays state.
		for i=1,RELAYS_COUNT,1 do
			local idx = i
			newstate.relay[i] = (data:sub(idx,idx)=="1")
		end

		-- Retrieve DINs.
		for i=1,INPUTS_COUNT,1 do
			local idx = i+RELAYS_COUNT
			newstate.input[i] = (data:sub(idx,idx)=="1")
		end

		-- Retrieve DOUTs.
		for i=1,OUTPUTS_COUNT,1 do
			local idx = i+RELAYS_COUNT+INPUTS_COUNT
			newstate.output[i] = (data:sub(idx,idx)=="1")
		end

		-- Retrieve AINs values.
		for i=1,ANALOGS_COUNT,1 do
			local idx = ((i-1)*4+1)+RELAYS_COUNT+INPUTS_COUNT+OUTPUTS_COUNT
			local str = data:sub(idx, idx+3)
			local value = tonumber(str)
			if value == nil then return end
			newstate.analog[i] = value
		end

		-- Parse time and date.
		local timeidx = RELAYS_COUNT+INPUTS_COUNT+OUTPUTS_COUNT+ANALOGS_COUNT*4+1
		local hour = tonumber(data:sub(timeidx, timeidx+1))
		local minute = tonumber(data:sub(timeidx+2, timeidx+2+1))
		
		local dateidx = RELAYS_COUNT+INPUTS_COUNT+OUTPUTS_COUNT+ANALOGS_COUNT*4+TIME_LENGTH+1
		local year = tonumber(data:sub(dateidx, dateidx+3))
		local month = tonumber(data:sub(dateidx+4, dateidx+4+1))
		local day = tonumber(data:sub(dateidx+4+2, dateidx+4+2+1))

		if not (hour and minute and year and month and day) then return end
		newstate.time = { hour=hour, minute=minute }
		newstate.date = { year=year, month=month, day=day }

		-- Replace old snapshot with the new one.
		state = newstate

	else
		-- invalid state packet, nothing to do.
	end
end

---------------------------------------------------------------------------------------------------------------------


local atmega = {}

-- Send command, e.g. R1 or O2
function atmega.sendCommand(cmd)
	uart.write(0, "C" .. cmd)
end

-- Get current outlets' snapshot.
function atmega.getState()
	return deepcopy(state)
end


---------------------------------------------------------------------------------------------------------------------


-- Arduino sends only state packets via Serial.
uart.on("data", "e", parseState, 0)

-- Start polling
tmr.alarm(3, 1000, 1, function()
	atmega.sendCommand("ss")
end)


---------------------------------------------------------------------------------------------------------------------

return atmega
