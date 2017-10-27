//#include <HttpClient.h>
#include <OneWire.h>

// TODO: PONER API ACA
const char * serverName = "https://464975a0-5659-4966-a7a6-634053404b58.mock.pstmn.io"; // Servidor a conectarse
const uint16_t serverPort = 80; // Puerto HTTP, en este caso 80 (web)
// Network constants
const char * ssid = "Judi’s MacBook Pro";
const char * pass = "judijudi";

//TODO setear bien el pin que se use
const int sensorPin = 8;

OneWire ds(sensorPin);

void setup() {
  Serial.begin(9600);
}

void loop() {

  //TODO: connect to internet
   
      //Read Temperature
      float temperature = getTemp();
      //getting the voltage reading from the temperature sensor
      Serial.println(temperature); 


    // Initialize the client library
//    HttpClient client;
//
//      //TODO postear temp
//      const char * urlPath= "https://464975a0-5659-4966-a7a6-634053404b58.mock.pstmn.io/hola";
//      int ret = client.post(serverName, serverPort, urlPath);
//      Serial.print(ret);

  delay(1000);
}
float getTemp()
{
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) 
  {
     
     ds.reset_search();
     return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) 
  {
    Serial.println("CRC no valido");
    return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) 
  {
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
