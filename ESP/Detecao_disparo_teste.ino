#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define __DEBUG__

const char* ssid = "ESP_SACA_AP";
const char* password = "SACA123saca";
const int port = 80;

int base = 16;   
int state = 0;    
int sensorValue; 


typedef struct {
  int state, new_state;

  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;

fsm_t fsm0;

String message;

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

    while (client.available()) {
      //String message = client.readStringUntil('\n');
      //Serial.println("Received message: " + message);

      // Update tis for all state machines
      unsigned long cur_time = millis();   // Just one call to millis()
      fsm0.tis = cur_time - fsm0.tes;

      // read sensor
      sensorValue = digitalRead(13);
  
      if(sensorValue && fsm0.state == 0){
        set_state(fsm0, 1); 

        Serial.println("disparo");
      }
      else if(fsm0.state == 1){
        set_state(fsm0, 2); 

        message = "";
      }
      else if(fsm0.state == 2 && fsm0.tis > 2000){
        set_state(fsm0, 0); 

        Serial.println("Para motor");
      }

      // update outputs
      if(fsm0.state == 0){
        //digitalWrite(base, LOW);

        Serial.println("actual state -> 0");
      }
      else if(fsm0.state == 1){
        Serial.println("actual state -> 1 (inicio)");

        client.println("disparo");
        message = client.readStringUntil('\n');
        Serial.println("Received message: " + message);

        Serial.println("actual state -> 1 (fim)");
      }
      else if(fsm0.state == 2){
        //digitalWrite(base, HIGH);

        Serial.println("actual state -> 2");
      }
      
      delay(10);
    }
  }
}



