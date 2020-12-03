/*
Released under Creative Commons Attribution 4.0
by bitluni 2016
https://creativecommons.org/licenses/by/4.0/
Attribution means you can use it however you like as long you
mention that it's base on my stuff.
I'll be pleased if you'd do it by sharing http://youtube.com/bitlunislab
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 200
// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
//#define CLOCK_PIN 13
//const int BUTTON_PIN = 10;
#define BRIGHTNESS  5
int buttonState = 0;
uint8_t globalhue = 0;

//Predefined Modes and Effects
int lightmode = 0;

// Define the array of leds
CRGB leds[NUM_LEDS];



const char* ssid = "Vancer";
const char* password = "0000000000";

ESP8266WebServer server(80);



const int RF_OSC = 200;



void handleRoot() {
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px'>Following functions are available:<br><br>";
  message += "<a href='/rainbow?fade=3000'>/rainbow</a> a rainbow animation on LEDs<br>";
  message += "<a href='/wave?r=255&g=32&b=10&fade=5000'>/wave</a> a slow wave animation on LED on base color specified by arguments: r=<0..255> g=<0..255> b=<0..255><br>";
  message += "<a href='/setleds?r=32&g=64&b=32&fade=1000'>/setleds</a> sets LEDs to the color from arguments: r=<0..255> g=<0..255> b=<0..255><br>";
  message += "<a href='/ledsoff?fade=500'>/ledsoff</a> turns off LEDs<br>";
  message += "<a href='/setpins?D1=128&D2=256&D3=512'>/setpins</a> sets to any of the in arguments specified pins (D0..D8) to their PWM values (0..1023). To use them digital: 0=off, 1023=on<br>";
  message += "<a href='/togglepins'>/togglepins</a> inverts all pin values form pins used before.<br>";
  message += "<a href='/rf?D=6&t=200&id=28013&channel=0&on=1'>/rf</a> sends a rf code from arguments: D=<0..8> t=<0..1000> id=<0..1048575> channel=<0..2> on=<0..1>. Dx is the pin, t is the optional signal clock(default is 200, works for me)<br><br>";
  message += "All functions except togglepins and rf support the argument 'fade' which specifies the milliseconds it takes to fade to the new specified state. ...nice blending ;-)<br>";
  message += "<br>Syntax is as follows: http://&ltip>/&ltcommand>?&ltargument1>=&ltvalue1>&&ltargument2>=&ltvalue2>&...<br>";
  message += "You can click on each link to see an example.<br><br>";
  message += "have fun -<a href='http://youtube.com/bitlunislab'>bitluni</a></body></html>";
  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

int getArgValue(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if(server.argName(i) == name)
      return server.arg(i).toInt();
  return -1;
}











void setup() { 
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
 //pinMode(BUTTON_PIN,INPUT);
 
  WiFi.softAPdisconnect(true);
  WiFi.disconnect(true);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //find it as http://lights.local
  /*if (MDNS.begin("lights")) 
  {
    Serial.println("MDNS responder started");
  }*/
  
  server.on("/", handleRoot);

    server.on("/butthole", [](){
    server.send(200, "text/plain", "butthole");
    lightmode = 0;
  });

  server.on("/rainbow", [](){
    server.send(200, "text/plain", "rainbow");
    lightmode = 0;
  });

  server.on("/wave", [](){
    server.send(200, "text/plain", "wave");
   lightmode = 1;
  });

  server.on("/setleds", [](){
    server.send(200, "text/plain", "setleds");
   lightmode = 2;
  });

  server.on("/ledsoff", [](){
    server.send(200, "text/plain", "ledsoff");
    lightmode = 3;
  });

  server.on("/togglepins", [](){
    server.send(200, "text/plain", "togglepins");
    lightmode = 4;
  });
  
  server.on("/setpins", [](){
    server.send(200, "text/plain", "setpins");
   lightmode = 5;
});
  server.on("/pinsoff", [](){
    server.send(200, "text/plain", "pinsoff");
     lightmode = 6;
  });

  //server.on("/rf", handleRf);
  
  server.onNotFound(handleNotFound);


  server.begin();
  Serial.println("HTTP server started");
  
 // strip.begin();
 // strip.show(); // Initialize all pixels to 'off'
}
void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }
void loop(void)
{
  server.handleClient();
  //MDNS.update();
 
  

   if (lightmode == 0) {
 globalhue = RainDrop(0,175,255,globalhue);
  Serial.print(globalhue);
  }
  
 if (lightmode == 1)
  {
globalhue = RainSplash(0,175,255,globalhue);
  }

   if (lightmode == 2)
  {
    globalhue = CyclonLightBKWD(200,50,255,0);
    globalhue = CyclonLightFWD(50,200,255,0);
  }
  
 // CyclonLightBKWD(200,50,255,0);
  //CyclonLightFWD(50,150,255,0);
 //CyclonLightBKWD(150,75,255,0);
 //CyclonLightFWD(75,150,255,0);
 //CyclonLightBKWD(150,75,255,0);
 //CyclonLightFWD(75,200,255,0);
 // CyclonLightBKWD(200,0,5,0);
}

 uint8_t CyclonLightFWD(int StartBulb,int End_Bulb,int Brightness, uint8_t hue)
 {
   LEDS.setBrightness(Brightness);
 // static uint8_t hue = 0;
  //Serial.print("x");
  // First slide the led in one direction
  for(int i = StartBulb; i < End_Bulb; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  return hue;
 }
  uint8_t CyclonLightBKWD(int StartBulb,int End_Bulb,int Brightness, uint8_t hue){
 LEDS.setBrightness(Brightness);
  //Serial.print("x");
  // Now go in the other direction.  
  for(int i = (StartBulb)-1; i >= End_Bulb; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  return hue;
 }

  uint8_t RainDrop(int Start_Bulb,int End_Bulb,int Brightness, uint8_t hue){
   
  int Center_Bulb = (Start_Bulb + End_Bulb)/2;
    LEDS.setBrightness(Brightness);
 // static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = Center_Bulb; i < End_Bulb; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    int Mirror_Bulb = i + (Center_Bulb - i) * 2; 
    leds[Mirror_Bulb] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
    //
  }
  return hue;
 }

 uint8_t RainSplash(int Start_Bulb,int End_Bulb,int Brightness, uint8_t hue){
   
  int Center_Bulb = (Start_Bulb + End_Bulb)/2;
    LEDS.setBrightness(Brightness);
 // static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = Start_Bulb; i < Center_Bulb; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    int Mirror_Bulb = i + (Center_Bulb - i) * 2; 
    leds[Mirror_Bulb] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
    
  }
  return hue;
 }






uint8_t RainLoop(int Start_Bulb,int End_Bulb,int Brightness, uint8_t hue){
   
  int Center_Bulb = (Start_Bulb + End_Bulb)/2;
    LEDS.setBrightness(Brightness);
 // static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = Start_Bulb; i < End_Bulb; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    int Mirror_Bulb = i + (Center_Bulb - i) * 2; 
    leds[Mirror_Bulb] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
    
  }
  return hue;
 }
