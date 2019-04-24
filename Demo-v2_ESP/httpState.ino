void httpState() {
  auto read_response = [](){
    String response = "";
    bool first_time = true;
    do{
      while(Serial.available())
        response += String(char(Serial.read()));
      
      delay(30);
      if (!Serial.available()) {
        if (first_time) {
          delay(400);
          first_time = false;
        } else {
          delay(150);
        }
      }
    }while(Serial.available());
    return response;
  };
  
  auto num = [](const String &numstr) {
    return String(numstr.toInt());
  };

  while(Serial.available()) Serial.read();
  Serial.print("{{~state~}}");
  String statestr = read_response();
  if (statestr.length() != (6+8+8+8*4+2+2+2+2+4+1)) {
    http.send(501, "text/plain", "Can't read state: " + String(statestr.length()) + statestr);
    return;
  }
  
  String json = "";

  json = "{\"relay\":[";
  int relaystart = 0;
  for(int i=0; i<6; ++i)
    json += (i>0? "," : "") + String(char(statestr[relaystart+i]));
  json += "]";

  json += ",";

  json += "\"input\":[";
  int inputstart = relaystart+6;
  for(int i=0; i<8; ++i)
    json += (i>0? "," : "") + String(char(statestr[inputstart+i]));
  json += "]";
  
  json += ",";

  json += "\"output\":[";
  int outputstart = inputstart+8;
  for(int i=0; i<8; ++i)
    json += (i>0? "," : "") + String(char(statestr[outputstart+i]));
  json += "]";

  json += ",";

  json += "\"analog\":[";
  int analogstart = outputstart + 8;
  for(int i=0; i<8; ++i) {
    int start = analogstart+i*4;
    json += (i>0? "," : "") + num(statestr.substring(start, start+4));
  }
  json += "]";

  json += ",";

  int timestart = analogstart + 8*4;
  json += String("\"time\":{") +
    "\"hour\":" + num(statestr.substring(timestart, timestart+2)) + "," +
    "\"minute\":" + num(statestr.substring(timestart+2, timestart+2+2)) +
  "}";
  
  json += ",";

  int datestart = timestart+2+2;
  json += String("\"date\":{") + 
    "\"year\":" + num(statestr.substring(datestart, datestart+4)) + "," +
    "\"month\":" + num(statestr.substring(datestart+4, datestart+4+2)) + "," + 
    "\"day\":" + num(statestr.substring(datestart+4+2, datestart+4+2+2)) +
  "}";
  
  json += "}";

  http.send(200, "application/json", json);
}


