-- Wi-Fi credentials.
local cfg = {
	ssid = "Wi-Fi Test",
	pwd = "password"
}

-- Print some useful information.
print("setting up Wi-Fi...")
print("SSID: \"" .. cfg.ssid .. "\" password: \"" .. cfg.pwd .. "\"")
wifi.setmode(wifi.SOFTAP)
wifi.ap.config(cfg)
print(wifi.ap.getip())
print("\ntype \"qw\" to cancel server startup.\n")

-- Start server after 2.5 sec.
tmr.alarm(0, 2500, 0, function()
	uart.on("data") -- stop listening UART.
	print "starting server..."
	require "server"
end)

-- Listen to UART data and stop the above timer in case of "qw" string was received.
uart.on("data", "w", function(d)
	if d == "qw" then
		tmr.stop(0) -- Stop timer. Server won't start.
		uart.on("data")
		print "cancelled."
	end
end, 0)
