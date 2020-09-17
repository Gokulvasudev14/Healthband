#define TempPin1 A0
#define TempPin2 A1
#define USE_ARDUINO_INTERRUPTS true    

#include <PulseSensorPlayground.h> 
#include <LiquidCrystal.h>   
#include <TinyGPS.h>
#include <SoftwareSerial.h>
TinyGPS gps;
SoftwareSerial SIM900(7, 8);
const int PulseWire = 0;       
const int LED13 = 13;          
int Threshold = 550; 
int TempValue;

PulseSensorPlayground pulseSensor;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  SIM900.begin(9600); 
  lcd.begin(20, 4);
    lcd.print("Welcome!");
  
  Serial.begin(9600);
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold); 
  
   if (pulseSensor.begin()) {
    Serial.println("Started!");
    
}
}
void loop()
{
  int myBPM = pulseSensor.getBeatsPerMinute(); 
  

  
if (pulseSensor.sawStartOfBeat()) {           
 Serial.print("BPM: ");                        
 Serial.println(myBPM);                        
 lcd.setCursor(0, 0);
 lcd.print("BPM:  ");
 lcd.setCursor(6, 0);
 lcd.print(myBPM);
 if(myBPM > 150){
  lcd.setCursor(0, 1);
 lcd.print("Needs Medical Attention!");
   }
   else if(myBPM < 50){
  lcd.setCursor(0, 1);
 lcd.print("Emergency!!!");
 bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
 for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }

  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    SIM900.print("AT+CMGF=1\r"); 
    delay(400);
    SIM900.println("AT + CMGS = \"+91xxxxxxxxxx\"");// recipient's mobile number with country code
    delay(300);
    SIM900.print("Latitude = ");
    SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SIM900.print(" Longitude = ");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    SIM900.println();
    

  }
 
  Serial.println(failed);
   }
   else{
    lcd.setCursor(0, 1);
 lcd.print("Normal");
    }
  TempValue = analogRead(TempPin2);
  float TempCel = ( TempValue*500)/1024;

  Serial.print("TEMPRATURE in Celsius = "); 
  Serial.print(TempCel);
  Serial.print("*C");
  lcd.setCursor(0, 2);
 lcd.print("TEMP: ");
 lcd.setCursor(7, 2);
 lcd.print(TempCel);
 if(TempCel < 35){
  lcd.setCursor(0, 3);
 lcd.print("Freezing");
   }
   else if(TempCel > 38){
  lcd.setCursor(0, 3);
 lcd.print("Fever!");
   }
   else{
    lcd.setCursor(0, 3);
 lcd.print("Normal");
    }
  delay(100);
}
}
 
