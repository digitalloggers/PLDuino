@ECHO OFF
REM *** Don't forget to adjust this ***
set PORT=COM3
set ARDUINO_PATH=C:\Arduino

echo NodeMCU on PLDuino on %PORT% is about to be updated.
echo.
echo WARNING! ALL USER LUA SCRIPTS WILL BE DESTROYED!
echo.
pause

rem -----------------------------------------------------------------------------------------------------------
set AVRDUDE_EXE=%ARDUINO_PATH%\hardware\tools\avr\bin\avrdude.exe
set AVRDUDE_CONF=%ARDUINO_PATH%\hardware\tools\avr\etc\avrdude.conf

CD BIN
%AVRDUDE_EXE% -D -v -c wiring -p ATmega2560 -P %PORT% -b 115200 -U flash:w:sys_gp0_low.ino.mega.hex:i -C %AVRDUDE_CONF%
python esptool.py --port %PORT% --baud 115200 write_flash 0x00000 at.bin
python esptool.py --port %PORT% --baud 115200 erase_flash
python esptool.py --port %PORT% --baud 115200 write_flash 0x00000 0x00000.bin
python esptool.py --port %PORT% --baud 115200 write_flash 0x10000 0x10000.bin
python esptool.py --port %PORT% --baud 115200 write_flash 0x7c000 esp_init_data_default.bin
%AVRDUDE_EXE% -D -v -c wiring -p ATmega2560 -P %PORT% -b 115200 -U flash:w:sys_gp0_high.ino.mega.hex:i -C %AVRDUDE_CONF%

ECHO PLEASE DON'T CONNECT TO %PORT% FOR 20min TO ALLOW FLASH TO BE PROPERLY FORMATTED BY THE FIRMWARE.
