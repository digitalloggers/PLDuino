+function() {
    function setupButtons() {
        for(let i=0; i<DOUTS_COUNT; ++i) {
            let idx = i+1;
            document.getElementById("dout"+idx+"on").onclick = function() {
                doAJAX("/dout?on=" + idx);
                return false;
            };
            document.getElementById("dout"+idx+"off").onclick = function() {
                doAJAX("/dout?off=" + idx);
                return false;
            };
        }
        for(let i=0; i<RELAYS_COUNT; ++i) {
            let idx = i+1;
            document.getElementById("relay"+idx+"on").onclick = function() {
                doAJAX("/relay?on=" + idx);
                return false;
            };
            document.getElementById("relay"+idx+"off").onclick = function() {
                doAJAX("/relay?off=" + idx);
                return false;
            };
        }
    }

    function updateView(state) {
        for(var i=0; i<DINS_COUNT; ++i)
            document.getElementById("din-status-" + (i+1)).className = (state.input[i]? "status-on" : "status-off");
        for(var i=0; i<DOUTS_COUNT; ++i)
            document.getElementById("dout-status-" + (i+1)).className = (state.output[i]? "status-on" : "status-off");
        for(var i=0; i<AINS_COUNT; ++i)
            document.getElementById("ain-status-" + (i+1)).innerHTML = state.analog[i];
        for(var i=0; i<RELAYS_COUNT; ++i)
            document.getElementById("relay-status-" + (i+1)).className = (state.relay[i]? "status-on" : "status-off");
        
        function pad (value, width, fill) {
            let str = value.toString();
            while(str.length < width)
                str = (fill || "0") + str;
            return str;
        }
        document.getElementById("datetime").innerHTML =
            pad(state.time.hour, 2) + ":" + pad(state.time.minute, 2) + " " +
            pad(state.date.year, 4) + "/" + pad(state.date.month, 2) + "/" + pad(state.date.day, 2);
    }

    function updateNames(cfg) {
        for(var i=0; i<DINS_COUNT; ++i) {
            document.getElementById("din-name-" + (i+1)).innerHTML = cfg.din_name[i];
        }
        for(var i=0; i<DOUTS_COUNT; ++i) {
            document.getElementById("dout-name-" + (i+1)).innerHTML = cfg.dout_name[i];
        }
        for(var i=0; i<AINS_COUNT; ++i) {
            document.getElementById("ain-name-" + (i+1)).innerHTML = cfg.ain_name[i];
        }
        for(var i=0; i<RELAYS_COUNT; ++i) {
            document.getElementById("relay-name-" + (i+1)).innerHTML = cfg.relay_name[i];
        }
    }

    window.onload = function() {
        function updateGUI() {
            doAJAX("/state", function(state) {
                try {
                    updateView(JSON.parse(state));
                } catch(e) {
                    console.log("can't parse");
                }
            });
            doAJAX("/configuration", function(cfg) {
                try {
                    updateNames(JSON.parse(cfg));
                } catch(e) {
                    console.log("can't parse");
                }
            });
        }
        setupButtons();
        updateGUI();
        setInterval(updateGUI, 1500);
    }
}()
