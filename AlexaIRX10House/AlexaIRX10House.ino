//************************************************************************************************
// AlexaHAL.ino
//
// For X10 and Media Center Control using Alexa commands
//
// "ircodes.h" contains the IR Codes needed for your Media Center.
// IR Codes were recorded with IRrecvDemoOLED.ino.
// Copy your received codes to ircodes.h
//
// Added computerMacro, dvdMacro, and tvMacro examples for creating IR macros
// to control your Media Center.  Since every Media Center is different, use this
// as a guide.  Flags are used to remember the last state of your equipment, either ON or OFF.
//
// Added "void updateMacroList()" and Time Libraries for timed X10 events.
//
// Added Webpage "ON" and "OFF" push buttons.
//
// Thanks to author Christian Schwinne for the great Espalexa library!
// 
// Modified by RCI 10-5-2020
//************************************************************************************************
#define ESPALEXA_MAXDEVICES 20
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WifiUDP.h>
#include <Wire.h> 
#include <SSD1306Wire.h>      
#include <SPI.h>
#include <Espalexa.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "ircodes.h"            // For RCI IR Components
#include <X10RCI.h>             // X10RCI Library,  See X10RCI.cpp and X10RCI.h
#include <NTPClient.h>
#include <Time.h>
#include <TimeLib.h>
#include <Timezone.h>

// Define NTP properties
#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "us.pool.ntp.org"  // change this to whatever pool is closest (see ntp.org)
// Set up the NTP UDP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

const char* ssid     = "your_ssid";
const char* password = "your_password";

#define ZC 13      // D7 Output from X10  YELLOW
#define TRANS 14   // D5 Input to X10     BLACK
#define IRled 12   // TX
#define ON 20
#define OFF 28
#define SDA 4  // TX
#define SCL 5  // RX
X10 X10(ZC, TRANS);     // ZC(1) = D7, Trans(4) = D5, GND(3) = Gnd, GND(2) = Gnd

// ***** Variables ********************************************************
int value = HIGH;            
byte incomingByte;         // from the X10 controller or the keyboard
byte Housecode_Devicecode; // for the Serial.write third byte
byte Housecode_Function;   // for the Serial.write third byte
byte luminance = 0x00;     // brightness and dim value
int row1 = 0;
int row2 = 12;
int row3 = 30;
int row4 = 48;
int period1sec = 1000;
int period1ms = 1;
unsigned long time_now = 0;
String ipString;
String stringOn = "ON ";
String stringOff = "OFF";
String housecode = "A";
String x10String = "";
boolean wifiConnected = false;
boolean latch_bit = 0;
int count, countFast;
int currentHours, currentMinutes, currentSeconds, currentYear, currentDay;
boolean tvPowFlag, dvdPowFlag, rcvPowFlag, satFlag, dvdFlag, cdFlag, tvFlag, hdmiFlag, smartcastFlag;
boolean toggle;
unsigned char chan;
unsigned char i, chanFlag[20];
// ***** Time and Date Variables Here *******
String date, dateShort, currentMonth;
String tyme, tymeShort, tymeHours, tymeMinutes, tymeSeconds;
const char * days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"} ;
const char * months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"} ;
const char * monthNum[] = {" 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", "11", "12"} ;
const char * ampm[] = {"AM", "PM"} ;
unsigned long epochTime;
// ***** SPLASH SCREEN *****
String splash = "AlexaIRx10";
// ***** X10 Channels Here *******************
String Chan1 = "Channel one";
String Chan2 = "Channel two";
String Chan3 = "Channel three";
String Chan4 = "Channel four";
String Chan5 = "Channel five";
String Chan6 = "Channel six";
String Chan7 = "Channel seven";
String Chan8 = "Channel eight";
String Chan9 = "Channel nine";
String Chan10 = "Channel ten";
String Chan11 = "Channel eleven";
String Chan12 = "Channel twelve";
String Chan13 = "Channel thirteen";
String Chan14 = "Channel fourteen";
String Chan15 = "Channel fifteen";
String Chan16 = "Channel sixteen";
String Chan17 = "House Lights";
// ***** Media Components Here ***************
String Comp1 = "disk player";
String Comp2 = "big screen";
String Comp3 = "computer";



///////////////////////////////////////////////////////////
SSD1306Wire  display(0x3c, SDA, SCL);
Espalexa espalexa;
ESP8266WebServer server(80);
IRsend irsend(IRled);  // An IR LED is controlled by GPIO pin 12 (D6)

//*********************************************************************************
String ip2Str(IPAddress ip)
{
  String s="";
  for (int i = 0; i < 4; i++) 
    {
    s += i  ? "." + String(ip[i]) : String(ip[i]);
    }
  return s;
}

