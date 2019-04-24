void httpSwitchDOUT() {
  String cmd;
  if (http.hasArg("on"))
    cmd = "O" + http.arg("on");
  else if (http.hasArg("off"))
    cmd = "o" + http.arg("off");
  Serial.print(SimpleCommandProcessor::makeCommand(cmd));
  http.send(200, "text/plain", "complete");
}


void httpSwitchRelay() {
  String cmd;
  if (http.hasArg("on"))
    cmd = "R" + http.arg("on");
  else if (http.hasArg("off"))
    cmd = "r" + http.arg("off");
  Serial.print(SimpleCommandProcessor::makeCommand(cmd));
  http.send(200, "text/plain", "complete");  
}


void httpNotFound() {
  Serial.println(http.uri() + " 404");
  http.send(404, "text/plain", "404 Not Found");  
}


