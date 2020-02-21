
#include <ArduinoJson.h>  // jsan library
#include <ESP8266WiFi.h>  //esp 8266 wifi library
#include <PubSubClient.h>   // pubclish library 
int Soil_pin = A0;     // analog pin soile moisture
int red_LED = D3; //    LED IF NEED OF WATER
int thresholdValue =800; 
//char d11[32];
char s22[32];

//===================pass and ssid========================================================================================
const char* ssid = "satyam";
const char* password = "satyam1020";
const char* mqtt_server = "159.65.146.25";
char val[200];
#define mqtt_port 1883

//======================== wifi client===========================================================================================
WiFiClient wifiClient;
PubSubClient client(wifiClient);

//====================wifi setup=================================================================================================

void setup_wifi() {
   Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      
      Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(D2,HIGH);
}
//===============================reconnect function=====================================================

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //Once connected, publish an announcement...
     // client.publish("/icircuit/presence/ESP32/", "hello world");
     //client.publish("outTopic", "Hello from ESP8266 testing tynor");
      // ... and resubscribe
      client.subscribe("outTopic");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    //Serial.print("channel:");
    //Serial.println(topic);
    //Serial.print("data:");  
   
    Serial.write(payload, length);
    Serial.println();


}

void setup()
{ 
   Serial.begin(115200); 
  Serial.setTimeout(500);// Set time out for 
 setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);
  reconnect();
   Serial.println();
  pinMode(Soil_pin, INPUT);   
  digitalWrite(red_LED, LOW); 
  Serial.begin(9600);
} 
 
void loop() {  
  int sensorValue = analogRead(Soil_pin); 
 // Serial.print(sensorValue);  
  if(sensorValue < thresholdValue){ 
   Serial.print("sensorValue");
   Serial.print("-- "); 
   Serial.print(sensorValue);
   sprintf(s22, " %02d",sensorValue);
   String a1="{'Id' :1,'type':1,'name':'soile moisture':";
   String a2= s22;
   String a3= "}";
   String e1=a1+a2+a3;
   Serial.println(e1);  
    client.publish("outTopic",String(e1).c_str());
   
   digitalWrite(red_LED,HIGH);
   delay(100);
    } 
    else
    {   
   Serial.print("sensorValue");
   Serial.print("-- "); 
   Serial.print(sensorValue);
   sprintf(s22, " %02d",sensorValue);
   String a1="{'Id' :1,'type':1,'name':'soile moisture':";
   String a2= s22;
   String a3= "}";
   String e1=a1+a2+a3;
   Serial.println(e1); 
     client.publish("outTopic",String(e1).c_str());
     
  // client.publish("outTopic",String(sensorValue).c_str());
   Serial.println("   - Time to water your plant");
   digitalWrite(red_LED,HIGH);
   delay(100); 
     
    } 
     delay(500);
    } 
 
