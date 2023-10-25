#include "DHT.h"
#include <ArduinoJson.h>


#define DHTPIN 9     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT22   // DHT 22

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <SD.h>
#include <SPI.h>

File SensorReadings;

int pinCS = 10; // Pin 10 on Arduino Uno
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}

const int MoisturePin = A0;
const int SunPin = A1;
const int RainPin = A2;



int motor_Down = 5;  // Window down pin.
int motor_Up = 4;    // Window up pin.
int Pump = 2;  //8 pum pin.
int Fan = 3; //5 fan
int Buzzer = 8; // buzzer
int Light = 7; // Light
int pest = 6; //pest

int max_valueMoist = 375;
int min_valueMoist = 185;
int max_valueRain = 550;
int min_valueRain = 240;
float max_valueSun = 735;
int min_valueSun = 0;
int min_sun = 100;

int treshHold = 40;
int treshHold2 = 40;

int on_time = 6;
int off_time = 18;

float sun_percent;
float moisture_percent;
float Rain_percent;

int Down_delay = 10000;
int Up_delay = 10000;
int Pump_delay = 10000;

float val_analogiqueMoisturePin;
float val_analogiqueSunPin;
float val_analogiqueRainPin;

int  rain = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Wire.begin();
  Serial.begin(115200);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year

pinMode(MoisturePin, INPUT);
  pinMode(SunPin, INPUT);
  pinMode(RainPin, INPUT);
  pinMode(motor_Down,OUTPUT);
   pinMode(motor_Up,OUTPUT);
   pinMode(Fan,OUTPUT);
   pinMode(Pump,OUTPUT);
    pinMode(Buzzer,OUTPUT);
    pinMode(Light,OUTPUT);
     pinMode(pinCS, OUTPUT);
 

  //  Serial.println("");
 //  Serial.print("Time");
 //  Serial.print(",");
//   Serial.print("Date");
 //  Serial.print(",");
 //   Serial.print("Tempreture(c)");
 //  Serial.print(",");
//   Serial.print("Humidity(%)");
//   Serial.print(",");
//   Serial.print("Wetness(%)");
 //  Serial.print(",");
 //  Serial.print("Sunshine(%)");
 //  Serial.print(",");
 //  Serial.print("RainFall(%)");
 //   Serial.println("");

  dht.begin();

   if (SD.begin())
  {
  //  Serial.println("SD card is ready to use.");
  } else
  {
   // Serial.println("SD card initialization failed");
    return;
  }

   SensorReadings = SD.open("test.csv", FILE_WRITE);
//  // if the file opened okay, write to it:
 if (SensorReadings) {
   SensorReadings.println("");
   SensorReadings.println("");
   SensorReadings.print("Time");
   SensorReadings.print(",");
   SensorReadings.print("Date");
   SensorReadings.print(",");
   SensorReadings.print("Tempreture(c)");
   SensorReadings.print(",");
   SensorReadings.print("Humidity(%)");
   SensorReadings.print(",");
   SensorReadings.print("Wetness(%)");
   SensorReadings.print(",");
   SensorReadings.print("Sunshine(%)");
   SensorReadings.print(",");
   SensorReadings.print("RainFall(%)");
   
  SensorReadings.close(); // close the file

 }
  else {
  //  Serial.println("error opening test.txt");
  }
}

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
//void displayTime(){
//  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
//  // retrieve data from DS3231
//  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
//  &year);
//
//  Serial.println("");
//  Serial.print(hour);
//  Serial.print(",");
//  Serial.print(minute);
//  Serial.print(",");
//  Serial.print(dayOfMonth, DEC);
//  Serial.print("/");
//  Serial.print(month, DEC);
//  Serial.print("/");
//  Serial.print(year, DEC);
//  Serial.print(",");
//
//}



void Fan_On()
{
    digitalWrite(Fan,HIGH);
}
 
void Fan_Off()
{
    digitalWrite(Fan,LOW);
}

void Pump_On()
{
    digitalWrite(Pump,HIGH);
}
 
