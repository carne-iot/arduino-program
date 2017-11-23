

//Sensor libraries
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Http communication libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

 //uncomment  lines 41 to 57 if you get errors about pin definations (eg. error: unknown type name 'D1' etc) while compiling. This is to map exact GPIOs of ESP8266 with NodeMCU pins
// refer following link for more details - https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h#L37-L59 
// if ESP8266 package for Arduino is updated to latest version 2.1.0 or above, then pins_arduino.h should be automatically downloaded and errors for unknown type name should be solved


#define ONE_WIRE_BUS D1
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);   // constructor for our DS18B20 sensor to read temperature using DallasTemperature libary

//wifi credentials
char* ssid = "vloteto";
char* pass = "0070071234";
//device credentials
const String token = "eyJhbGciOiJIUzUxMiJ9.eyJ1aWQiOjAsImp0aSI6ODU0ODA2ODA2MjQ2MTkyNTcxNywicm9sZXMiOlsiUk9MRV9ERVZJQ0UiXSwiZGlkIjoxMjM0MTIzNCwic3ViIjoidXNlcm5hbWUiLCJpYXQiOjE1MTEzMDkzNzIsImV4cCI6OTIyMzM3MjAzNjg1NDc3NX0.9ZF-JR1ehWkjJVmBFGgMxODLl1h_KGXlC6Kh4ov3Ppb_39a8wi3JFtXKf7Pbg6NOzcnFzd5s6oY-cMUhAWylDA";


void setup()   {                
  Serial.begin(9600);
  sensors.begin();
  connectWifi();
  }

void loop() {



if(WiFi.status() == WL_CONNECTED){

    //Read Temperature
    float temperature = getTemp();
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

}

float getTemp(void) {   // function to read and display temperature
  
  sensors.requestTemperatures();           // Send the command to DS18B20 to get temperatures
  float temperatureDegC = sensors.getTempCByIndex(0);
  Serial.print("Temperature (Deg C):");   // enable just for debugging
  Serial.println(temperatureDegC);  // enable just for debugging prints the temperature in deg C 
 return temperatureDegC;
 //delay(500);  // wait for 3 seconds and then clear display, you can adjust this time to increase or decrease duration of displaying Deg C or Deg F
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



