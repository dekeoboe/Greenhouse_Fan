/*
  LiquidCrystal display Temp/Humidity sensor data in two lines with error trapping
 switch relay depending on output of DHT22 temp/humidity sensor to control ventilation fan
 This sketch is set with a range of between 55F and 85F with relay activated for fan outside those limits
 Uses a 16x2 LCD display (Hitachi HD44780 driver) connected to a DHT22 temp/humidity sensor.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 T
 The circuit:
 * LCD Pin 1 to GND
 * LCD pin 2 to +5V
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K potentiometer ends to +5V and ground with wiper to LCD VO pin (pin 3)
 * DHT22 pin 1 to VCC (5v)
 * DHT22 pin 2 to Arduino data pin 7
 * DHT22 pin 3 not connected
 * DHT22 pin 4 to Arduino GND
 * -- Connect 10k pull up resistor between DHT22 pin 2 and VCC (5v)
 * using Sainsmart quad relay, only using #1 relay for this sketch
 * --Relay INT1 to arduino digital pin 1 (pin 8)
 * --Relay Gnd to arduino GND
 * --Relay VCC to Arduino VCC (5v)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe
 
 */
//include Temp Humidity Sensor library code
#include <DHT22.h>
//include LCD library code
#include <LiquidCrystal.h>
//initialize lcd library with the pin numbers used in this instance
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define Relay1  8    //set pin 8 as the control pin for the relay                    
void setup(){
  //set up the LCD's number of columns
  lcd.begin(16,2);
  pinMode(Relay1, OUTPUT); //Set relay control 8 as output for relay control
  // DHT22 Data wire is plugged into pin 7 on the Arduino
  // Connect a 10K resistor between VCC and pin 7 (strong pullup)
#define DHT22_PIN 7

}

short int currentTemp; //ensure currentTemp matches type of DHT22 data for temp
// Setup a DHT22 instance
DHT22 myDHT22(DHT22_PIN);


void loop() { 
  DHT22_ERROR_t errorCode;

  // The sensor can only be read from every 1-2s, and requires a minimum
  // 2s warm-up after power-on. 
  delay(500);
  //Serial.print("Requesting data...");
  errorCode = myDHT22.readData();
  switch(errorCode)
  {
  case DHT_ERROR_NONE:
    if (myDHT22.getTemperatureF()>80&&myDHT22.getTemperatureF()<90)
    {
       lcd.setCursor(10,0);
       lcd.print("FanOff ");
 lcd.setCursor(0,0);
      lcd.print(myDHT22.getTemperatureF());
      lcd.print("F ");
      lcd.setCursor(0,1);
      lcd.print(myDHT22.getHumidity());
      lcd.print("%");
//delay(1000);      
      digitalWrite(Relay1, HIGH);  
    }//Turn on relay and display set condition on LCD
    else {
      digitalWrite(Relay1, LOW); //set relay "low" which flips relay off
      lcd.setCursor(10,0);
      lcd.print("FanOn  ");  //turn relay off and display condition
      lcd.setCursor(0,0);
      lcd.print(myDHT22.getTemperatureF());
      lcd.print("F ");
      lcd.setCursor(0,1);
      lcd.print(myDHT22.getHumidity());
      lcd.print("%");
  //    delay(1000);
    }
    break;
  case DHT_ERROR_CHECKSUM:
    lcd.setCursor(0,0);
    lcd.autoscroll();
    lcd.print("check sum error ");
    lcd.print(myDHT22.getTemperatureC());
    lcd.noAutoscroll();
    lcd.setCursor(0,1);
    lcd.print("C ");
    lcd.print(myDHT22.getHumidity());
    lcd.println("%");
    break;
  case DHT_BUS_HUNG:
    lcd.setCursor(0,0);
    lcd.autoscroll();
    lcd.print("BUS Hung ");
    lcd.noAutoscroll();
    break;
  case DHT_ERROR_NOT_PRESENT:
    lcd.setCursor(0,0);
    lcd.autoscroll();
    lcd.print("Not Present ");
    lcd.noAutoscroll();
    break;
  case DHT_ERROR_ACK_TOO_LONG:
    lcd.setCursor(0,0);
    lcd.autoscroll();
    Serial.println("ACK time out ");
    lcd.noAutoscroll();
    break;
  case DHT_ERROR_SYNC_TIMEOUT:
    lcd.setCursor(0,0);
    lcd.autoscroll();
    lcd.print("Sync Timeout ");
    lcd.noAutoscroll();
    break;
  case DHT_ERROR_DATA_TIMEOUT:
    lcd.setCursor(0,0);
    lcd.autoscroll();
    lcd.print("Data Timeout ");
    lcd.noAutoscroll();
    break;
  case DHT_ERROR_TOOQUICK:

    lcd.autoscroll();
    Serial.print("Polled too quickly ");
    lcd.noAutoscroll();
    break;
  }
}




