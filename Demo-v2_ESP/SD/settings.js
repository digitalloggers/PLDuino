window.onload = function() {
  document.getElementById("datetime-sync-with-clock").onclick = function() {
    var now = new Date();
    document.getElementById("datetime-year").value = now.getFullYear();
    document.getElementById("datetime-month").value = now.getMonth()+1;
    document.getElementById("datetime-day").value = now.getDate();
    document.getElementById("datetime-hour").value = now.getHours();
    document.getElementById("datetime-minute").value = now.getMinutes();
  };

  function loadCurrentConfiguration() {
    function loadDateTime() {
      doAJAX("/state", function(response) {
        try {
          var state = JSON.parse(response);
          document.getElementById("datetime-year").value = state.date.year;
          document.getElementById("datetime-month").value = state.date.month;
          document.getElementById("datetime-day").value = state.date.day;
          document.getElementById("datetime-hour").value = state.time.hour;
          document.getElementById("datetime-minute").value = state.time.minute;
        } catch(e) {
          console.log("Can't parse.");
          console.log(response);
          console.log(e);
          setTimeout(loadDateTime, 200);
        }
      });
    }
    loadDateTime();

    doAJAX("/configuration", function(response) {
      try {
        var cfg = JSON.parse(response);
          for(var i=0; i<RELAYS_COUNT; ++i) {
            document.getElementById('relay-name-'+(i+1)).value = cfg.relay_name[i];
            document.getElementById('relay-echo-name-'+(i+1)).value = cfg.relay_echo_name[i];
            document.getElementById('relay-echo-enabled-'+(i+1)).checked = (cfg.relay_echo_on[i] == 1);
          }
          for(var i=0; i<DOUTS_COUNT; ++i) {
            document.getElementById('dout-name-'+(i+1)).value = cfg.dout_name[i];
          }
          for(var i=0; i<DINS_COUNT; ++i) {
            document.getElementById('din-name-' + (i+1)).value = cfg.din_name[i];
          }
          for(var i=0; i<AINS_COUNT; ++i) {
            document.getElementById('ain-name-' + (i+1)).value = cfg.ain_name[i];
          }
          document.getElementById("wifi-ssid").value = cfg.wifi.ssid;
          document.getElementById("wifi-password").value = cfg.wifi.password;
          document.getElementById("wifi-mode-sta").checked = (cfg.wifi.mode == "sta");
          document.getElementById("wifi-mode-ap").checked = (cfg.wifi.mode != "sta");
      } catch(e) {
        console.log("Can't parse.");
        console.log(response);
        console.log(e);
      }
    });
  }

  loadCurrentConfiguration();
};
