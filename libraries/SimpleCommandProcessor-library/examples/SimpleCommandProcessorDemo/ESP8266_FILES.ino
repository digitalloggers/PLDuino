#ifdef ARDUINO_ARCH_ESP8266

const char *INDEX_HTML = R"HTML(
  <head>
    <script type="text/javascript" src="index.js"></script>
    <style>
      p { font: 12pt serif }
      span { font-weight: bold; color: blue; }
    </style>
  </head>
  <body>
    <p>DIN1: <span id="din-state-0"></span></p>
    <p>DIN2: <span id="din-state-1"></span></p>
    <p>DIN3: <span id="din-state-2"></span></p>
    <p>DIN4: <span id="din-state-3"></span></p>
    <p>DIN5: <span id="din-state-4"></span></p>
    <p>DIN6: <span id="din-state-5"></span></p>
    <p>DIN7: <span id="din-state-6"></span></p>
    <p>DIN8: <span id="din-state-7"></span></p>
  </body>
)HTML";

const char *INDEX_JS = R"JS(
  function doAJAX(url, callback) {
    var request = (window.ActiveXObject && new ActiveXObject('Microsoft.XMLHTTP')) ||
                  (window.XMLHttpRequest && new XMLHttpRequest());
    if (request) {
      request.onreadystatechange = function(object) {
        if (request.readyState == 4 && callback)
          callback(request.responseText);
      };
      request.open("POST", url, true);
      request.send("");
    } else {
      console.log("XMLHttpRequest not found");
    }
  };
  
  window.onload = function() {
    function updateView(state) {
      for(var i=0; i<8; ++i)
        document.getElementById("din-state-"+i).innerHTML = (state.dins.charAt(i)=='H'? "HIGH" : "LOW");
    }
    function updateGUI() {
      doAJAX("/state", function(state) {
        try {
          updateView(JSON.parse(state));
        } catch(e) {
          console.log("can't parse");
        }
      });
    }
    updateGUI();
    setInterval(updateGUI, 500);
  }
)JS";

#endif
