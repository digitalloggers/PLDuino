#pragma once
#include <Arduino.h>

class SimpleCommandProcessor {
public:
  typedef bool (*fnOnCommand)(const String &cmd);
  typedef void (*fnOnData)(const String &data);
  SimpleCommandProcessor (fnOnCommand fn_on_cmd, fnOnData fn_on_data);
  void analyze (const String &data);
public:
  static String makeCommand (const String &payload);
private:
  fnOnCommand on_cmd;
  fnOnData on_data;
  String buf;
};
