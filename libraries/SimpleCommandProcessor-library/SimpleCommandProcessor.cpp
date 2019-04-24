#include "SimpleCommandProcessor.h"
#include <Arduino.h>

SimpleCommandProcessor::SimpleCommandProcessor (fnOnCommand fn_on_cmd, fnOnData fn_on_data)
  : on_cmd(fn_on_cmd),
    on_data(fn_on_data)
{
}

String SimpleCommandProcessor::makeCommand (const String &payload) {
  return "{{~" + payload + "~}}";
}

void SimpleCommandProcessor::analyze (const String &data) {
  const int MAX_COMMAND_LENGTH = 32;
  buf += data;
  while(true) {
    int pos_begin = buf.indexOf("{{~");
    if (pos_begin >= 0) {
      on_data(buf.substring(0, pos_begin));
      buf = buf.substring(pos_begin);
      int pos_end = buf.indexOf("~}}");
      if (pos_end >= 0) {
        if (!on_cmd(buf.substring(3, pos_end))) {
          on_data("{{~");
          buf = buf.substring(3);
        } else {
          buf = buf.substring(pos_end+3);
        }
      } else break;
    }
    else {
      break;
    }
  }
  if (buf.length() > MAX_COMMAND_LENGTH + 4) {
    int pos = buf.length() - (MAX_COMMAND_LENGTH+4);
    on_data(buf.substring(0, pos));
    buf = buf.substring(pos);
  }
}
