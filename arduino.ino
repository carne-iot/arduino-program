#include <HttpClient.h>

#include <MRF24G.h>
#include <DEIPcK.h>
#include <DEWFcK.h>

// TODO: PONER API ACA
const char * serverName = "things.ubidots.com"; // Servidor a conectarse
const uint16_t serverPort = 80; // Puerto HTTP, en este caso 80 (web)

//TODO setear bien el pin que se use
const int sensorPin = 0;

// Network constants
const char * ssid = "Judi’s MacBook Pro";
const char * pass = "judijudi";
IPSTATUS status;
#define WiFiConnectMacro() deIPcK.wfConnect(ssid, pass, &status)

void setup() {
  Serial.begin(9600);
}

void loop() {

  // Connect to network
  if (WiFiConnectMacro()) {
    Serial.println("Conectado");
    deIPcK.begin();
  } else if (IsIPStatusAnError(status)) {
    Serial.print("Error: ");
    Serial.println(status, DEC);
    return;
  }
  
  
  // Initialize the client library
  HttpClient client;

  ///TODO

  // if there are incoming bytes available
  // from the server, read them and print them:

      //Read Temperature
      
      //getting the voltage reading from the temperature sensor
      int reading = analogRead(sensorPin);  
      
      // converting that reading to voltage, for 3.3v arduino use 3.3
      float voltage = reading * 5.0;
      voltage /= 1024.0; 
      
      // now print out the temperature
      float temperature = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)

      //TODO
      const char * urlPath= "POST TODO";
      client.post(serverName, serverPort, urlPath);


  delay(5000);
}

