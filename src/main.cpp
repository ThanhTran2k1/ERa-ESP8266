
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

const char ssid[] = "AIoT JSC";
const char pass[] = "aiot1234";

ERaTimer timer;
// khai bao bien debound nut nhan
long long int deboundMillis = 0;

// khai bao bien LED
int led1 = D1;
int led2 = D2;
int led3 = D3;
// khai bao nut nhan
int buttonPin1 = D4;
int buttonPin2 = D6;
int buttonPin3 = D7;
//
bool flagButton1 = false;
bool flagButton2 = false;
bool flagButton3 = false;

void IRAM_ATTR ISR_Button1()
{
  if (millis() - deboundMillis > 20)
  {
    ets_printf("ISR_Button1 triggered !");
    flagButton1 = true;
    digitalWrite(led1, HIGH);
  }
  deboundMillis = millis();
}

ERA_WRITE(V0)
{
  int state1 = param.getInt();
  if (state1 == 1)
  {
    flagButton1 = true;
  }
  digitalWrite(led1, state1);
  ERa.virtualWrite(V0, digitalRead(led1));
}
/////////////////
void IRAM_ATTR ISR_Button2()
{
  if (millis() - deboundMillis > 20)
  {
    ets_printf("ISR_Button2 triggered !");
    flagButton2 = true;
    digitalWrite(led2, HIGH);
  }
  deboundMillis = millis();
}
ERA_WRITE(V1)
{
  int state2 = param.getInt();
  if (state2 == 1)
  {
    flagButton2 = true;
  }
  digitalWrite(led2, state2);
  ERa.virtualWrite(V1, digitalRead(led2));
}
///////////////////////////////
void IRAM_ATTR ISR_Button3() {
  if(millis() - deboundMillis >20) {
    ets_printf("ISR_Button3 triggered !");
    flagButton3 = true;
    digitalWrite(led3,HIGH);
  }
  deboundMillis = millis();
}
ERA_WRITE(V2)
{
  int state3 = param.getInt();
  if(state3 == 1)
  {
    flagButton3 = true;
  }
  digitalWrite(led3,state3);
  ERa.virtualWrite(V2,digitalRead(led3));
}






/* This function print uptime every second */
void timerEvent()
{
  ERa.virtualWrite(V0, digitalRead(led1));
  ERa.virtualWrite(V1, digitalRead(led2));
  ERa.virtualWrite(V2, digitalRead(led3));
  ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);
}

unsigned long time1;
unsigned long time2;
unsigned long time3;

void setup()
{
  /* Setup debug console */
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  attachInterrupt(buttonPin1, ISR_Button1, CHANGE);
  attachInterrupt(buttonPin2, ISR_Button2, CHANGE);
  attachInterrupt(buttonPin3, ISR_Button3, CHANGE);
  ERa.begin(ssid, pass);

  /* Setup timer called function every second */
  timer.setInterval(100L, timerEvent);
}

void loop()
{
  if (flagButton1 == true)
  {
    if ((unsigned long)(millis() - time1) > 3000)
    {
      digitalWrite(led1, LOW);
      flagButton1 = false;
      time1 = millis();
    }
  }

  if (flagButton2 == true)
  {
    if ((unsigned long)(millis() - time2) > 5000)
    {
      digitalWrite(led2, LOW);
      flagButton2 = false;
      time2 = millis();
    }
  }

  if(flagButton3 == true)
  {
    if((unsigned long)(millis() - time3) > 7000)
    {
      digitalWrite(led3,LOW);
      flagButton3 = false;
      time3 = millis();
    }
  }
  ERa.run();
  timer.run();
  
}
