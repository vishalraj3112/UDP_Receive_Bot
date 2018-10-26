#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char* ssid = "Mascot";
const char* password = "12345678";
char data[200] ={};
int packetsize = 0; 
String receiveddata="";

IPAddress ip;

WiFiUDP Server;


#define m1 D0
#define m2 D1 
#define m3 D2 
#define m4 D3

void initOutputs() {
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
}


void moveForwards() {
  Serial.println("Forward");
  analogWrite(m1,1024);
  analogWrite(m2,0);
  analogWrite(m3,1024);
  analogWrite(m4,0);
}

void moveBackwards() {
  Serial.println("Backwards");
  analogWrite(m1,0);
  analogWrite(m2,1024);
  analogWrite(m3,0);
  analogWrite(m4,1024);
}

void turnRight() {
  Serial.println("Hard Right");
  analogWrite(m1,1024);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,1024);
}

void turnLeft() {
  Serial.println("Hard Left");
  analogWrite(m1,0);
  analogWrite(m2,1024);
  analogWrite(m3,1024);
  analogWrite(m4,0);
}


void softLeftF()
{
  Serial.println("Soft Left F");
  analogWrite(m1,0);
  analogWrite(m2,0);
  analogWrite(m3,1024);
  analogWrite(m4,0);
}

void softLeftB()
{
  Serial.println("Soft left B");
  analogWrite(m1,0);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,1024);
}

void softRightF()
{
  Serial.println("Soft Right F");
  analogWrite(m1,1024);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,0);
}
void softRightB()
{
  Serial.println("Soft Right B");
  analogWrite(m1,0);
  analogWrite(m2,1024);
  analogWrite(m3,0);
  analogWrite(m4,0);
}

void resetEngine() {
  Serial.println("reset e");
  analogWrite(m1,0);
  analogWrite(m2,0);
  analogWrite(m3,0);
  analogWrite(m4,0);
}


void setup() {
  Serial.begin(115200);
  initOutputs();

  connectWifi(); 
  
  //setup AP
//  WiFi.mode(WIFI_AP);
//  WiFi.softAP(ssid,pw,11);
//  WiFi.begin();
//  Serial.println("AP started at: ");
//  Serial.print(WiFi.softAPIP());
//  Server.begin(1111);
//  Serial.println("Server started...");
//  
}

void connectWifi() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WIFI network");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Server.begin(1111);
  ip = WiFi.localIP();
  Serial.println(ip);
}

void handleInput(String x) {
  if(x.equals("Forward")) {
    //logic for forward
    moveForwards();
    Serial.println("yes");
  }
  else if(x.equals("softLeftF")) {
     //logic for soft left forward
     softLeftF();
  }
  else if(x.equals("softRightF")) {
     //logic for soft right forward
     softRightF();
  }
  else if(x.equals("Backward")) {
     //logic for backward
     moveBackwards();
  }
  else if(x.equals("softLeftB")) {
     //logic for soft left backward
     softLeftB();
  }
  else if(x.equals("softRightB")) {
     //logic for soft right backward
     softRightB();
  }
  else if(x.equals("hardRight")) {
     //logic for hard right
     turnRight();
  }
  else if(x.equals("hardLeft")) {
     //logic for hard left
     turnLeft();
  }
  else if(x.equals("Reset")) {
     //logic for reset engine
     resetEngine();
     
  }
}

void loop() {
  
    char message = Server.parsePacket();
    packetsize = Server.available();
    if (message)
    {
      
     Server.read(data,packetsize);
     IPAddress remoteip=Server.remoteIP();
     delay(100);
     Serial.println(Server.beginPacket(remoteip,1111));
     Serial.println(remoteip);
     Serial.println(Server.remotePort());
     Serial.println(data);
     Server.write(data);
     Server.endPacket();
    }

    if(packetsize) {
      Serial.println(packetsize);
      for (int i=0;packetsize > i ;i++)
      {
        receiveddata+= (char)data[i];
      }
        
      Serial.print(receiveddata);
      handleInput(receiveddata);
      Serial.println();
      receiveddata="";
    }
//    delay(100);
  
//  if(WiFi.status()==!WL_CONNECTED)
//  {
//    Serial.print(".");
//    delay(100);
//  }
}

