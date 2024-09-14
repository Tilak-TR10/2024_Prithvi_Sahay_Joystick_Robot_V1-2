#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "Prithvi Sahay";
const char* password = "12345678";

//CHANGE3
#define ENA   D1  //D1
#define IN_1  D0  //D0
#define IN_2  D2  //D2
  
#define IN_3  D3  //D3  
#define IN_4  D4  //D4  
#define ENB   D8  //D8  

#define Water D5    // Relay motor Connection

String command;     //String to store app command state.

int speedCar = 150; // 0 to 255
int speed_low = 60;

int SoilSensor = A0; //Soil Sensor Connection 

Servo myservoSoil, myservoSeed;   // create servo object to control a servo 
ESP8266WebServer server(80);

void setup() {
 
  Serial.begin(115200);
  myservoSoil.attach(D6);
  myservoSeed.attach(D7);
  
  myservoSeed.write(160); //On Startups Default Location If not OK Then CHANGE1 160 to 0
  myservoSoil.write(0); //On Startups Default Location If not OK Then CHANGE2 0 to 160
 
 pinMode(ENA, OUTPUT); 
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT);
 pinMode(ENB, OUTPUT); 
 pinMode(Water, OUTPUT);  
 pinMode(SoilSensor, INPUT);
  
// Connecting WiFi
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void loop() {
    server.handleClient();
      command = server.arg("State");
      Serial.println(command);
           if (command == "F") goForword();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "Water") goWater();
      else if (command == "Soil") goSoil();
      else if (command == "Seed") goSeed();
      else if (command == "0") speedCar = 100;
      else if (command == "1") speedCar = 120;
      else if (command == "2") speedCar = 140;
      else if (command == "3") speedCar = 160;
      else if (command == "4") speedCar = 180;
      else if (command == "5") speedCar = 200;
      else if (command == "6") speedCar = 215;
      else if (command == "7") speedCar = 230;
      else if (command == "8") speedCar = 240;
      else if (command == "9") speedCar = 255;
      else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

  void goForword(){ 
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
   }

  void goBack(){ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
   }

  void goRight(){ 
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
    }

  void goLeft(){
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
    }

  void stopRobot(){  
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
      digitalWrite(Water, LOW);
      myservoSoil.write(0);
      myservoSeed.write(160);
  }

  void goWater(){  //CHANGE4
   digitalWrite(Water, HIGH);
   delay(2500);
   digitalWrite(Water, LOW);
   delay(3500);
   command = "S";
  }

  void goSoil(){ //CHANGE5 
   myservoSoil.write(120);
   delay(3000);
   Serial.println(analogRead(SoilSensor));
   if(analogRead(SoilSensor) >= 600){ //Change +- 600 If needed
    goWater(); 
    delay(50);
   }
   else{
    myservoSoil.write(0);
   }
  } 
  
 void goSeed(){ 
  delay(1000); 
  myservoSeed.write(0); 
  delay(1000);
  myservoSeed.write(160);
  delay(3000);
  }  
