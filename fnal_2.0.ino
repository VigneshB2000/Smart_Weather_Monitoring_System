#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <SoftwareSerial.h>
#include<Wire.h>
#include<LiquidCrystal.h>
#include <LiquidCrystal_I2C.h> 
#include "DHT.h"       

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "VigneshB"
#define AIO_KEY         "aio_qoyg59LectCi6zT6FuT4H2hnLgbk"
#define DHTTYPE DHT11   
#define dht_dpin 0

LiquidCrystal_I2C lcd(0x3F,16,2);
DHT dht(dht_dpin, DHTTYPE);
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");

const char* ssid     = "E.D.I.T.H";     
const char* password = "vicky008"; 

byte tt[]={B00100,B01010,B01010,B01110,B01110,B11111,B11111,B01110};  
byte hh[]={B00100,B000100,B01110,B10111,B10111,B111111,B01110,B00000};
byte bb[]={B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};
byte cc[]={B01010,B01010,B11111,B11111,B11111,B01110,B00100,B00100};
byte cc1[]={B00000,B00000,B00001,B00011,B10110,B11100,B01000,B00000};

void setup()
{ 
  pinMode(A0, INPUT);
  lcd.begin();
  lcd.clear();
  
  Serial.begin(9600);
  lcd.createChar(0,tt);
  lcd.createChar(1,bb);
  lcd.createChar(2,cc);
  lcd.createChar(3,hh);
  lcd.createChar(4,cc1);
  delay(10);
  
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("VIGNESH B");
  lcd.setCursor(5,1);
  lcd.print("1816152");
  delay(3000);
  
  for(int p=0;p<25;p++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WEATHER  MONITOR");
  lcd.setCursor(5,1);
  lcd.write((byte)0);
  lcd.print(" ");
  lcd.write((byte)1);
  lcd.print(" ");
  lcd.write((byte)3);
  delay(3000);
  lcd.clear();
  
  dht.begin();
  Serial.begin(9600);
  delay(700);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting"); 
    lcd.setCursor(0,0);
    lcd.print("Connecting... ");
    lcd.write((byte)2);
  }
  connect();
}

void connect()
{
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) 
  {
    switch (ret) 
    {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }
    if(ret >= 0)
      mqtt.disconnect();
    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  
  Serial.println(F("Adafruit IO Connected!"));
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connected to");
  lcd.setCursor(0,1);
  lcd.print(" Adafruit  ");
  lcd.write((byte)4);
  lcd.write((byte)2);
  delay(2000);
}

void loop() 
{
     if(! mqtt.ping(3))
     {
     if(! mqtt.connected())
     connect();
  }
    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("R.H.");
    lcd.write((byte)3);
    lcd.print(" :");
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("TEMP");
    lcd.write((byte)0);
    lcd.print(" :");
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C");
    
    if (! Temperature.publish(t))
    {                     
      Serial.println(F("Failed"));
    } 
     if (! Humidity.publish(h)) 
     {                     
      Serial.println(F("Failed"));
    }
        else 
    {
      Serial.println(F("Sent!"));
      delay(10000);
    }
}
