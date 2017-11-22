#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <OneWire.h>

#include <DallasTemperature.h>

#define LED_BUILTIN 2

//wifi credentials
char* ssid = "vloreto2";
char* pass = "0070071234";
//device credentials
const String token = "eyJhbGciOiJIUzUxMiJ9.eyJ1aWQiOjAsImp0aSI6ODU0ODA2ODA2MjQ2MTkyNTcxNywicm9sZXMiOlsiUk9MRV9ERVZJQ0UiXSwiZGlkIjoxMjM0MTIzNCwic3ViIjoidXNlcm5hbWUiLCJpYXQiOjE1MTEzMDkzNzIsImV4cCI6OTIyMzM3MjAzNjg1NDc3NX0.9ZF-JR1ehWkjJVmBFGgMxODLl1h_KGXlC6Kh4ov3Ppb_39a8wi3JFtXKf7Pbg6NOzcnFzd5s6oY-cMUhAWylDA";

//TODO setear bien el pin que se use
const int sensorPin = 6;

OneWire ds(sensorPin);
DallasTemperature sensor(&ds);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  connectWifi();
  sensor.begin();
}

void loop() {

   //Turn led on
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
  
  if(WiFi.status() == WL_CONNECTED){

    //Read Temperature
    sensor.requestTemperatures();
    float temperature = sensor.getTempCByIndex(0);
    
    Serial.print("temperature: ");
    Serial.println(temperature); 

    HTTPClient http;    //Declare object of class HTTPClient
    http.begin("http://server.carne-iot.itba.bellotapps.com/devices/MTIzNDEyMzQ=/temperature");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
    http.addHeader("X-Hidden-Method", "put");
    http.addHeader("Accept", "application/json");
    http.addHeader("Authorization", String("Bearer " + token)); 
    String body = String("{\"value\":\"") + temperature + String("\"}");
 
    int httpCode = http.POST(body);   //Send the request
    String payload = http.getString();  //Get the response payload

    Serial.print("Http Code: ");
    Serial.println(httpCode);   //Print HTTP return code
 
    http.end();  //Close connection
  }else{
    Serial.println("Wifi not connected");
  }

  //Turn led off
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);

}

void connectWifi(){
  WiFi.begin(ssid, pass);
  Serial.println("Connecting.");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
  
}

float getTemp(){
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)){
     
     ds.reset_search();
     return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC no valido");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.print("Dispositivo no reconocido");
    return -1000;
  }
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // Se envía comando para conversión de temperatura
 
  byte present = ds.reset();
  ds.select(addr);  
  ds.write(0xBE); // Lectura de scratchpad
 
 
  for (int i = 0; i < 9; i++) { // Almacenamos los 9 bytes del scratchpad
    data[i] = ds.read();
  }
 
  ds.reset_search();
 
  // Valores de temperatura almacenados en lo bytes 0 y 1
  byte MS_Byte = data[1];
  byte LS_Byte = data[0];
  // Conversion de temperatura en grados celsius
  float tempRead = ((MS_Byte << 8) | LS_Byte); 
  float TemperatureSum = tempRead * 0.0625;
 
  return TemperatureSum;
}