void Pump_Off()
{
    digitalWrite(Pump,LOW);
}
void Light_On()
{
    digitalWrite(Light,HIGH);
}
 
void Light_Off()
{
    digitalWrite(Light,LOW);
}
void Up()
{
    digitalWrite(motor_Down,LOW);
    digitalWrite(motor_Up,HIGH);
 
}

void Down()
{
    digitalWrite(motor_Up,LOW);
    digitalWrite(motor_Down,HIGH);
 
}

void Stop()
{
    digitalWrite(motor_Up,LOW);
    digitalWrite(motor_Down,LOW);
 
}

void sendDataOverSerial(JsonDocument& jsonDocument) {
  String jsonString;
  serializeJson(jsonDocument, jsonString); // Convert JSON object to a string
  Serial.println(jsonString); // Send the JSON string over Serial
}

void loop(){

  //displayTime(); // display the real-time clock data on the Serial Monitor,
   
   byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);

//  Serial.println("");
//  Serial.print(hour);
 // Serial.print(".");
 // Serial.print(minute);
 // Serial.print(",");
//  Serial.print(dayOfMonth, DEC);
//  Serial.print("/");
 // Serial.print(month, DEC);
//  Serial.print("/");
 // Serial.print(year, DEC);
 // Serial.print(",");

 
  val_analogiqueMoisturePin=analogRead(MoisturePin); 
  moisture_percent = map(val_analogiqueMoisturePin, max_valueMoist, min_valueMoist, 0, 100);

  val_analogiqueSunPin=analogRead(SunPin); 

  val_analogiqueRainPin=analogRead(RainPin); 
  Rain_percent = map(val_analogiqueRainPin, max_valueRain, min_valueRain, 0, 100);

  delay(200);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
   // Serial.println(F("Failed to read from DHT sensor!"));
  //  return;
  }

  // Compute heat index in Fahrenheit (the default)
///  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
 /// float hic = dht.computeHeatIndex(t, h, false);

 //  Serial.print(t);
 //  Serial.print(",");
 //  Serial.print(h);
 //  Serial.print(",");
 //  Serial.print(moisture_percent);
 //  Serial.print(",");
 //  Serial.print(sun_percent);
//   Serial.print(",");
  // Serial.print(Rain_percent);
 //   Serial.println("");


//  Serial.print("hour : ");
//Serial.println(hour); 
// Serial.println("");

//Serial.print("minute : ");
// Serial.println(minute); 
// Serial.println("");

// Serial.print("dayOfMonth : ");
 //Serial.println(dayOfMonth); 
 //Serial.println("");

 //Serial.print("month : ");
// Serial.println(month); 
// Serial.println("");

//Serial.print("year : ");
 //Serial.println(year); 
 //Serial.println("");
 
  //   Serial.print("Tempreturen : ");
 //Serial.println(t); 
 //Serial.println("");

  // Serial.print("Humidity : ");
// Serial.println(h); 
// Serial.println("");
  
val_analogiqueMoisturePin=analogRead(MoisturePin); 
moisture_percent = map(val_analogiqueMoisturePin, max_valueMoist, min_valueMoist, 0, 100);
// Serial.print("moisture_percent : ");
 //Serial.println(moisture_percent); 
 //Serial.println("");
  delay(1000);

    val_analogiqueSunPin=analogRead(SunPin); 
  sun_percent = (val_analogiqueSunPin/max_valueSun)* 100;
  sun_percent = ceil(sun_percent);
 //Serial.print("sun_percent : ");
 //Serial.println(sun_percent); 
 //Serial.println("");
  delay(1000);

    val_analogiqueRainPin=analogRead(RainPin); 
  Rain_percent = map(val_analogiqueRainPin, max_valueRain, min_valueRain, 0, 100);
// Serial.print("Rain_percent : ");
// Serial.println(Rain_percent); 
// Serial.println("");
 // delay(1000);

