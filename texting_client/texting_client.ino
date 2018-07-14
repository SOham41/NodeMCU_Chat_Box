#include <LiquidCrystal_I2C.h>        ///lcd library
LiquidCrystal_I2C lcd(0x3F, 16, 2);     ///initializing 16x2 lcd with addres 0x3F,

#include <ESP8266WiFi.h>

byte ledPin = D4;   ///'inbuilt led pin
char ssid[] = "AndroidAP";               // SSID of your home WiFi
//WiFiServer server(80);                    

IPAddress server(192, 168, 43, 80);            // IP address of the server

WiFiClient client;  //to get and send data

volatile long timE, timE1; //for storing time
uint8_t GPIO_Pin = D6;    
uint8_t GPIO_Pin1 = D5;
volatile bool f, f1;  //flags  f for
volatile byte p2;     //cursor position
String text = "                 "; //text to be sent - initialised with spaces
volatile char l;  //char returned by potentiometer

void setup() {
 Serial.begin(9600);
 pinMode(GPIO_Pin, INPUT_PULLUP);  //input_pulup - connect pin to high when not connected to anything
 attachInterrupt(digitalPinToInterrupt(GPIO_Pin), StartKey, FALLING); //whenever something happens at this pin - D5 - do function Startkey
 pinMode(GPIO_Pin1, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(GPIO_Pin1), Next, FALLING);
 //WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
 WiFi.begin(ssid, pass);                 // connects to the WiFi router
 while (WiFi.status() != WL_CONNECTED) {
   Serial.print(".");
   delay(500);
 }
 pinMode(ledPin, OUTPUT);
 lcd.begin();                         //initialise lcd
 lcd.backlight();                     //swwitch on lcd backlight
 lcd.setCursor(0, 0);                 //set cursor to 0th column, 0th row
 lcd.print("ESP8266");                
 lcd.setCursor(0, 1);                 //set cursor to 0th column, 1st row
 lcd.print("Hello, world!");
}

void loop() {
  if(!f){                ///this flag will check if the keyboard has been enabled or not
    p2 = 0;              //set cursor to 0 each time
    client.connect(server, 80);  //connect to port 80 with servers ip address
    digitalWrite(ledPin, LOW);  //led on
    //if(!f1){
      lcd.clear();        //clear lcd
      f1 = 1;             //set flag for clearing the lcd when entering keyboard
      text.setCharAt(17, '\r'); //add terminating character to end of the text to be sent
      client.println(text); // sends the answer to the client
      digitalWrite(ledPin, HIGH);
    //}
    String answer = client.readStringUntil('\r'); //read
    lcd.setCursor(0, 0);
    lcd.print("From Server:");
    lcd.setCursor(0, 1);
    Serial.println(answer);
    lcd.print(answer);
    client.flush();  //empty the buffer
    digitalWrite(ledPin, HIGH); //led low
    delay(2000);
  }
  else{  //if it has entered keyboard
    if(f1){ //if lcd clearing has been enabled.
      lcd.clear();
      f1 = 0; //reset
    }
    lcd.setCursor(0, 0);
    lcd.print("Send:");
    lcd.setCursor(p2, 1); //set cursor according to the increment
    l = map(analogRead(A0), 0, 1020, 97, 122); //read the analog values and map it to corresponding values of ascii
    lcd.print(l);
    text.setCharAt(p2, l); //add the character at the end of the text
  }
}

void StartKey(){
  if(millis()-timE > 500)
    f = !f;
  timE = millis();
}

void Next(){  // go to next cursor space
  if(millis()-timE1 > 500){
//    text[p2] = l;
    p2++;
  }
  timE1 = millis();
}

