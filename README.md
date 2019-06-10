# arduino_romdump
https://github.com/phooky/PROM/tree/master/tools/eeprom_read but with Intel hex output over serial. From the article at link: https://www.nycresistor.com/2012/07/07/stick-a-straw-in-its-brain-and-suck-how-to-read-a-rom/

I just added the intel hex output part.

Had errors dumping the one EPROM i updated this program for, so i'm unsure if this works 100% fine.

# Using (under GNU/Linux)
Follow the article until the code part, flash this code and then run `cat /dev/ttyUSBn > dump.hex`, take a peek at the file sometimes to see if it has finished, if it has then interrupt the `cat`ting of ttyUSBn and use `objcopy -I ihex -O binary dump.hex dump.bin` to produce a binary of the ihex file. **Make sure to specify the output filename, it will overwrite the hex file otherwise**
