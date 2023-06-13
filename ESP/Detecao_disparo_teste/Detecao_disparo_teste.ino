#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#define __DEBUG__

const char* ssid = "ESP_SACA_AP";
const char* password = "SACA123saca";
const int port = 80;

const int motorPin = 5;
const int PWM_value = 100;
int base = 5;   
int state = 0;    
int sensorValue, newSensorValue = false, oldSensorValue = false; 

float dist_papel = 50;

bool stopRotate = false;
double distance = 0;

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
void rotate(double distance);

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
      newSensorValue = digitalRead(12); 
      sensorValue = newSensorValue && !oldSensorValue;

      if(fsm0.state == 0 && sensorValue){
        set_state(fsm0, 1); 

        Serial.println("disparo");
      }
      else if(fsm0.state == 1 && (message.length() <= 3)){
        set_state(fsm0, 0); 

        message = "";
      }
      else if(fsm0.state == 1 && (message.length() >= 5)){
        set_state(fsm0, 2); 

        distance = (getDistance(message)/1000)*30;

        Serial.print("A distancia obtida é de: "); Serial.println(distance, 2);
      
        message = "";
      }
      else if(fsm0.state == 2 && stopRotate){
        set_state(fsm0, 0); 
        stopRotate = false;

        Serial.println("Para motor");
      }

      // update outputs
      // update outputs
      if(fsm0.state == 0){
        analogWrite(base, 0);

        Serial.println("actual state -> 0");
      }
      else if(fsm0.state == 1){
        Serial.println("actual state -> 1");
        client.println("disparo");
        message = client.readStringUntil('\n');
        Serial.println("Received message: " + message);
      }
      else if(fsm0.state == 2){
        rotate(distance);
        stopRotate = true;
        Serial.println("actual state -> 2");
      }

      //update old values
      oldSensorValue = newSensorValue;
      
      delay(20);
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

float Calc_dist_papel(float diam_menor, float diam_maior, float espessura){
  float raio = diam_menor/2;
  float total_dist=0, p=0;
  int count=0;
  do{
    p= 2*3.14*(raio);
    total_dist= total_dist+p;
    raio=raio+espessura;
    count =count+1;
   
  }while(raio<=(diam_maior/2));
  // Passar distância total de mm para 
  //Serial.printf("Perimetro %d: %.3f, Raio= %.5f, distancia atual: %.4f \n", count, p, raio, total_dist);
 // Serial.printf("Distância Total: %.2f \n",total_dist);
 return total_dist;
}

float Calc_novo_raio(float comprimento, float diam_min, float espessura){ // comprimento em METROS
  double p=0, raio=diam_min/2;
  //comprimento=comprimento*1000;
  //Serial.printf("Perimetro: %.2f, Comprimento: %.2f, Raio: %.6f \n",p,comprimento,raio );
  do{
    p= 2*3.14*raio;
    comprimento=comprimento-p;
    if(comprimento>=0){
      raio=raio+espessura;
    }
    //Serial.printf("Perimetro: %.2f, Comprimento: %.2f, Raio: %.6f \n",p,comprimento,raio );
  }while(comprimento>=0);
 // Serial.printf("Perimetro: %.2f, Comprimento: %.2f, Raio: %.6f \n",p,comprimento,raio );
 // Serial.printf("Tamanho Raio: %.4f \n", raio);
  return (raio);
}

float Calc_tempo_rota(float raio , float speed_nominal, float dist_percorrer ){ // Velocidade em RPM, Raio em cm, distancia em mm
  float W=0, V=0, T=0, freq=0;
  int numero_inteiros=0;
  //Calculo velocidade do papel
  freq=(speed_nominal/60); //frequência em Hertz
  // w=2*pi*f
  W=2*3.14*freq;
  V=W*raio;
  //calculo tempo
  T=dist_percorrer/V;
  Serial.printf("Frequência: %.2f, Velocidade Angular: %.2f, Velocidade: %.2f, Tempo: %.2f \n", freq,W,V,T);
  return T;
}


void rotate(double distance){
  dist_papel = Calc_dist_papel(0.025, 0.090, 0.000121);
  float raio = Calc_novo_raio(dist_papel, 0.025, 0.000121);
  float time = Calc_tempo_rota(raio, 150, distance);

  //rotate
  analogWrite(motorPin, PWM_value);
  delay(time*1000);
  analogWrite(motorPin, 0);

  //update roll distance
  dist_papel = dist_papel - distance;
}
