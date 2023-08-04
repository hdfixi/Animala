#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
//#include <Wire.h> 
#include <HTTPClient.h>
#include <UrlEncode.h>
// #include <RtcDS1302.h>
// #include <ThreeWire.h>
#include<NewPing.h>
#include <NTPClient.h>

#define trig 19
#define echo 34

#define MAX_DISTANCE 200

NewPing sonar(trig, echo, MAX_DISTANCE);
//lcd object

LiquidCrystal_I2C lcd(0x27,16,2);
//wifi

const char* ssid = "GalaxyA12";
const char* password = "iheb2024";

// +international_country_c
// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+21629263530";
String apiKey = "6924552";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}
//ThreeWire myWire(4,5,2); // IO, SCLK, CE
//RtcDS1302<ThreeWire> Rtc(myWire);

const long timeZoneOffset = 1; // set your time zone offset here (e.g. -5 for Eastern Standard Time)
const int irSensorPin = 34; // Pin for IR sensor
//const int servoPin = 22; // Pin for servo
//const int servoStartPos = 0; // Initial position of servo
//const int servoEndPos = 90; // Position of servo when IR sensor sends 1

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", timeZoneOffset * 3600);
//Servo myservo;

unsigned long irSensorTriggeredTime = 0;
bool messageSent = false;


int h,m,t,k=0;
int n=0;
int p=0;
int beep=13;
Servo ms;
int pos = 20; 
const int servoPin=4;
void setup() {
    //sonar();
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	ms.setPeriodHertz(50); 
  ms.attach(servoPin, 1000, 2000);




  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  //wifi init
  //lcd.clear();
  WiFi.begin(ssid, password);
  lcd.setCursor(0,0);
  lcd.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //lcd.setCursor(10,0);
    Serial.print(".");
    //lcd.clear();
  }
  lcd.clear();
  Serial.println("");
  lcd.setCursor(0,0);
  lcd.print("Connected! ");
  Serial.println(WiFi.localIP());
delay(1000);
lcd.clear();
  // Send Message to WhatsAPP
  //sendMessage("n7ebek ya rouhi ❤");

//RegistryInstalledBuilt-inUpdates
//ServoESP32 by Jaroslav Paral
//Generate RC servo signal on a selected pins with ESP32 device and Arduino framework.
//Installation
//1.0.3 released 3 years ago
//￼Add to Project
  //lcd initialisation

  
  //myservo.attach(servoPin);
  //myservo.write(servoStartPos);
  pinMode(irSensorPin, INPUT); // Initialize IR sensor as input
  pinMode(beep,OUTPUT);
 //myservo.attach(servoPin);
 
  timeClient.update();
  m=timeClient.getMinutes();
  h=timeClient.getHours();
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print(m);
  n=0;
}



void loop() {
  t=timeClient.getSeconds();
  irSensorTriggeredTime = millis();
  timeClient.update();
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(timeClient.getFormattedTime());
  // lcd.setCursor((timeClient.getHours()/10==1?6:5),0);
  // lcd.print(":");
  // lcd.setCursor((timeClient.getMinutes()/10==0?7:6),0);
  // lcd.print(timeClient.getMinutes());

  int irSensorVal = digitalRead(irSensorPin); // Read the value from IR sensor

  // Serial.print(timeClient.getFormattedTime());
  // Serial.print(" / ");
  // Serial.println(timeClient.getMinutes()-m);
  if(t==0){n=0;Serial.println("yooo");}
  if(n==0&&timeClient.getMinutes()-m>=1&&sonar.ping_cm()<=23){

	for (pos = 0; pos <= 200; pos += 3) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		ms.write(pos);    // tell servo to go to position in variable 'pos'
		delay(8);             // waits 15ms for the servo to reach the position
	}
	delay(300);
	for (pos = 200; pos >= 40; pos -= 3) { // goes from 180 degrees to 0 degrees
		ms.write(pos);    // tell servo to go to position in variable 'pos'
		delay(8);             // waits 15ms for the servo to reach the position
	}
  delay(1000);
 	 n=1;



  }
 
  else if (t>55||t<3) {
  
   if (sonar.ping_cm()>=20&&p==0) {
      sendMessage("Hi, I'm petsecure!");
      sendMessage("I'm nearly empty !!");
      sendMessage("please refill me when you get home.");
      Serial.println("message sent");
      //beep signalm
      analogWrite(beep,150);
      delay(300);
      analogWrite(beep,0);
      delay(300);
      analogWrite(beep,150);
      delay(300);
      analogWrite(beep,0);
      delay(300);
      analogWrite(beep,150);
      delay(300);
      analogWrite(beep,0);
      delay(300);
    p=1;
 
  
    }
    // else if(sonar.ping_cm()<18){p=0;Serial.println(sonar.ping_cm());Serial.println("fffffff");}
    // Serial.println(sonar.ping_cm());
    // Serial.println("hmmmmm");
  } 

  
  


}
