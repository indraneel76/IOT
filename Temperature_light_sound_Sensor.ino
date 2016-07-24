#include <dht.h>

#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
// This means that you need to connect the TX line from the esp to the Arduino's pin 2
// and the RX line from the esp to the Arduino's pin 3

dht DHT;

#define DHT11_PIN 7
int soundSensorPin=A1;
const int LDR =0 ;

void setup(){
  Serial.begin(9600);
  pinMode(soundSensorPin,INPUT);
}

void loop()
{
Serial.begin(115200);
esp8266.begin(115200); // your esp's baud rate might be different
sendData("AT+RST\r\n",2000,DEBUG); // reset module
sendData("AT+CWMODE=1\r\n",3000,DEBUG); // configure as access point
sendData("AT+CWJAP=\"inda\",\"7506047379\"\r\n",3000,DEBUG);

sendData("AT+CIFSR\r\n",3000,DEBUG); // get ip address

Serial.println("Start of operation") ;
sendData("AT+CIPSTART=\"TCP\",\"1-dot-indaiot-1368.appspot.com\",80\r\n",3000,DEBUG);
sendData("AT+CIPSEND=72\r\n",3000,DEBUG);


//String sendcommand = "GET http://1-dot-indaiot-1368.appspot.com/r/t/28/80 HTTP/1.1\r\n\r\n\r\n";
String sendcommand = "GET http://1-dot-indaiot-1368.appspot.com/r/t/";

String temp=getTemp();
sendcommand.concat(temp);
sendcommand.concat("/");

String light =getLight();
sendcommand.concat(light);
sendcommand.concat("/");

String sound =getSound();
sendcommand.concat(sound);


sendcommand.concat(" HTTP/1.1\r\n\r\n\r\n");
Serial.println(sendcommand) ; 
//Serial.print(sendcommand.length());

String resp=sendData(sendcommand,5000,DEBUG);
Serial.println("http response start") ;
Serial.println(resp);
Serial.println("http response stop") ;


sendData("AT+CIPCLOSE\r\n",10000,DEBUG) ;
Serial.println("completed operation") ;
delay(30000) ;  
}



String getTemp ()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  String temp=String(DHT.temperature,0) ;
  Serial.println(temp);
  String humidity=String(DHT.humidity,0) ;
  Serial.print("Humidity = ");
  Serial.println(humidity);
  delay(5000);
 String value=String() ;
 value.concat(temp);
 value.concat("/");
 value.concat(humidity) ;

return value;
}

String getLight()
{
int lightvalue=0;

 lightvalue = analogRead(LDR);    // read the input pin

 //lightvalue=lightvalue-976;
 String lightvaluestr = String(lightvalue);
 Serial.print("light=");
 Serial.print(lightvaluestr);
 return lightvaluestr;
}
 

String getSound()
{
  int soundReading =analogRead(soundSensorPin);
  String soundStrReading ;
  soundStrReading = String(soundReading);
  return soundStrReading;
}

String sendData(String command, const int timeout, boolean debug)
{
String response = "";

esp8266.print(command); // send the read character to the esp8266

long int time = millis();

while( (time+timeout) > millis())
{
while(esp8266.available())
{

// The esp has data so display its output to the serial window 
char c = esp8266.read(); // read the next character.
response+=c;
} 
}

if(debug)
{
Serial.print(response);
}

return response;
}

