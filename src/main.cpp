
/*************************************************************
  Download latest ERa library here:
    https://github.com/eoh-jsc/era-lib/releases/latest
    https://www.arduino.cc/reference/en/libraries/era
    https://registry.platformio.org/libraries/eoh-ltd/ERa/installation

    ERa website:                https://e-ra.io
    ERa blog:                   https://iotasia.org
    ERa forum:                  https://forum.eoh.io
    Follow us:                  https://www.fb.com/EoHPlatform
 *************************************************************/

// Enable debug console
// Set CORE_DEBUG_LEVEL = 3 first
// #define ERA_DEBUG
#include <Arduino.h>
#define DEFAULT_MQTT_HOST "remarkable-accountant.cloudmqtt.com"

// You should get Auth Token in the ERa App or ERa Dashboard
#define ERA_AUTH_TOKEN "b474420f-ee18-44cc-931e-c7c6c8e74704"


#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>

const char ssid[] = "Mk NhuTen  123   @";
const char pass[] = "Mk NhuTen  123   @";

ERaTimer timer;
//khai bao bien debound nut nhan
long long int lastMillisButton1=0;
long long int lastMillisButton2=0;
long long int lastMillisButton3=0;
//khai bao bien LED
int led1 = D1;
int led2 = D2;
int led3 = D3;
//khai bao nut nhan
int buttonPin1 = D5;
int buttonPin2 = D6;
int buttonPin3 = D7;
//
bool interruptFlag = false;


void IRAM_ATTR ISR_Button1() {
  if(millis() - lastMillisButton1 >20){
    ets_printf("ISR_Button1 triggered !");
    digitalWrite(led1,!digitalRead(led1));
  }
  lastMillisButton1 = millis();
} 

ERA_WRITE(V0)
{
  int state1 = param.getInt();
  digitalWrite(led1, state1);
  ERa.virtualWrite(V0, digitalRead(led1));
}
////////////////////////
void IRAM_ATTR ISR_Button2 () {
  if(millis() - lastMillisButton2 >20) {
    ets_printf("ISR_Button2 triggered !");
    digitalWrite(led2,!digitalRead(led2));
  }
  lastMillisButton2 = millis();
}

ERA_WRITE(V1)
{
  int state2 = param.getInt();
  digitalWrite(led2,state2);
  ERa.virtualWrite(V1,digitalRead(led2));
}
////////////////////////////////
void IRAM_ATTR ISR_Button3() {
  if(millis() - lastMillisButton3 > 20) {
    ets_printf("ISR_Button2 triggered !");
    digitalWrite(led3,!digitalRead(led3));
  }
  lastMillisButton3 = millis();
}

ERA_WRITE(V2)
{
  int state3 = param.getInt();
  digitalWrite(led3,state3);
  ERa.virtualWrite(V2,digitalRead(led3));
}



/* This function print uptime every second */
void timerEvent() {
  ERa.virtualWrite(V0,digitalRead(led1));
  ERa.virtualWrite(V1,digitalRead(led2));
  ERa.virtualWrite(V2,digitalRead(led3));
    ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);
}

void setup() {
    /* Setup debug console */
    Serial.begin(115200);
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);
    pinMode(led3,OUTPUT);
    pinMode(buttonPin1,INPUT_PULLUP);
    pinMode(buttonPin2,INPUT_PULLUP);
    pinMode(buttonPin3,INPUT_PULLUP);
    attachInterrupt(buttonPin1, ISR_Button1, RISING);
    attachInterrupt(buttonPin2, ISR_Button2, RISING);
    attachInterrupt(buttonPin3, ISR_Button3, RISING);
    xTaskCreate();
    ERa.begin(ssid, pass);

    /* Setup timer called function every second */
    timer.setInterval(1000L, timerEvent);
}

void loop() {
    ERa.run();
    timer.run();
}

