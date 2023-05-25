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

String message = "";

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
  Serial.begin(115200);
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

   // Update tis for all state machines
  unsigned long cur_time = millis();   // Just one call to millis()
  fsm0.tis = cur_time - fsm0.tes;


  if (client.connected()) {
    
    while (client.available()) {
    Serial.println("available");
      //String message = client.readStringUntil('\n');
      //Serial.println("Received message: " + message);
  
      if(sensorValue && fsm0.state == 0){
        fsm0.new_state = 1;
        #ifdef __DEBUG__
          Serial.println("disparo");
        #endif  
      }
      else if(fsm0.state == 1 && message == "anda"){
        fsm0.new_state = 2;
        message = "";
      }
      else if(fsm0.state == 2 && fsm0.tis > 2000){
        fsm0.new_state = 3;
        #ifdef __DEBUG__
          Serial.println("Para motor");
        #endif
      }
      else if(fsm0.state == 3 && message == "parou"){
        fsm0.new_state = 0;
        message = "";
      }

      if(fsm0.state == 0){
        sensorValue = digitalRead(13);
        digitalWrite(base, LOW);
      }
      else if(fsm0.state == 1){
        client.println("disparo");
        message = client.readStringUntil('\n');
      }
      else if(fsm0.state == 2){
        digitalWrite(base, HIGH);
      }
      else if(fsm0.state == 3){
        digitalWrite(base, LOW);
        client.println("para");
        message = client.readStringUntil('\n');
      }
  
      // Update the states
      set_state(fsm0, fsm0.new_state); 
      
      client.println("Message received"); // Send a response back to the client
      delay(10);
    }
  }
}



