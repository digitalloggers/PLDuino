# PLDuino
PLDuino controller's repository for code, docs and other related stuff.

**First of all, install the necessary board definitions using these instructions: https://github.com/digitalloggers/plduino-arduinoide-package/blob/master/README.md**

Updated Demo to v2.0 from the experimental repo (Arduino/ESP8266 based). Removed the old NodeMCU part. Added SimpleCommandProcessor library, please copy it to your libraries/ folder.

Supporting assets are located in SD/ subfolders of both sketches (Demo-v2_Mega2560/SD and Demo-v2_ESP/SD). Copy the contents from Demo-v2_Mega2560/SD/ to the main SD card, and upload the files from Demo-v2_ESP/SD/ into ESP-02's internal "SD" storage (after flashing its part of the Demo) using built-in WebFileManager on port 9999 (http://192.168.4.1:9999/). See also WebFileManager library: https://github.com/digitalloggers/ESP8266WebFileManager-library
