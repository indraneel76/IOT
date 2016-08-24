
#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define DEBUG true

//#define SERIAL_BUFFER_SIZE 512

// make RX Arduino line is pin 2, make TX Arduino line is pin 3.
// This means that you need to connect the TX line from the esp to the Arduino's pin 2
// and the RX line from the esp to the Arduino's pin 3

#define RELAY1  7 

void setup(){
  pinMode(13, OUTPUT);
   pinMode(RELAY1, OUTPUT); 
  digitalWrite(13, LOW);
  digitalWrite(RELAY1,HIGH); // Turns Relay Off
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial.flush(); 
   
  

}

void loop()
{

Serial.flush();
delay(5000);
//Reseting ESP module
resetESPModule();

delay(2000);
clearbuffer();
//configure ESP as Access point
configESPModule();
delay(2000);
clearbuffer();
//connect to wifi
connectToWIFI();

delay(2000);
clearbuffer();
//RequestforAction
RequestforAction();

delay(3000);
clearbuffer();
//closing connection
closeConnection();

clearbuffer();

delay(5000);
 
}





void resetESPModule()
{



Serial1.print("AT+RST\r\n"); // send the read character to the Serial
Serial1.flush();
long int time = millis();
delay(2000);

while( (time+7000) > millis())
{


while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c);
}
}



Serial1.flush();
}

void configESPModule()
{
Serial1.print("AT+CWMODE=1\r\n"); // send the read character to the Serial
Serial1.flush();
long int time = millis();

delay(2000);
while( (time+7000) > millis())
{


while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c);
}
}

Serial.flush();
}


void connectToWIFI()
{

Serial1.print("AT+CWJAP=\"WIFI_SSID\",\"WIFI_PASSWORD\"\r\n"); // send the read character to the Serial
Serial.flush();
long int time = millis();
while( (time+7000) > millis())
{


while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c) ;
}
}

Serial.flush();
}


void RequestforAction()
{

Serial1.print("AT+CIPSTART=\"TCP\",\"1-dot-indaiot-1368.appspot.com\",80\r\n"); // send the read character to the Serial
Serial.flush();
long int time = millis();
long i=0 ;
delay(2000);

while( (time+7000) > millis())
{


while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c) ;
}
}


Serial1.print("AT+CIPSEND=66\r\n"); // send the read character to the Serial
Serial.flush();
time = millis();
while( (time+7000) > millis())
{
while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c) ;
}
}


delay(2000);
String sendcommand = "GET http://1-dot-indaiot-1368.appspot.com/r/action/";
sendcommand.concat(" HTTP/1.1\r\n\r\n\r\n");


char resp3[512]="";
String response ;
response.reserve(400);
Serial1.print(sendcommand); // send the read character to the Serial
//Serial.flush();
time = millis();
 i=0 ;
//delay(1000);


while( (time+10000) > millis())
{


while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c);
response+=c;
resp3[i]=c;
i++;
//delay(1000);
}
}


if (response.indexOf("START") >=0)
{
 Serial.println("lightLed"); 
lightLed();
}

if (response.indexOf("STOP") >=0)
{
  noLightLed();
  Serial.println("NolightLed"); 
}




Serial.flush();
}


void closeConnection()
{

Serial1.print("AT+CIPCLOSE\r\n"); // send the read character to the Serial
Serial.flush();
long int time = millis();

while( (time+5000) > millis())
{


while(Serial1.available())
{

// The esp has data so display its output to the serial window 
char c = Serial1.read(); // read the next character.
Serial.print(c);
}
}

Serial.flush();
}




void clearbuffer()
{
  while(Serial.available())
{

// The esp has data so display its output to the serial window 
char c = Serial.read(); // read the next character.
}
}

void lightLed()
{
  digitalWrite(13, HIGH);
  digitalWrite(RELAY1,LOW); // Turns Relay Off
}

void noLightLed()
{
  digitalWrite(13, LOW);
  digitalWrite(RELAY1,HIGH); // Turns Relay Off
}

/*
void serialEvent() {
  lcd.autoscroll();
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
  //  inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    lcd.write(inChar);
  }
}

*/
