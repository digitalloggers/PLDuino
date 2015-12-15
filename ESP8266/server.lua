-- This module implements a simple web-server to allow configuration changes via Wi-Fi.
-- Please be careful! Since it's just an example of working with ESP module, it has no access control at all (to keep things simple).

local atmega = require "atmega"
local json = require "cjson"

local function close_connection(c)
	c:close()
end

-- Send file fname via conn, asynchronously.
local function send_file(conn, fname)
	local curpos = 0
	local function cbk(c)
		file.open(fname)
		file.seek("set", curpos)
		local data = file.read(111)
		curpos = file.seek("cur", 0)
		file.close()

		if data == nil then
			close_connection(c)
		else
			c:send(data, cbk)
		end
	end
	cbk(conn)
end

-- Start listening for requests on port 80.
sv = net.createServer(net.TCP, 30)
sv:listen(80, function(conn)
	conn:on("receive", function(c, pl)

		-- Retrieve path.
		local path = string.match(pl, ".*GET +(.*) +HTTP.*")

		if path == "/" then
			-- load index.html.
			c:send(
				"HTTP/1.0 200 OK\r\n" ..
				"Content-Type: text/html\r\n" ..
				"Connection: close\r\n" ..
				"\r\n",
				function(cnn) send_file(cnn, "index.html") end
			)
		elseif path == "/state" then
			-- send a JSON with outlets' state.
			local state = atmega.getState()
			local s="HTTP/1.0 200 OK\r\n" .. 
					"Content-Type: application/json\r\n" ..
					"Connection: close\r\n" ..
					"\r\n" ..
					json.encode(state)
			c:send(s, close_connection)
		else
			-- state change request.

			-- format: device/index/newstate, where
			--     device   = relay for relays, dout for digital outputs;
			--     index    = 0..5 for relays, 0..7 for douts;
			--     newstate = on / off

			local device,index,newstate = string.match(path, "/(%a+)/(%d+)/(%a+)")
			index = tonumber(index)
			
			-- validate the data
			local valid = true
			if not (device and index and newstate) then valid = false
			elseif not (device == "relay" or device == "dout") then valid = false
			elseif not (newstate == "on" or newstate == "off") then valid = false
			elseif index < 0 then valid = false
			elseif device == "relay" and index>=6 then valid = false
			elseif device == "dout" and index>=8 then valid = false
			end

			if valid == true then
				if device == "relay" then
					-- Relay request
					atmega.sendCommand((newstate == "on" and "R" or "r") .. (index+1))
				else
					-- DOUT request
					atmega.sendCommand((newstate == "on" and "O" or "o") .. (index+1))
				end
				c:send("HTTP/1.0 200 OK\r\nConnection: close\r\n\r\n", close_connection)
			else
				-- Report 404 error, no such device.
				c:send("HTTP/1.0 404 Not found\r\nConnection: close\r\n\r\n", close_connection)
			end
		end
	end)
end)
