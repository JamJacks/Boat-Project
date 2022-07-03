# Boat-Project

## Setup
The boat controller communicates with all the external systems using CAN. In order to do this, a CAN Arduino sheild. 
You have to download external libraries for the LCD and CAN functionalities. You may have to change syntax because of slightly different libraries, please let me know if you run into any weird errors while compiling. 
https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library (LCD display library)
https://github.com/Seeed-Studio/Seeed_Arduino_CAN (CAN library)
https://wiki.seeedstudio.com/CAN-BUS_Shield_V2.0/ (instructions for using CAN library)

If you go to tools -> serial monitor within the arduino IDE, and set the baud rate to 115200, you can see a running log of all the CAN messages being received by the board. The port under tools->port should be set to whichever port the I2C_LED board is connected to.

Eventually the I2C and BMS codes will have to be combined, I'm developing them seperately for now for testing purposes.

### Test system
## CapSend
Sends emulated CM350 CAN signals. Can be used instead of CM350 for testing. Potentiometers are used as mock inverter inputes to set different motor speeds, temperatures, etc.

## I2C_LED
Receives CAN messages from the CM350 and displays them on the LCD screen via I2C.

## BMS_code
Intakes signals from the BMS and warns the user of fault codes, temperatures, etc.
