#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
#include <ESP8266WiFi.h>

byte ledPin = D4;
char ssid[] = "AndroidAP";               // SSID of your home WiFi
char pass[] = "diehard1";               // password of your home WiFi
WiFiServer server(80);                    

IPAddress ip(192, 168, 43, 80);            // IP address of the server
IPAddress gateway(192,168,43,1);           // gateway of your network
IPAddress subnet(255,255,255,0);          // subnet mask of your network

volatile long timE, timE1;
uint8_t GPIO_Pin = D6;
uint8_t GPIO_Pin1 = D5;
volatile bool f, f1;
volatile byte p2;
String text = "                 ";
volatile char l;

void setup() {
  Serial.begin(9600);                   // only for debug
  pinMode(GPIO_Pin, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(GPIO_Pin), StartKey, FALLING);
 pinMode(GPIO_Pin1, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(GPIO_Pin1), Next, FALLING);
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  pinMode(ledPin, OUTPUT);
   lcd.begin();
 lcd.backlight();
 lcd.setCursor(0, 0);
 lcd.print("ESP8266");
 lcd.setCursor(0, 1);
 lcd.print("Hello, world!");
}

void loop () {
  if(!f){
    WiFiClient client = server.available();
    if (client) {
      if (client.connected()) {
        digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
        Serial.println(".");
        String request = client.readStringUntil('\r');    // receives the message from the client
        lcd.setCursor(0,0);
        lcd.print("From client: "); 
        lcd.setCursor(0, 1);
        lcd.print(request);
        client.flush();
        f1 = 1;
        text.setCharAt(17, '\r');
        client.println(text); // sends the answer to the client
        digitalWrite(ledPin, HIGH);
        delay(2000);
        lcd.clear();
      }
      client.stop();                // terminates the connection with the client
    }
  }
  else{
    if(f1){
      lcd.clear();
      //text = "";
      f1 = 0;
    }
    lcd.setCursor(0, 0);
    lcd.print("Send:");
    lcd.setCursor(p2, 1);
    l = map(analogRead(A0), 0, 1020, 97, 122);
    lcd.print(l);
    text.setCharAt(p2, l);
  }
}

void StartKey(){
  if(millis()-timE > 500){
    f = !f;
  }
  timE = millis();
}

void Next(){
  if(millis()-timE1 > 500){
    //text[p2] = l;
    p2++;
  }
  timE1 = millis();
}

