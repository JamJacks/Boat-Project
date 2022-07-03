#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <stdio.h>
#include <string.h>


const int ledPin =  LED_BUILTIN;// the number of the LED pin
LiquidCrystal_I2C lcd(0x27, 20, 4); 
#define CAN_2515

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin

void setup()
{
  
  //lcd.init();                  
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("B:");
  lcd.setCursor(0, 1);
  lcd.print("RPM:");
  lcd.setCursor(0, 2);
  lcd.setCursor(14, 0);
  lcd.print("M:");
  lcd.setCursor(14, 1);
  lcd.print("I:");
  lcd.setCursor(14, 2);
  lcd.print("B:");
//  lcd.setCursor(19, 2);
//  lcd.print("F");
 


  
  
  SERIAL_PORT_MONITOR.begin(115200);

    while (CAN_OK != CAN.begin(CAN_250KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////ERROR MESSAGE FUNCTIONS///////////////////////////////////////
//NOTE: The (F(_____)); macro is important! It changes the strings to local variables instead/
////////of global variables. If you take these out, your board will take a huge dump./////////
//////////////////////////////////////////////////////////////////////////////////////////////

void sendError0(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("Gate/Desaturation   "));
    delay(1000);                           
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("HW Over-current     "));
    delay(1000);           
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("Accelerator Shorted "));
    delay(1000);           
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("Accelerator Open    "));
    delay(1000);           
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("Current Sensor Low  "));
    delay(1000);  
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("Current Sensor High "));
    delay(1000); 
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("Module Temp Low     "));
    delay(1000); 
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("Module Temp High    "));
    delay(1000);    
  }
}
void sendError1(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);          
    lcd.print(F("Cont PCB Temp Low   "));
    delay(1000);  
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("Cont PCB Temp High  "));
    delay(1000);          
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("Gate Dr PCB TempLow "));
    delay(1000);             
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("Gate Dr PCB TempHigh"));
    delay(1000); 
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("5V Sense V Low      "));
    delay(1000);   
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("5V Sense V High     "));
    delay(1000);  
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("12V Sense V Low     "));
    delay(1000);
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("12V Sense V High    "));
    delay(1000);     
  }
}
void sendError2(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("2.5V Sense V Low    "));
    delay(1000);  
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("2.5V Sense V High   "));
    delay(1000);    
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("1.5V Sense V Low    "));
    delay(1000);
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("1.5V Sense V High   "));
    delay(1000);
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("DC Bus V High       "));
    delay(1000);
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("DC Bus V Low        "));
    delay(1000);
            }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("Pre-charge Timeout  "));
    delay(1000);
  }
  if(buf && 0x80){
     lcd.setCursor(0, 3);
     lcd.print(F("Pre-charge V Fail   "));
     delay(1000);
  }
}
void sendError3(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("EEPROM ChecksumInval"));
    delay(1000);
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("EEPROM DataOutRange "));
    delay(1000);
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("EEPROM Update Req   "));
    delay(1000);
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("DC Bus Over-V       "));
    delay(1000);
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("Gate Driver Init    "));
    delay(1000);
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("Reserved            "));
    delay(1000);
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("Brake Shorted       "));
    delay(1000);
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("Brake Open          "));
    delay(1000);
  }
}
void sendError4(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("Motor Over-speed    "));
    delay(1000);
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("Over-current        "));
    delay(1000);
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("Over-voltage        "));
    delay(1000);
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("Inverter Over-temp  "));
    delay(1000);
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("Accel Input Shorted "));
    delay(1000);
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("Accel Input Open    "));
    delay(1000);
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("Direction Command   "));
    delay(1000);
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("Inv Response Timeout"));
    delay(1000);
  }
}
void sendError5(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("Hardware Gate/Desat "));
    delay(1000);
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("HardwareOver-current"));
    delay(1000);
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("Under-voltage Fault "));
    delay(1000);
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("CAN CommandMsgLost  "));
    delay(1000);
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("Motor Over-temp     "));
    delay(1000);
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("Reserved            "));
    delay(1000);
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("Reserved            "));
    delay(1000);
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("Reserved            "));
    delay(1000);
  }
}
void sendError6(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("Brake Input Shorted "));
    delay(1000);
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("Brake Input Open    "));
    delay(1000);
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("Module A Over-temp  "));
    delay(1000);
  }
  if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("Module B Over-temp  "));
    delay(1000);
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("Module C Over-temp  "));
    delay(1000);
    lcd.setCursor(0, 3);
  }
  if(buf && 0x20){
    lcd.setCursor(0, 3);
    lcd.print(F("PCB Over-temp       "));
    delay(1000);
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("GateDriveBoard1 temp"));
    delay(1000);
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("GateDriveBoard2 temp"));
    delay(1000);
  }
}
void sendError7(unsigned char buf){
  if(buf && 0x1){
    lcd.setCursor(0, 3);
    lcd.print(F("GateDriveBoard3temp "));
    delay(1000);
  }
  if(buf && 0x2){
    lcd.setCursor(0, 3);
    lcd.print(F("Current Sensor      "));
    delay(1000);
  }
  if(buf && 0x4){
    lcd.setCursor(0, 3);
    lcd.print(F("Gate Driver Over-V  "));
    delay(1000);
  }
    if(buf && 0x8){
    lcd.setCursor(0, 3);
    lcd.print(F("Reserved            "));
    delay(1000);
  }
  if(buf && 0x10){
    lcd.setCursor(0, 3);
    lcd.print(F("Hardware DCBus OverV"));
    delay(1000);
  }
  if(buf && 0x20){          
    lcd.setCursor(0, 3);         
    lcd.print(F("Reserved            "));
    delay(1000);
  }
  if(buf && 0x40){
    lcd.setCursor(0, 3);
    lcd.print(F("ResolverNotConnected"));
    delay(1000);
  }
  if(buf && 0x80){
    lcd.setCursor(0, 3);
    lcd.print(F("Reserved            "));
    delay(1000);
  }
}