//sun_percent = (val_analogiqueSunPin/max_sun)*100;
//moisture_percent = 100-((val_analogiqueMoisturePin/max_moi)*100);
//Rain_percent = 100-((val_analogiqueRainPin/max_rain)*100);

   //Serial.print(sun_percent);
  //Serial.print(",");
   //Serial.print(moisture_percent);
   //Serial.print(",");
   //Serial.print(Rain_percent);

    
  delay(1000); // every second
////Heat Control
////if (t >= max_tep)    {  
// if (t >= max_tep && val_analogiqueSunPin >= min_sun)    { 
  //    Fan_On();
//  }
//  else {
//    Fan_Off();
//  }


//Moisture Control

if (moisture_percent <= treshHold)    {  
      Pump_On();
      delay(Pump_delay);
       Pump_Off();
  }
  else {
    Pump_Off();
  }

  //Rain Control
if (Rain_percent >= treshHold2 && rain == 0 )    {  

  Up();
    delay(Up_delay);
    Stop();
   rain = 1;

  
  }
 if (Rain_percent <= treshHold2 && rain == 1 ) {
    Down();
    delay(Down_delay);
    Stop();
    rain = 0;
  }
    else {
    Stop();
  }
  digitalWrite(Buzzer,HIGH);
  //digitalWrite(Light,HIGH);
  delay(500);
  digitalWrite(Buzzer,LOW);
  //digitalWrite(Light,LOW);

  
///Time control for Light


//if (sun_percent >= min_sun)    { 
      Light_Off();
// }
 //else {
 // Light_Off();
// }



///Time control for Fan

if (minute == 0 || minute == 10 || minute == 20 || minute == 30 || minute == 40 || minute == 50 && hour == on_time && sun_percent >= min_sun)    { 
//if (minute == 0 || minute == 20 || minute == 40)    { 
      Fan_On();
 }
if (minute == 5 || minute == 15 || minute == 25 || minute == 35 || minute == 45 || minute == 55 && hour == off_time && sun_percent >= min_sun)    { 
//if (minute == 10 || minute == 30 || minute == 50)    { 
     Fan_Off();
}


if (hour == on_time && val_analogiqueSunPin >= min_sun)    { 
      Light_On();
    //  Fan_On()
} else {
if (hour == off_time && val_analogiqueSunPin >= min_sun)    { 
      Light_Off();
    //  Fan_Off()
 }
}

 SensorReadings = SD.open("test.txt", FILE_WRITE);
//  // if the file opened okay, write to it:
 if (SensorReadings) {

  SensorReadings.print(hour);
  SensorReadings.print(".");
  SensorReadings.print(minute);
  SensorReadings.print(",");
  SensorReadings.print(dayOfMonth, DEC);
  SensorReadings.print("/");
  SensorReadings.print(month, DEC);
  SensorReadings.print("/");
  SensorReadings.print(year, DEC);
  SensorReadings.print(",");
  SensorReadings.print(t);
  SensorReadings.print(",");
  SensorReadings.print(h);
  SensorReadings.print(",");
  SensorReadings.print(moisture_percent);
  SensorReadings.print(",");
  SensorReadings.print(sun_percent);
  SensorReadings.print(",");
  SensorReadings.print(Rain_percent);
  SensorReadings.println("");
    
  SensorReadings.close(); // close the file
//Serial.println("Done.");
 }
  else {
 //   Serial.println("error opening test.txt");
  }

  // Construct JSON data and send it to the ESP8266 module over Serial
  DynamicJsonDocument jsonDocument(100);
 //jsonDocument["hour"] = hour;
 // jsonDocument["minute"] = minute;
 // jsonDocument["dayOfMonth"] = dayOfMonth;
 // jsonDocument["month"] = month;
 // jsonDocument["year"] = year;
  jsonDocument["temperature"] = t;
  jsonDocument["humidity"] = h;
  jsonDocument["moisture"] = moisture_percent;
  jsonDocument["sunlight"] = sun_percent;
  jsonDocument["rainfall"] = Rain_percent;

  // Send sensor data to ESP32 module over Serial
  sendDataOverSerial(jsonDocument); // Send JSON data to ESP32 over Serial
  //delay(1000); // Serial.print(jsonDocument);

}
