//*************************************************************************
// X10oLEDDemo.ino
// by RCI
// Connect the NodeMCU to the X10 PSC04 PowerLine Module
// to control X10 Devices.
// Requires X10RCI.h and X10RCI.cpp
// Uses SSD1306 0.96" oLED module.
// IC2 Display on pins SDA = D2, SCL = D1.
//*************************************************************************
#include <Wire.h> 
#include <SSD1306Wire.h> 

#include <SPI.h>
#include <X10RCI.h>        // X10RCI Library,  See X10RCI.cpp and X10RCI.h

// Variables
//*************************************************************************
String stringOn = "ON ";
String stringOff = "OFF";
String housecode = "A";

char h_code, i;
char txt[4];

SSD1306Wire  display(0x3c, 4, 5);
#define ZC 13     // D7 Output from X10  BLACK
#define TRANS 14  // D5 Input to X10     YELLOW
                     // RED and GREEN to GND
#define ON 20
#define OFF 28
#define row1 0
#define row2 18
#define row3 36

X10 X10(ZC, TRANS);     // ZC = Black, Trans = Yellow
//*********************************************************************************
void OledText(char col, char row, String dtext) 
{  
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(col, row, dtext);
  display.display(); 
}
//*********************************************************************************
void OledNumber(char col, char row, int integer) 
{  
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(col, row, String (integer));
  display.display(); 
}
//*********************************************************************************
void X10Display(char row, String hcode, int chan, char OnOff)
{
  String msg;
  int str_len = hcode.length() + 1;
  char hc[str_len];
  char state;
  if(OnOff == OFF)
     {
     msg = stringOff;
     state = OFF; 
     }
  if(OnOff == ON)
     {
     msg = stringOn; 
     state = ON;
     } 
  hcode.toCharArray(hc, str_len);      
  display.clear();
  OledText(0, row1, "X10oLED"); 
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, row3, hcode);
  display.drawString(20, row3, String (chan));
  display.drawString(50, row3, msg);
  display.display();
  //X10.X10Trans('A', chan, state);
  X10.X10Trans(hc[0], chan, state);
}
//**************************************************************************
void setup() 
{
  display.init();

  OledText(0, row1, "X10oLED");
  delay(2000); 

}
//*****************************************************************
void loop() 
{


    X10Display(row3, housecode, 10, ON);          // House codes A - P,  Channels 0 - 15
    delay(5000);
 
    X10Display(row3, housecode, 10, OFF);          // House codes A - P,  Channels 0 - 15
    delay(5000);

}
