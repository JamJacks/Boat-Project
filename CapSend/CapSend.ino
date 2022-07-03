#include <SPI.h>


#define CAN_2515

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;

#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
   
void setup() {
    //Initial setup code for CAN bus send
    SERIAL_PORT_MONITOR.begin(115200);
    while(!Serial){};

    while (CAN_OK != CAN.begin(CAN_250KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
        
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
    
}


void loop() {
    
      unsigned char stmp[8]; //Define 8-byte array

      //The following reads analog values from potentiometers    
      int zero = analogRead(A0)*0.25;
//      if(zero > 0xC1 && zero < 0xD1){
//        zero = 0;
//      }
//      else if(zero >= 0xD1){
//        zero = 1;
//      }
      int one = analogRead(A1)*0.25;//*0.25 - 5;
//      if(one > 0xC1 && one < 0xD1){
//        one = 0;
//      }
//      else if(one >= 0xD1){
//        one = 1;
//      }
      int two = analogRead(A2)*0.25;//*0.25 - 5;
//      if(two > 0xC1 && two < 0xD1){
//        two = 0;
//      }
//      else if(two >= 0xD1){
//        two = 1;
//      }
      int three = analogRead(A3)*0.25 - 2;//*0.25 - 5;
//      if(three > 0xC1 && three < 0xD1){
//        three = 0;
//      }
//      else if(three >= 0xD1){
//        three = 1;
//      }
      
      int four = analogRead(A4);//*0.043 + 159;
      int five = analogRead(A5);//*0.0078125;
      unsigned int canId;// = four;
      bool hiLo;

      //CAN ID for RPM, Temps, ERR and Batt/ This code only cycles throught the four CAN IDs needed
      ////////////////////////////////////// to test the display. 
      if(four < 256){
        canId = 162; //TEMP
      }
      else if(four >= 256 && four < 512){
        canId = 202; //ERR
      }
      else if(four > 512 && four <= 768){
        canId = 192; //RPM
      }
      else{
        canId = 171; //BATT
      }
      
      //Set values for Hi-Low switch
      if(five < 500.0){
        hiLo = true;
      }
      if(five > 500.0){
        hiLo = false;
      }
      if(!hiLo){
        stmp[0] = zero;
        stmp[2] = one;
        stmp[4] = two;
        stmp[6] = three;
      }
      else{
        stmp[1] = zero;
        stmp[3] = one;
        stmp[5] = two;
        stmp[7] = three;
      }
      CAN.sendMsgBuf(canId, 0, 8, stmp);
      delay(100);         
    
    // send data per 100ms
    SERIAL_PORT_MONITOR.println(canId);
    
}

// END FILE


//////////////////////////////CODE GRAVEYARD -- DISREGARD////////////////////////////////////

//      const byte ROWS = 4; //four rows
//      const byte COLS = 4; //four columns
//      char keys[ROWS][COLS] = {
//        {1,2,3,10},
//        {4,5,6,11},
//        {7,8,9,12},
//        {15,0,14,13}
//      };
//      byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
//      byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
//      Keypad key = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//      int input = key.getKey();// Read the key
