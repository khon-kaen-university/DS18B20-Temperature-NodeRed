#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define pin D2

OneWire oneWire(pin);

DallasTemperature sensors(&oneWire);
int sensorValue = 0; //set sensorValue
float Celcius = 0;
float Fahrenheit = 0;

const char* ssid = "xxxxxx"; // name wifi
const char* pass = "xxxx"; // password wifi
String sensor_id = "x"; // id
const char *host = "xxxxxxx"; // url server
const int httpsPort = "xxxxx"; // port
String Link = "/xxxxxx"; // link to connect node red
const char fingerprint[] PROGMEM = "60 7F F1 02 28 6C 22 61 6A F0 8B 50 66 43 06 C3 D5 B3 67 BC";
String payload;

void setup() {
    Serial.begin(9600);
    
    // code make esp8266 connect to internet
    WiFi.begin(ssid,pass);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi Connected");  
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
    
  sensors.begin();
}

void loop()
{     
   // get value Temperature from ds18b20 
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celcius);
  
   // show output at console
   String sting_tempC = "";
   sting_tempC += Celcius;
   Serial.print("Temperature C = ");
   Serial.println(sting_tempC);
    
   String sting_tempF = "";
   sting_tempF += Fahrenheit;
   Serial.print("Temperature F = ");
   Serial.println(sting_tempF);
 
   // code connect to node red
   WiFiClientSecure httpsClient; 
   httpsClient.setFingerprint(fingerprint);
   httpsClient.setTimeout(5000);
   httpsClient.connect(host, httpsPort);

   payload = "[{\"id\":"+sensor_id+",\"tempC\":"+sting_tempC+",\"tempF\":"+ sting_tempF +"}]";
  if((httpsClient.print(String("POST ") 
        + Link + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Content-Type: application/json"+ "\r\n" +
        "Content-Length: 36" + "\r\n\r\n" +
        payload + "\r\n" +
        "Connection: close\r\n\r\n")))
    {
        Serial.println("POST-OK!!!");
    } 
    else 
    {
        Serial.println("POST-Failed!!!");   
    }
    delay(1000);


}
