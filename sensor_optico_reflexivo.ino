
//SENSOR DE PRESENÇA

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>//Biblioteca do UDP.
#include <ArduinoJson.h>
#include <NTPClient.h>//Biblioteca do NTP.

WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
const int buttonPin = 14; 
const int ledStatus = 13;
int buttonState = 0;  // Guardar o valor retornado
const int statussensor = 0;
boolean controlhe  = false;
void setup() {
 
  Serial.begin(115200);//CONECÇÃO da Serial
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledStatus, INPUT);
  WiFi.begin("Projeti", "projeti2018");    //Conecção do WiFi
  Serial.print("Esperando por conecçao");
  while (WiFi.status() != WL_CONNECTED) {  //espera o wifi conectar 
    delay(500);
    Serial.print(".");
  }
     Serial.print("Conectado");
     digitalWrite(LED_BUILTIN, LOW);
   ntp.begin();//Inicia o NTP.
   ntp.forceUpdate();//Força o Update.
}

void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    StaticJsonBuffer<300> JSONbuffer;   //Declara um buffer Json estatico
    JsonObject& JSONencoder = JSONbuffer.createObject(); 
  
    JSONencoder["tipo"] = "presença";
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && controlhe == true) { // Se HIGH, superficie preta  
      digitalWrite(LED_BUILTIN, LOW); // Acender o led
      Serial.write("LOW"); // Escrever no console
      String hora = ntp.getFormattedTime();//Váriavel que armazenara o horario do NTP.
      JSONencoder["hora"] = hora;
      JSONencoder["valor"] = "TRUE";
      char JSONmessageBuffer[300];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      ///////////////////ENVIA OS DADOS PARA O SERVIDOR VIA POST/////////////////////////////
 
      HTTPClient http;    //Declare object of class HTTPClient
   
      http.begin("http://10.0.0.2:3000/alarme");      //Especifica o destino da requisição
      http.addHeader("Content-Type", "application/json");  //Especifica o content-type header
   
      int httpCode = http.POST(JSONmessageBuffer);//Envia a requisição
      String payload = http.getString();//Recupera a resposta payload
   
      Serial.println("codigo de retorno HTTP" + httpCode);//Imprime o codigo de retorno HTTP
      Serial.println("resposta da requisição payload" + payload);//Imprime a resposta da requisição payload
      if(payload == "true"){
        controlhe = true;
      }else if(payload == "false"){
        controlhe = false;
      }
      http.end();//Fecha conecção
      Serial.println(JSONmessageBuffer);
      
      
    } else if(buttonState == HIGH && controlhe == true){
      String hora = ntp.getFormattedTime();//Váriavel que armazenara o horario do NTP.
      JSONencoder["valor"] = "FALSE";
      JSONencoder["hora"] = hora;
      char JSONmessageBuffer[300];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      digitalWrite(LED_BUILTIN, HIGH); // Apagar o led
      Serial.write("HIGH"); // Escrever no console
       ///////////////////ENVIA OS DADOS PARA O SERVIDOR VIA POST/////////////////////////////
 
      HTTPClient http;    //Declare object of class HTTPClient
   
      http.begin("http://10.0.0.2:3000/alarme");      //Especifica o destino da requisição
      http.addHeader("Content-Type", "application/json");  //Especifica o content-type header
   
      int httpCode = http.POST(JSONmessageBuffer);//Envia a requisição
      String payload = http.getString();//Recupera a resposta payload
   
      Serial.println("codigo de retorno HTTP" + httpCode);//Imprime o codigo de retorno HTTP
      Serial.println("resposta da requisição payload" + payload);//Imprime a resposta da requisição payload
      if(payload == "true"){
        controlhe = true;
      }else if(payload == "false"){
        controlhe = false;
      }
      http.end();//Fecha conecção
      Serial.println(JSONmessageBuffer);      
    }else {
      String hora = ntp.getFormattedTime();//Váriavel que armazenara o horario do NTP.
      JSONencoder["valor"] = "status";
      JSONencoder["hora"] = hora;
      char JSONmessageBuffer[300];
      JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
      digitalWrite(LED_BUILTIN, HIGH); // Apagar o led
      Serial.write("HIGH"); // Escrever no console
       ///////////////////ENVIA OS DADOS PARA O SERVIDOR VIA POST/////////////////////////////
 
      HTTPClient http;    //Declare object of class HTTPClient
   
      http.begin("http://10.0.0.2:3000/alarme");      //Especifica o destino da requisição
      http.addHeader("Content-Type", "application/json");  //Especifica o content-type header
   
      int httpCode = http.POST(JSONmessageBuffer);//Envia a requisição
      String payload = http.getString();//Recupera a resposta payload
   
      Serial.println("codigo de retorno HTTP" + httpCode);//Imprime o codigo de retorno HTTP
      Serial.println("resposta da requisição payload" + payload);//Imprime a resposta da requisição payload
      if(payload == "true"){
        controlhe = true;
      }else if(payload == "false"){
        controlhe = false;
      }
      http.end();//Fecha conecção
      Serial.println(JSONmessageBuffer);    
    }
 
  } else {
 
    Serial.println("Erro na conecção WIFI");
    
  }
  delay(100);  //Send a request every 30 seconds
 
}
