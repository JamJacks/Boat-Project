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

void setup(){
    SERIAL_PORT_MONITOR.begin(115200);

    while (CAN_OK != CAN.begin(CAN_250KBPS)) {             // init can bus : baudrate = 250k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}


void loop() { //Begin Loop
    unsigned char len = 5;
    unsigned char buf[8];
    

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();

        SERIAL_PORT_MONITOR.println("-----------------------------");
        SERIAL_PORT_MONITOR.print("Get data from ID: ");
        SERIAL_PORT_MONITOR.println(canId);
          
        //lcd.setCursor(4, 3);
        //SERIAL_PORT_MONITOR.println(len);
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
       
        //*****************************************************************************************
        //***************************************Display Status***************************************
        if(canId == 0x01dd0001){
          SERIAL_PORT_MONITOR.print("getting status indicator");
          SERIAL_PORT_MONITOR.print("\t");
          decStat(buf);
          
          checkVolt();          
        }

        //****************************************************************************************
        //**************************************Display battery cell voltage********************************

        //apply LTC number mask
        //init_Mask(unsigned char num, unsigned char ext, unsigned char ulData);
        
        //apply Cell number mask
        //init_Mask(unsigned char num, unsigned char ext, unsigned char ulData);
        
        //****************************************************************************************
        //**************************************Display invertor temp*****************************
        
        //****************************************************************************************
        //**************************************Display battery temp******************************
        //NOTE: Extract bitpos and length from BMS .dbc (database file).
        //      The Battery Management System will supply you with appropriate CAN messages for the
        //      real-time battery module temperatures. All other values in this block should be 
        //      sufficient.
        //if(canId == ?){ //CAN ID from BMS (Replace '?')
         
        //}
        //****************************************************************************************
        
        SERIAL_PORT_MONITOR.println();

      
        
    }    
} //End of loop. 

void checkVolt (){
  unsigned char len = 5;
  unsigned char buf[8];
  //loop through all the bms modules and ltcs it wants info from 
  long addHold;
  long iHold;
  long sendHold;
  for (int i=0; i<3; i++){
    iHold=i<<4;
    addHold = 0x01de0800;;
    for (int j=0; j<7; j++){              
      addHold = addHold|iHold;
      addHold = addHold|j;
      CAN.sendMsgBuf(addHold, 1, 8, 0);             
      for (int k=0; k<2; k=k+1){
        sendHold = 0x01df0900;
        sendHold = sendHold + (0x00 * k);
        sendHold=sendHold|iHold;
        sendHold=sendHold|j;
               
        if(CAN_MSGAVAIL == CAN.checkReceive()){
          CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
          unsigned long canId = CAN.getCanId();

          //******************************************************************************************
          //************************Display CAN Message on bottom line********************************    
          for (byte h = 0; h < len; h++) { // print the data            
            SERIAL_PORT_MONITOR.print(buf[h], HEX);
            SERIAL_PORT_MONITOR.print("\t");              
          } 
          if((canId>>8 == 0x01df09)& (sendHold>>8 == 0x01df09)){          
            SERIAL_PORT_MONITOR.println("cells 1-4");
            getCell (buf[8],1);       
          }   
          if((canId>>8 == 0x01df0a) & (sendHold>>8 == 0x01df0a)){          
            SERIAL_PORT_MONITOR.println("cells 5-8");   
            getCell (buf[8],5);    
          }
          if((canId>>8 == 0x01df0b2) & (sendHold>>8 == 0x01df0b)){          
            SERIAL_PORT_MONITOR.println("cells 9-12");  
            getCell (buf[8],9);     
          }
        }
      }                                                
    }
  }
}

void getCell (unsigned char buf[8], int cell){
  long hold;
   SERIAL_PORT_MONITOR.print("cell " ": ");
   SERIAL_PORT_MONITOR.print(cell);
   SERIAL_PORT_MONITOR.println(": ");
   hold = buf[1];
   hold=hold<<8;
   hold=hold|buf[0];
   hold = ((float)hold/10000);
   SERIAL_PORT_MONITOR.println(hold, 4);
          
   SERIAL_PORT_MONITOR.print("cell " ": ");
   SERIAL_PORT_MONITOR.print(cell+1);
   SERIAL_PORT_MONITOR.println(": ");
   hold = buf[3];
   hold=hold<<8;
   hold=hold|buf[2];
   hold = ((float)hold/10000);
   SERIAL_PORT_MONITOR.println(hold, 4);
         
   SERIAL_PORT_MONITOR.print("cell " ": ");
   SERIAL_PORT_MONITOR.print(cell+2);
   SERIAL_PORT_MONITOR.println(": ");         
   hold = buf[5];
   hold=hold<<8;
   hold=hold|buf[4];
   hold = ((float)hold/10000);
   SERIAL_PORT_MONITOR.println(hold, 4);
          
   SERIAL_PORT_MONITOR.print("cell " ": ");
   SERIAL_PORT_MONITOR.print(cell+3);
   SERIAL_PORT_MONITOR.println(": ");
   hold = buf[7];
   hold=hold<<8;
   hold=hold|buf[6];
   hold = ((float)hold/10000);
   SERIAL_PORT_MONITOR.println(hold, 4);
   SERIAL_PORT_MONITOR.print("\t");
}

void decStat(unsigned char buf[8]){
  //status definitions
  int CELL_HVC = 0x01; // at least one cell v is >HVC  
  int CELL_LVC = 0x02; // at least one cell v is <LVC
  int CELL_BVC = 0x04; // at least one cell v is >BVC
  
  // BMS flag definitions
  int NOT_LOCKED = 0x01; // configuration not locked
  int FAULT_CENSUS = 0x02; // not all cells present
  int FAULT_OVERTEMP = 0x04; // thermistor overtemp
  int THERM_CENSUS = 0x08; // not all thermistors present

  int ltcNum = buf[4];
  int ifErr = buf[3];
  int fault = buf[2];
  int CANid = buf[1];
  int stat = buf[0];

  //display status
  if(stat & CELL_HVC == CELL_HVC){
    SERIAL_PORT_MONITOR.println("at least one cell v is >HVC ");
    //loop throught the proper check to see which cell(s) is too high
  }

  if(stat & CELL_LVC == CELL_LVC){
    SERIAL_PORT_MONITOR.println("at least one cell v is <LVC ");
    //loop throught the proper check to see which cell(s) is too low
  }

  if(stat & CELL_BVC == CELL_BVC){
    SERIAL_PORT_MONITOR.println("at least one cell v is >BVC ");
    //loop throught the proper check to see which cell(s) are unbalanced
  }

  //checking for errors
  if(ifErr == 1){
    if(fault & NOT_LOCKED == NOT_LOCKED){
      SERIAL_PORT_MONITOR.println("configuration not locked");      
    }

    if(fault & FAULT_CENSUS == FAULT_CENSUS){
      SERIAL_PORT_MONITOR.println("not all cells present");
      //loop throught the proper check to see which cell is disconnected      
    }

    if(fault & FAULT_OVERTEMP == FAULT_OVERTEMP){
      SERIAL_PORT_MONITOR.println("at least one thermistor is over temp"); 
      //loop through the thermistors to see which battery is too high           
    }

    if(fault & THERM_CENSUS == THERM_CENSUS){
      SERIAL_PORT_MONITOR.println("not all thermistors are present");
      //loop through the thermistors to see which is disconnected      
    }
  }
}

void checkTemp(long canId, unsigned char buf[8]){        
  //*****************************************************************************************
  //**********************************Display battery temp message(s)*******************************
  if(canId == 0x01df0e00){
    //SERIAL_PORT_MONITOR.print("\t");
    SERIAL_PORT_MONITOR.print("temp 1");
    SERIAL_PORT_MONITOR.print("\t");
  }

  if(canId == 0x01df0e10){
    SERIAL_PORT_MONITOR.print("temp 2");
    SERIAL_PORT_MONITOR.print("\t");
  }

  if(canId == 0x01df0e20){
    SERIAL_PORT_MONITOR.print("temp 3");
    SERIAL_PORT_MONITOR.print("\t");
  }

  if(canId == 0x01df0e30){
    SERIAL_PORT_MONITOR.print("temp 4");
    SERIAL_PORT_MONITOR.print("\t");
  }
       
}
