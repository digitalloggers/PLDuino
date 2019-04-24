DINS_COUNT = 8;
DOUTS_COUNT = 8;
AINS_COUNT = 8;
RELAYS_COUNT = 6;

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