//*********************************************************************************
void OledText(char row, char col, String dtext) 
{  
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(col, row, dtext);
  display.display(); 
}
//*********************************************************************************
void OledNumber(char row, char col, int integer) 
{  
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(col, row, String (integer));
  display.display(); 
}
//************************************************************************************************
// Delay (loops) Milliseconds
void delayms(int loops) 
{
    time_now = millis();
    
    while(millis() < time_now + loops)
      {
      espalexa.loop(); 
      delay(1);   
      }
   
}
//************************************************************************************************
void dummy(uint8_t brightness)
{
  
}
//******************************************************************************** 
void updateTime () 
{
    date = "";  // clear the variables
    dateShort = "";  // clear the variables
    tyme = "";
    tymeShort = "";
    tymeSeconds = "";
    
    // update the NTP client and get the UNIX UTC timestamp 
//    timeClient.update();
    epochTime =  timeClient.getEpochTime();

    // convert received time stamp to time_t object
    time_t local, utc;
    utc = epochTime;

    // Then convert the UTC UNIX timestamp to local time
    TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -300};  //UTC - 5 hours - change this as needed
    TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -360};   //UTC - 6 hours - change this as needed
    Timezone usEastern(usEDT, usEST);
    local = usEastern.toLocal(utc);

    currentDay = weekday(local)-1;
    currentMonth = monthNum[month(local)-1];
    // now format the Time variables into strings with proper names for month, day etc
    dateShort += monthNum[month(local)-1];
    dateShort += "/";
    dateShort += day(local);

        // format the time to 12-hour format with AM/PM and seconds
    tyme += hourFormat12(local);
    tyme += ":";
    if(minute(local) < 10)  // add a zero if minute is under 10
      tyme += "0";
    tyme += minute(local);
    tyme += ":";  
    if(second(local) < 10)  // add a zero if minute is under 10
      tyme += "0";
    tyme += second(local);
    tymeSeconds = second(local);           // convert secs to temp string
    tymeMinutes = minute(local);           // convert mins to temp string
    tymeHours = hourFormat12(local);       // convert hrs to temp string
    currentSeconds = tymeSeconds.toInt();  // convert secs to Int
    currentMinutes = tymeMinutes.toInt();  // convert mins to Int
    currentHours = tymeHours.toInt();      // convert hrs to Int
    tyme += " ";
    tyme += ampm[isPM(local)];
}
//******************************************************************************** 
void updateNet () 
{
    timeClient.update();
}
//******************************************************************************** 
void updateMacroList () 
{
  
   //*****************************   
   // Coffee Pot Macro   
   if(tyme == "5:05:05 AM") 
      {
      X10Chan15(255);  
      }
   if(tyme == "9:09:09 AM") 
      {
      X10Chan15(0);  
      }      
   //*****************************   
   // Garage Light Macro
   if(tyme == "6:07:07 PM") 
      {       
      X10Chan14(255);    
      }     
   if(tyme == "10:10:10 PM") 
      {
      X10Chan14(0);      
      }      
   //*****************************   
   //  Morning Macro
   if(tyme == "8:08:08 AM")
      {
       X10Chan2(0);
       X10Chan3(0);
       X10Chan4(0); 
       X10Chan5(0);
       X10Chan10(0); 
       X10Chan14(0);         
      }   
   //*****************************   
   //  Christmas Macro
   if((currentMonth == "12") && (tyme == "4:04:04 PM"))
      {
      X10Chan3(255);
      }
   if((currentMonth == "12") && (tyme == "5:05:05 AM"))
      {
      X10Chan3(255);     
      }
   if((currentMonth == "12") && (tyme == "10:15:00 PM"))
      {
      X10Chan3(0);   
      }
   //*****************************   
   //  Macro 
   
   return;             
}
//*********************************************************************************
void displayFlag() 
{
   display.clear();
   OledText(row2, 0, "TV= "); 
   OledNumber(row2, 40, tvPowFlag);  
   OledText(row2, 60, "DV= "); 
   OledNumber(row2, 100, dvdPowFlag);         
   OledText(row3, 0, "SC= "); 
   OledNumber(row3, 40, smartcastFlag);                      
   OledText(row3, 60, "RV= ");      
   OledNumber(row3, 100, rcvPowFlag); 
   delayms(1000);
 
}
//*********************************************************************************
void displaydata(char *mess) 
{
                    //123456789012345
   display.clear();                 
   OledText(row2, 0, splash);        
   OledText(row3, 0, ipString); 
   OledText(row4, 0, mess);     
}
//******************************************************************************** 
void displayTime ()   // Gets called every second
{
    // print the date and time as a header
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, row1, dateShort);  
    display.drawString(64, row1, tyme);       
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, row2, splash);
    display.drawString(0, row3, ipString);
    display.drawString(0, row4, x10String);
    display.display();  
}
//*********************************************************************************
void X10Send(char row, String hcode, int chan, char OnOff)
{
  String OnOffState;
  int str_len = hcode.length() + 1;
  char hc[str_len];
  char state;
  x10String = "";
  
  if(OnOff == OFF)
     {
     OnOffState = stringOff;
     state = OFF; 
     }
  if(OnOff == ON)
     {
     OnOffState = stringOn; 
     state = ON;
     } 
  hcode.toCharArray(hc, str_len);      

  x10String += hcode;
  x10String += "  ";
  x10String += String (chan);
  x10String += "  ";
  x10String += OnOffState;

  X10.X10Trans(hc[0], chan, state);
  return;
}
//*************************************************************************************
void X10Chan1(uint8_t brightness) 
{    
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 1, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 1, OFF);          // House codes A - P,  Channels 0 - 15
        }
}
//*************************************************************************************
void X10Chan2(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 2, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 2, OFF);         // House codes A - P,  Channels 0 - 15
        }        
}
//*************************************************************************************
void X10Chan3(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 3, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 3, OFF);         // House codes A - P,  Channels 0 - 15
        }   
}
//*************************************************************************************
void X10Chan4(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 4, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 4, OFF);         // House codes A - P,  Channels 0 - 15
        }     
}
//*************************************************************************************
void X10Chan5(uint8_t brightness) 
{    
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 5, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 5, OFF);          // House codes A - P,  Channels 0 - 15
        }
}
//*************************************************************************************
void X10Chan6(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 6, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 6, OFF);         // House codes A - P,  Channels 0 - 15
        }        
}
//*************************************************************************************
void X10Chan7(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 7, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 7, OFF);         // House codes A - P,  Channels 0 - 15
        }   
}
//*************************************************************************************
void X10Chan8(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 8, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 8, OFF);         // House codes A - P,  Channels 0 - 15
        }     
}
//*************************************************************************************
void X10Chan9(uint8_t brightness) 
{    
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 9, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 9, OFF);          // House codes A - P,  Channels 0 - 15
        }
}
//*************************************************************************************
void X10Chan10(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 10, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 10, OFF);         // House codes A - P,  Channels 0 - 15
        }        
}
//*************************************************************************************
void X10Chan11(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 11, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 11, OFF);         // House codes A - P,  Channels 0 - 15
        }   
}
//*************************************************************************************
void X10Chan12(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 12, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 12, OFF);         // House codes A - P,  Channels 0 - 15
        }     
}
//*************************************************************************************
void X10Chan13(uint8_t brightness) 
{    
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 13, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 13, OFF);          // House codes A - P,  Channels 0 - 15
        }
}
//*************************************************************************************
void X10Chan14(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 14, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 14, OFF);         // House codes A - P,  Channels 0 - 15
        }        
}
//*************************************************************************************
void X10Chan15(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 15, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 15, OFF);         // House codes A - P,  Channels 0 - 15
        }   
}
//*************************************************************************************
void X10Chan16(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 16, ON);          // House codes A - P,  Channels 0 - 15
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 16, OFF);         // House codes A - P,  Channels 0 - 15
        }     
}
//*************************************************************************************
void X10Chan17(uint8_t brightness) 
{
      if (brightness == 255) 
        {
        value = LOW;
        X10Send(row4, housecode, 4, ON);          // House codes A - P,  Channels 0 - 15
        delayms(2000);
        X10Send(row4, housecode, 2, ON);      
        }
      else  
        {
        value = HIGH;
        X10Send(row4, housecode, 2, OFF);
        delayms(2000);
        X10Send(row4, housecode, 4, OFF);
        delayms(2000); 
        X10Send(row4, housecode, 10, OFF);
        delayms(2000);
        X10Send(row4, housecode, 14, OFF);
        }
      
}
//************************************************************************************************
// root web page
void handle_root() 
{
  
  String message =
  "<!DOCTYPE html>\n"
  "<html>\n"
  "<head>\n"
  "<title>AlexaHAL</title>\n"
  "<script>\n"
//*******************************************    
  "function pressButton(btn,value){\n"
  " var xmlhttp = new XMLHttpRequest();\n"
  " xmlhttp.onreadystatechange=function() {\n"
  "  if (xmlhttp.readyState==4 && xmlhttp.status==200) {\n"
  "   var response = xmlhttp.responseText;\n"
  "  }\n"
  " }\n"
  " xmlhttp.open('GET','/button?task=' + btn + '&value=' + value);\n"
  " xmlhttp.send();\n"
  " return false;\n"
  "}\n"
//*******************************************   
  "</script>\n"
  "</head>\n"
  "<style type='text/css'>\n"
//*******************************************    
  "body {\n"
  "  padding-left:10px;\n"        // was 20
  "  padding-right:10px;\n"       // was 20
  "  margin: 0;\n"
  "  text-decoration: none;\n"
  "  text-align:center;\n"
  "  font-family: Geneva,'Lucida Sans','Lucida Grande',sans-serif;\n"
  "  font-size: 25px;\n"          // button font size was 30
  "  line-height: 30px;\n"
  "  color: #000000;\n"           // this is Font Color
  "  background-color: #fff;\n"   // screen color
  "}\n"
//*******************************************    
  ".span4 {\n"
  " width:41.5%;\n"
  "}\n"
//*******************************************      
  ".button {\n"
  "  border:2px solid #333;\n"
  "  padding-top:30px;\n"
  "  padding-bottom:30px;\n"
  "  margin:2px;\n"
  "  color: black;\n"
  "  text-align:center;\n"
  "  max-width:380px;\n"
  "  float:left;\n"
  "  border-radius: 48px;\n"
  "  background-color: #eee;\n"
  "  font-weight: bold;\n"
  "  font-family: courier;\n"
  "  font-size: 250%;\n"   
  "}\n"
//*******************************************    
  ".number_btnon {\n"
  " background-color:#1f4;\n"
  "}\n"
//*******************************************    
  ".number_btnoff {\n"
  " background-color:#f33;\n"
  "}\n"
//*******************************************    
  "h1 {\n"
  "color: blue;\n"
  "font-family: verdana;\n"
  "font-size: 300%;\n"
  "}\n"
//*******************************************    
  "</style>\n"
  "<body>\n"
  "<h1>AlexaHAL</a></h1>" 
  "<p><a onclick='pressButton(\"num1\");'><div class='button number_btnon span4'>Chan 1</div></a>\n"
  "<a onclick='pressButton(\"num1off\");'><div class='button number_btnoff span4'>Chan 1</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num2\");'><div class='button number_btnon span4'>Chan 2</div></a>\n"
  "<a onclick='pressButton(\"num2off\");'><div class='button number_btnoff span4'>Chan 2</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num3\");'><div class='button number_btnon span4'>Chan 3</div></a>\n"
  "<a onclick='pressButton(\"num3off\");'><div class='button number_btnoff span4'>Chan 3</div></a><br></p>\n"    
  "<p><a onclick='pressButton(\"num4\");'><div class='button number_btnon span4'>Chan 4</div></a>\n"
  "<a onclick='pressButton(\"num4off\");'><div class='button number_btnoff span4'>Chan 4</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num5\");'><div class='button number_btnon span4'>Chan 5</div></a>\n"
  "<a onclick='pressButton(\"num5off\");'><div class='button number_btnoff span4'>Chan 5</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num6\");'><div class='button number_btnon span4'>Chan 6</div></a>\n"
  "<a onclick='pressButton(\"num6off\");'><div class='button number_btnoff span4'>Chan 6</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num7\");'><div class='button number_btnon span4'>Chan 7</div></a>\n"
  "<a onclick='pressButton(\"num7off\");'><div class='button number_btnoff span4'>Chan 7</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num8\");'><div class='button number_btnon span4'>Chan 8</div></a>\n"
  "<a onclick='pressButton(\"num8off\");'><div class='button number_btnoff span4'>Chan 8</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num9\");'><div class='button number_btnon span4'>Chan 9</div></a>\n"
  "<a onclick='pressButton(\"num9off\");'><div class='button number_btnoff span4'>Chan 9</div></a><br></p>\n"    
  "<p><a onclick='pressButton(\"num10\");'><div class='button number_btnon span4'>Chan 10</div></a>\n"
  "<a onclick='pressButton(\"num10off\");'><div class='button number_btnoff span4'>Chan 10</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num11\");'><div class='button number_btnon span4'>Chan 11</div></a>\n"
  "<a onclick='pressButton(\"num11off\");'><div class='button number_btnoff span4'>Chan 11</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num12\");'><div class='button number_btnon span4'>Chan 12</div></a>\n"
  "<a onclick='pressButton(\"num12off\");'><div class='button number_btnoff span4'>Chan 12</div></a><br></p>\n" 
  "<p><a onclick='pressButton(\"num13\");'><div class='button number_btnon span4'>Chan 13</div></a>\n"
  "<a onclick='pressButton(\"num13off\");'><div class='button number_btnoff span4'>Chan 13</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num14\");'><div class='button number_btnon span4'>Chan 14</div></a>\n"
  "<a onclick='pressButton(\"num14off\");'><div class='button number_btnoff span4'>Chan 14</div></a><br></p>\n"
  "<p><a onclick='pressButton(\"num15\");'><div class='button number_btnon span4'>Chan 15</div></a>\n" 
  "<a onclick='pressButton(\"num15off\");'><div class='button number_btnoff span4'>Chan 15</div></a><br></p>\n"    
  "<p><a onclick='pressButton(\"num16\");'><div class='button number_btnon span4'>Chan 16</div></a>\n"
  "<a onclick='pressButton(\"num16off\");'><div class='button number_btnoff span4'>Chan 16</div></a></p>\n"
  "</div>\n"
  "</body>\n"
  "</html>\n";
  
  server.send(200, "text/html", message);
  
}
//************************************************************************************************
// button web page
void handle_button() 
{  
  String messageLogic = "";
  String task = "";
  String value = "";
  task = "";
  value = "";
  for (i = 0; i < server.args(); i++ ) 
    {
    if(server.argName(i) == "task")
      {
      task = server.arg(i);
      }
    if(server.argName(i) == "value")
      {
      value = server.arg(i);
      }
    }
 
  //***** Call WebPage Button Functions *****************************************   
  if(task == "num1")
    {
    chan = 1;  
    chanFlag[chan] = 1;
    X10Chan1(255);   
    }
  else if(task == "num1off")
    {
    chan = 1; 
    chanFlag[chan] = 0;
    X10Chan1(0);   
    }
  //*********************************************      
  else if(task == "num2")
    {
    chan = 2;  
    chanFlag[chan] = 1;
    X10Chan2(255);    
    }
  else if(task == "num2off") 
    {
    chan = 2;
    chanFlag[chan] = 0;
    X10Chan2(0);    
    }
  //*********************************************            
  else if(task == "num3")
    {
    chan = 3;
    chanFlag[chan] = 1;
    X10Chan3(255);    
    }
  else if(task == "num3off") 
    {
    chan = 3;
    chanFlag[chan] = 0;
    X10Chan3(0);    
    }
  //*********************************************      
  else if(task == "num4")
    {
    chanFlag[4] = 1;
    X10Chan4(255);   
    }
  else if(task == "num4off") 
    {
    chanFlag[4] = 0;
    X10Chan4(0);    
    }  
  //*********************************************      
  else if(task == "num5") 
    {
    chanFlag[5] = 1;
    X10Chan5(255);    
    }
  else if(task == "num5off") 
    {
    chanFlag[5] = 0;
    X10Chan5(0);    
    }  
  //*********************************************      
  else if(task == "num6")
    {
    chanFlag[6] = 1;
    X10Chan6(255);    
    }
  else if(task == "num6off") 
    {
    chanFlag[6] = 0;
    X10Chan6(0);    
    }  
  //*********************************************      
  else if(task == "num7") 
    {
    chanFlag[7] = 1;
    X10Chan7(255);   
    }
  else if(task == "num7off")
    {
    chanFlag[7] = 0;
    X10Chan7(0);    
    }  
  //*********************************************      
  else if(task == "num8")
    {
    chanFlag[8] = 1;
    X10Chan8(255);    
    }
  else if(task == "num8off") 
    {
    chanFlag[8] = 0;
    X10Chan8(0);    
    }  
  //*********************************************      
  else if(task == "num9") 
    {
    chanFlag[9] = 1;
    X10Chan9(255);    
    }
  else if(task == "num9off") 
    {
    chanFlag[9] = 0;
    X10Chan9(0);    
    }  
  //*********************************************      
  else if(task == "num10")
    {
    chanFlag[10] = 1;
    X10Chan10(255);   
    }
  else if(task == "num10off")
    {
    chanFlag[10] = 0;
    X10Chan10(0);    
    }  
  //*********************************************      
  else if(task == "num11") 
    {
    chanFlag[11] = 1;
    X10Chan11(255);    
    }
  else if(task == "num11off") 
    {
    chanFlag[11] = 0;
    X10Chan11(0);    
    }   
  //*********************************************      
  else if(task == "num12") 
    {
    chanFlag[12] = 1;
    X10Chan12(255);    
    }
  else if(task == "num12off") 
    {
    chanFlag[12] = 0;
    X10Chan12(0);    
    }   
  //*********************************************      
  else if(task == "num13") 
    {
    chanFlag[13] = 1;
    X10Chan13(255);    
    }
  else if(task == "num13off") 
    {
    chanFlag[13] = 0;
    X10Chan13(0);    
    }   
  //*********************************************      
  else if(task == "num14") 
    {
    chanFlag[14] = 1;
    X10Chan14(255);    
    }
  else if(task == "num14off") 
    {
    chanFlag[14] = 0;
    X10Chan14(0);    
    }   
  //*********************************************      
  else if(task == "num15") 
    {
    chanFlag[15] = 1;
    X10Chan15(255);   
    }
  else if(task == "num15off") 
    {
    chanFlag[15] = 0;
    X10Chan15(0);    
    }   
  //*********************************************      
  else if(task == "num16")
    {
    chanFlag[16] = 1;
    X10Chan16(255);    
    } 
  else if(task == "num16off")
    {
    chanFlag[16] = 0;
    X10Chan16(0);    
    }     
  else
    {
    messageLogic += " {unknown:" + task + "}";
    }

  server.send(200, "text/plain", messageLogic);
  
}
//************************************************************************************************
void computerMacro(uint8_t brightness) 
{
     displaydata("Computer Macro ");
if (brightness == 255)
{  
     if(tvPowFlag == 0) // If TV is OFF, Turn ON TV
       {
       if(smartcastFlag == 0) // If TV is Not on Smartcast, set TV to Smartcast and Set to HDMI 1
         {
         irsend.sendRaw(rawDataVizPower, sizeof(rawDataVizPower)/sizeof(uint16_t), 38);
         displaydata("Viz Power      ");
                    //123456789012345    
         // Turn RCV to SAT
         irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
         displaydata("Rcv SAT        ");
                    //123456789012345
         rcvPowFlag = 1;
         tvPowFlag = 1;                 
         }
       if(smartcastFlag == 1)
         {
         irsend.sendRaw(rawDataVizPower, sizeof(rawDataVizPower)/sizeof(uint16_t), 38);
         displaydata("Viz Power      ");
                    //123456789012345  
         delayms(12000);                                                         
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345 
         delayms(1000); 
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345  
         delayms(1000);
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345 
         delayms(1000);
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345 
         delayms(1000);       
         // Turn RCV to SAT
         irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
         displaydata("Rcv SAT        ");
                    //123456789012345            
         rcvPowFlag = 1;
         tvPowFlag = 1; 
         smartcastFlag = 0;                    
         }
       }
     if(tvPowFlag == 1)
       {
       if(smartcastFlag == 0) // If TV is Not on Smartcast, and Set to HDMI 1
         {
         // Turn RCV to SAT
         irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
         displaydata("Rcv SAT        ");
                    //123456789012345         
         rcvPowFlag = 1;                            
         }
       if(smartcastFlag == 1)
         {
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345  
         delayms(1000);
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345 
         delayms(1000);
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345 
         delayms(1000);
         // Turn RCV to SAT
         irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
         displaydata("Rcv SAT        ");
                    //123456789012345            
         rcvPowFlag = 1;
         smartcastFlag = 0;   
         }
       }
} 
   
if(brightness <= 1)
{
         
}
                 
}
//************************************************************************************************
void dvdMacro(uint8_t brightness)   // Disk Player
{

   displaydata("DVD Macro      ");     
if(brightness == 255)
{
   if(tvPowFlag == 0)
     {
     if(smartcastFlag == 0) // If TV is on Smartcast
       {
       // Turn ON TV
       irsend.sendRaw(rawDataVizPower, sizeof(rawDataVizPower)/sizeof(uint16_t), 38);
       displaydata("Viz Power      ");
                  //123456789012345    
       tvPowFlag = 1;
       delayms(12000);  
       if(rcvPowFlag == 0) // If Receiver is OFF
         {        
                // Turn On DVD
         irsend.sendRaw(rawDataDVDPower, sizeof(rawDataDVDPower)/sizeof(uint16_t), 38);
         displaydata("DVD Power      ");
                  //123456789012345   
         dvdPowFlag = 1;  
         delayms(2000); 
             // Set RCV to DVD
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38); 
         rcvPowFlag = 1;   
         delayms(2000); 
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38);            
         return; 
         }
       }
     if(smartcastFlag == 1) // If TV IS on Smartcast
       { 
         // Turn TV ON
         irsend.sendRaw(rawDataVizPower, sizeof(rawDataVizPower)/sizeof(uint16_t), 38);
         displaydata("Viz Power      ");
         tvPowFlag = 1; 
                    //123456789012345  
         delayms(12000);                                                          
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345   
         delayms(700); 
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345   
         delayms(700);  
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345 
         delayms(700);    
                         // Turn On DVD
         irsend.sendRaw(rawDataDVDPower, sizeof(rawDataDVDPower)/sizeof(uint16_t), 38);
         displaydata("DVD Power      ");
                  //123456789012345   
         dvdPowFlag = 1;   
         delayms(700); 
               // Set RCV to DVD
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38); 
         rcvPowFlag = 1;   
         delayms(2000);  
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38);                             
         return;                                        
       }
     }// End If tvPowFlag = 0
     if((tvPowFlag == 1) && (smartcastFlag == 0))
       {     
       if(dvdPowFlag == 0) 
         {
         // Turn On DVD
         irsend.sendRaw(rawDataDVDPower, sizeof(rawDataDVDPower)/sizeof(uint16_t), 38);
         displaydata("DVD Power      ");
                  //123456789012345   
         dvdPowFlag = 1; 
         delayms(1000);  
               // Set RCV to DVD
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38); 
         rcvPowFlag = 1;   
         delayms(2000);  
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38);            
         }        
       } 
     if((tvPowFlag == 1) && (smartcastFlag == 1))
       {                                                                 
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345  
         delayms(700);  
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345  
         delayms(700);  
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");
                    //123456789012345  
         delayms(700);  
         irsend.sendRaw(rawDataVizInput, sizeof(rawDataVizInput)/sizeof(uint16_t), 38);
         displaydata("TV Input       ");         
                    //123456789012345 
         delayms(700);             
         tvPowFlag = 1; 
         smartcastFlag = 0; 
               // Set RCV to DVD
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38);  
         rcvPowFlag = 1;   
         delayms(2000);  
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38);                        
       
       if(dvdPowFlag == 0) 
         {
         // Turn On DVD
         irsend.sendRaw(rawDataDVDPower, sizeof(rawDataDVDPower)/sizeof(uint16_t), 38);
         displaydata("DVD Power      ");
                  //123456789012345   
         dvdPowFlag = 1;  
         delayms(1000);  
               // Set RCV to DVD
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38); 
         rcvPowFlag = 1;   
         delayms(2000);  
         irsend.sendRaw(rawDataRcvDVD, sizeof(rawDataRcvDVD)/sizeof(uint16_t), 38);             
         }
   
       }         
        
}