void loop() { //Begin Loop
    
//LED setup
    
    int battDanLED=7;
    pinMode(battDanLED, OUTPUT);
    int battWarnLED=6;
    pinMode(battWarnLED, OUTPUT);
    int battTempLED=5;
    pinMode(battTempLED, OUTPUT);
    int motorTempLED=4;
    pinMode(motorTempLED, OUTPUT);
    int directionLED=3;
    pinMode(directionLED,OUTPUT); 
  
    //digitalWrite(motorTempLED, HIGH);
    //digitalWrite(battWarnLED, HIGH);
    //digitalWrite(battDanLED, HIGH);
    //digitalWrite(battTempLED, HIGH);
    //digitalWrite(directionLED,HIGH);

    
    //Warning value variables NEEDS TO BE CHANGED AFTER VALUE VERIFICATION
    int battDan = 5;
    int battWarn = 20;
    int maxMotorTemp = 100;
    int maxBatteryTemp = 100;  

    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        SERIAL_PORT_MONITOR.println("-----------------------------");
        SERIAL_PORT_MONITOR.print("Get data from ID: ");
        SERIAL_PORT_MONITOR.println(canId);
            
        //lcd.setCursor(4, 3);
        
        for (byte i = 0; i < len; i++) { // print the data
            SERIAL_PORT_MONITOR.print(buf[i], HEX);
            SERIAL_PORT_MONITOR.print("\t");
        //******************************************************************************************
        //************************Display CAN Message on bottom line********************************   
            //lcd.print(buf[i], HEX);
        }
        //******************************************************************************************
        //************************************Display Battery charge********************************
        //TODO-if/while for CAN ID (BMS)
        //NOTE: Values semi-arbitrary, based on battery max discharge current. Actual values may need to
        //      be calculated once the battery modules are in hand and analyzed. Current values rely on max of
        //      1000 equaling a 100% battery charge. No values surpassing 1000 will be counted. 
        if(canId == 202){
          lcd.setCursor(2, 0);
          unsigned long chargeIn = ((buf[0] << 8) | buf[1]);
          if(chargeIn > 1000.0){
            chargeIn = 1000.0;
          }
          double charge = chargeIn*0.1;
          if(charge <= battWarn && charge > battDan){
            digitalWrite(battWarnLED, HIGH);
            digitalWrite(battDanLED, LOW);
          } else if(charge < battDan){
            digitalWrite(battDanLED, HIGH);
            digitalWrite(battWarnLED, LOW);
          } else {
            digitalWrite(battDanLED, LOW);
            digitalWrite(battWarnLED, LOW);
          }
          if(charge < 100){
            lcd.print(" ");
            lcd.print(charge, 0);
            lcd.print("%      ");
          }
          else{
            lcd.print(charge, 0);
            lcd.print("%      ");
          }
        }
        //*****************************************************************************************
        //***************************************Display RPM***************************************
        if(canId == 192){ 
          lcd.setCursor(4, 1);
          //double rpm = buf[7]/35.0;//Test value *to-do
          int rpmIn = ((buf[2] << 8) | buf[3]);
          static_cast<int> (rpmIn);
          double rpm = rpmIn / 1000.0; //
          if(rpm<0){
            digitalWrite(directionLED,HIGH);
          } else {
            digitalWrite(directionLED, LOW);
          }
          lcd.print(rpm, 1);
          lcd.print("k  ");
        }
        //*****************************************************************************************
        //**********************************Display ERROR message(s)*******************************
        if(canId == 171){ 
          lcd.setCursor(0, 2); //Post Lo
          lcd.print("FAULT MESSAGE");
          lcd.setCursor(0, 3);
          ////Call Byte 0 error messages
          if(buf[0] != 0){
            sendError0(buf[0]);
          }
          ////Call Byte 1 error messages
          if(buf[1] != 0){
            sendError1(buf[1]);
          }
          ////Call Byte 2 error messages
          if(buf[2] != 0){
            sendError2(buf[2]);
          }
          ////Call Byte 3 error messages
          if(buf[3] != 0){
            sendError3(buf[3]);
          }
          ////Call Byte 4 error messages
          if(buf[4] != 0){
            sendError4(buf[4]);
          }
          ////Call Byte 5 error messages
          if(buf[5] != 0){
            sendError5(buf[5]);
          }
          ////Call Byte 6 error messages
          if(buf[6] != 0){
            sendError6(buf[6]);
          }
          ////Call Byte 7 error messages
          if(buf[7] != 0){
            sendError7(buf[7]);  
          }
        }
        else{
          lcd.setCursor(0, 2);
          lcd.print("              ");
          lcd.setCursor(0, 3);
          lcd.print("                    "); 
        }
        //****************************************************************************************
        //**************************************Display motor temp********************************
        if(canId == 162){ 
        lcd.setCursor(16, 0);
        //int mTemp = buf[7]/2 + 50;//Test value *to-do
          int mTemp = ((buf[4] << 8) | buf[5]);
          static_cast<int>(mTemp); //Cast to signed int
          
          double motorTemp = (mTemp/10)*1.8 + 32; //Convert to F from C and factor
          if(motorTemp >= maxMotorTemp){
            digitalWrite(motorTempLED, HIGH); 
          }
          if(motorTemp < maxMotorTemp){
            digitalWrite(motorTempLED, LOW); 
          }
          if(motorTemp < 100){
            lcd.print(" ");
            lcd.print(motorTemp, 0);
            lcd.print("F");
          }
          else{
            lcd.print(motorTemp, 0);
            lcd.print("F");
          }
        }
        //****************************************************************************************
        //**************************************Display invertor temp*****************************
        if(canId == 162){
          lcd.setCursor(16, 1);
          //int iTemp = buf[7]/3 + 50;//Test value
          int iTemp = ((buf[2] << 8) | buf[3]);
          static_cast<int>(iTemp); //Cast to signed int
          double invertorTemp = (iTemp/10)*1.8 + 32; //Convert to F from C and factor
          if(invertorTemp < 100){
            lcd.print(" ");
            lcd.print(invertorTemp, 0);
            lcd.print("F");
          }
          else{
            lcd.print(invertorTemp, 0);
            lcd.print("F");
          }
        }
        //****************************************************************************************
        //**************************************Display battery temp******************************
        //NOTE: Extract bitpos and length from BMS .dbc (database file).
        //      The Battery Management System will supply you with appropriate CAN messages for the
        //      real-time battery module temperatures. All other values in this block should be 
        //      sufficient.
        //if(canId == ?){ //CAN ID from BMS (Replace '?')
          lcd.setCursor(16, 2);
          int bTemp = 100; //**test**
          static_cast<int>(bTemp); //Cast to signed int
          double batteryTemp = (bTemp/10)*1.8 + 32; //Convert to F from C and factor
          if(batteryTemp >= maxBatteryTemp){
            digitalWrite(motorTempLED, HIGH); 
          }
          if(batteryTemp < maxBatteryTemp){
            digitalWrite(motorTempLED, LOW); 
          }
          if(batteryTemp < 100){
            lcd.print(" ");
            lcd.print(batteryTemp, 0);
            lcd.print("F");
          }
          else{
            lcd.print(batteryTemp, 0);
            lcd.print("F");
          }
        //}
        //****************************************************************************************
        
        SERIAL_PORT_MONITOR.println();
        
    }       
} //End of loop. 
