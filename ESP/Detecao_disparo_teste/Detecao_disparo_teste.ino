#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define __DEBUG__

const char* ssid = "ESP_SACA_AP";
const char* password = "SACA123saca";
const int port = 80;

int base = 16;   
int state = 0;    
int sensorValue, newSensorValue = false, oldSensorValue = false; 


typedef struct {
  int state, new_state;

  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;

fsm_t fsm0;

unsigned long timeToRotate;

String message, subMessage;

// Set new state
void set_state(fsm_t& fsm, int new_state)
{
  // if the state chnanged tis is reset
  if (fsm.state != new_state) {  
    fsm.state = new_state;
    fsm.tes = millis();
    fsm.tis = 0;
  } 
}

WiFiServer server(port);
WiFiClient client;

double getDistance(String message);

void setup() {
  Serial.begin(9600);
  pinMode(base,OUTPUT); 
  
  // Initialize state machines
  set_state(fsm0, 0);

  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  if (!client.connected()) {
    client = server.available();
  }

  if (client.connected()) {

    if(client.available()) 
      Serial.println("available");

    message = "";

    while (1) {
      // Update tis for all state machines
      unsigned long cur_time = millis();   // Just one call to millis()
      fsm0.tis = cur_time - fsm0.tes;

      // read sensor
      newSensorValue = digitalRead(13); 
      sensorValue = newSensorValue && !oldSensorValue;

      if(fsm0.state == 0 && sensorValue){
        set_state(fsm0, 1); 

        Serial.println("disparo");
      }
      else if(fsm0.state == 1 && (message.length() <= 4)){
        set_state(fsm0, 0); 

        message = "";
      }
      else if(fsm0.state == 1 && (message.length() >= 5)){
        set_state(fsm0, 2); 

        double distance = getDistance(message);

        Serial.print("A distancia obtida é de: "); Serial.println(distance, 2);

        timeToRotate = 6000;
      
        message = "";
      }
      else if(fsm0.state == 2 && fsm0.tis > timeToRotate){
        set_state(fsm0, 0); 

        //Serial.println("Para motor");
      }

      // update outputs
      // update outputs
      if(fsm0.state == 0){
        //digitalWrite(base, LOW);

        //Serial.println("actual state -> 0");
      }
      else if(fsm0.state == 1){
        //Serial.println("actual state -> 1");

        client.println("disparo");
        message = client.readStringUntil('\n');
        Serial.println("Received message: " + message);
      }
      else if(fsm0.state == 2){
        //digitalWrite(base, HIGH);

        //Serial.println("actual state -> 2");
      }

      //update old values
      oldSensorValue = newSensorValue;
      
      delay(100);
    }
  }  
}

double getDistance(String message){
    double distance_1 = message.substring(5).toDouble();
    double distance_2 = message.substring(6).toDouble();
     
    if(distance_1 < distance_2)
      return distance_2;
    else
      return distance_1;
  }