if(brightness <= 1);
{
                 
}


}
//************************************************************************************************
void tvMacro(uint8_t brightness)    // Big Screen
{
     displaydata("TV Macro       "); 
                //123456789012345 
   // Turn the TV and Receiver ON Only if smartcast is true

if (brightness == 255)
{  

   if(tvPowFlag == 0)
     {
     if(smartcastFlag == 1) // If TV is on Smartcast
       {
       // Turn ON TV
       irsend.sendRaw(rawDataVizPower, sizeof(rawDataVizPower)/sizeof(uint16_t), 38);
       displaydata("Viz Power      ");
                  //123456789012345     
       tvPowFlag = 1;
       delayms(4000); 
       // Turn On SAT
       irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
       displaydata("Rcv SAT        ");
                  //123456789012345  
       rcvPowFlag = 1;  
       delayms(1000);           
       return; 
       }
     
     if(smartcastFlag == 0) // If TV is NOT on Smartcast. Delay for Bootup
       {
         // Set TV to SmartCast        
         irsend.sendRaw(rawDataVizVbut, sizeof(rawDataVizVbut)/sizeof(uint16_t),38);
         displaydata("V-Button       ");
                    //123456789012345          
         smartcastFlag = 1;
         tvPowFlag = 1;  
         delayms(12000);   
         // Turn On SAT
         irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
         displaydata("Rcv SAT        ");
                    //123456789012345 
         rcvPowFlag = 1;         
         delayms(1000);                                
         return;                                          
       }
     }// End If tvPowFlag = 0  

   if(tvPowFlag == 1) 
     { 
     if(smartcastFlag == 0) // If TV is NOT on Smartcast
       {
       // Set TV to SmartCast        
       irsend.sendRaw(rawDataVizVbut, sizeof(rawDataVizVbut)/sizeof(uint16_t),38);
       displaydata("V-Button       ");
                    //123456789012345
       smartcastFlag = 1; 
       delayms(1000);                       
       // Turn On SAT
       irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
       displaydata("Rcv SAT        ");
                  //123456789012345  
       rcvPowFlag = 1;    
       delayms(1000);        
       }
     if(smartcastFlag == 1) // If TV is on Smartcast
       {
       // Turn On SAT
       irsend.sendRaw(rawDataRcvSAT, sizeof(rawDataRcvSAT)/sizeof(uint16_t), 38);
       displaydata("Rcv SAT        ");
                  //123456789012345  
       rcvPowFlag = 1;   
       delayms(1000);    
       } 
     }
}// End If brightness == 255       

if(brightness <= 1)
{
                //123456789012345
     displaydata("All Off Macro  ");
  
     if(tvPowFlag == 1) // If TV is OFF, Turn ON TV
       {
       // Turn OFF TV
       irsend.sendRaw(rawDataVizPower, sizeof(rawDataVizPower)/sizeof(uint16_t), 38);
       displaydata("Viz Power      ");
                  //123456789012345    
       delayms(1000);  
       tvPowFlag = 0;       
       }
     if(dvdPowFlag == 1) // If TV is OFF, Turn ON TV
       {
       // Turn OFF DVD
       irsend.sendRaw(rawDataDVDPower, sizeof(rawDataDVDPower)/sizeof(uint16_t), 38);
       displaydata("DVD Power      ");
                  //123456789012345   
       delayms(1000); 
       dvdPowFlag = 0;        
       }       
     if(rcvPowFlag == 1)  // If RCV is ON, Turn it OFF
        {
        // Turn OFF Rcv  
        irsend.sendRaw(rawDataRcvPower, sizeof(rawDataRcvPower)/sizeof(uint16_t), 38);
        displaydata("Rcv Power      ");
                   //123456789012345 
        delayms(1000); 
        rcvPowFlag = 0;
        }  
}

}
//************************************************************************************************
void setup()
{
  pinMode(ZC, INPUT_PULLUP);
  irsend.begin(); 
  display.init();

                   //123456789012345
  OledText(row2, 0, splash); 
  delay(2000);
  
  // Connect to WiFi network
  WiFi.mode(WIFI_STA);   
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
    {
    delay(500);
    }
    
//***********************************************
// Convert IPAddr to String for Printing  
  ipString = ip2Str(WiFi.localIP());  
  OledText(row3, 0, ipString); 
  delay(2000); 

  server.on("/", handle_root);
  server.on("/button", handle_button);
  server.onNotFound([]()
    {
    if (!espalexa.handleAlexaApiCall(server.uri(),server.arg(0)))
       {
       server.send(404, "text/plain", "Not found");
       }
    }
  );
    
  // Define your devices here. 
    espalexa.addDevice(Comp1, dvdMacro, 255);      
    espalexa.addDevice(Comp2, tvMacro, 255); 
    espalexa.addDevice(Comp3, computerMacro, 255);
    //espalexa.addDevice("Chan1", X10Chan1, 255); 
    espalexa.addDevice(Chan2, X10Chan2, 255); 
    espalexa.addDevice(Chan3, X10Chan3, 255); 
    espalexa.addDevice(Chan4, X10Chan4, 255);     
    espalexa.addDevice(Chan5, X10Chan5, 255); 
    espalexa.addDevice(Chan6, X10Chan6, 255); 
    espalexa.addDevice(Chan7, X10Chan7, 255); 
    espalexa.addDevice(Chan8, X10Chan8, 255);   
    espalexa.addDevice(Chan9, X10Chan9, 255); 
    espalexa.addDevice(Chan10, X10Chan10, 255);    
    espalexa.addDevice(Chan11, X10Chan11, 255); 
    //espalexa.addDevice(Chan12, X10Chan12, 255);  
    //espalexa.addDevice(Chan13, X10Chan13, 255); 
    espalexa.addDevice(Chan14, X10Chan14, 255);  
    espalexa.addDevice(Chan15, X10Chan15, 255); 
    espalexa.addDevice(Chan16, X10Chan16, 255);     
    espalexa.addDevice(Chan17, X10Chan17, 255);       
    espalexa.begin(&server); 

  tvFlag = 0; 
  tvPowFlag = 0;
  dvdFlag = 0;
  dvdPowFlag = 0; 
  rcvPowFlag = 0; 
  satFlag = 0; 
  cdFlag = 0; 
  hdmiFlag = 0; 
  smartcastFlag = 0;
  displayFlag();
  latch_bit = 0;
  count = 0;
  countFast = 0;
  updateNet(); 
  updateTime(); 
  displayTime(); 
  for (i = 0; i < 20; i++ ) 
    {
    chanFlag[i] = 2;  
    }
}
//************************************************************************************************
void loop() 
{

   count++;
   if(count >= 30000)     // Update Internet Time ~ every 30 seconds
     {      
     updateNet(); 
     count = 0; 
     }
   countFast++;
   if(countFast >= 970)   // Update date/time display ~1 second
     {  
     updateTime(); 
     displayTime(); 
     updateMacroList();          
     countFast = 0;
     }
     
   
   espalexa.loop();
//   delayms(1);

}
//************************************************************************************************