/*
 Leeroy: An arduino CI lamp that reads a Jenkins feed
 
 Code inspired from:
 
 http://arduino.cc/en/Tutorial/WebClientRepeating
 http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231812230

 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

EthernetClient client;

//For holding the title of the entry
const int MY_BUFFER_SIZE = 100;
char buffer [MY_BUFFER_SIZE+1]; 

//variables for polling
unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 30000;  // delay between updates, in milliseconds 1 minute

//status of light
String light = "";

void setup() {
  pinMode(3, OUTPUT); 
  pinMode(5, OUTPUT);  
  
  // start serial port:
  Serial.begin(9600);
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using just the MAC adress, getting IP from DHCP
  Ethernet.begin(mac);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
   // for debugging
  Serial.print("Polling every ");
  Serial.print(postingInterval);
  Serial.println(" miliseconds");
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    serialEvent();
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  //reset the entry flag when we make a new conncetoin
  // if there's a successful connection:
  if (client.connect("dl.dropbox.com", 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    //TODO: configure somewhere else?
    client.println("GET /u/667447/test.rss HTTP/1.1");
    client.println("Host: dl.dropbox.com");
    client.println("User-Agent: Leeroy Arduino CI lamp");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}

void serialEvent() {
 
  if(client.find("<entry>")) {
    if(client.find("<title>")) {
      if(client.readBytesUntil('<', buffer, MY_BUFFER_SIZE)) {
        if(String(buffer).indexOf("(stable)") != -1) {
          if(light != "GREEN") {
            green();
          }
        } else {
          if(light != "RED") {
            red();
          }
        } 
        
        client.stop();
        client.flush();  
      }
    }
  }
}


void red() {
  digitalWrite(3, LOW); 
  digitalWrite(5, HIGH); 
  light = "RED";
  Serial.println("red");
}

void green() {
  digitalWrite(5, LOW);  
  digitalWrite(3, HIGH);
  light = "GREEN";
  Serial.println("green");
}


