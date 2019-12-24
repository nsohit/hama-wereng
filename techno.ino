#include "config.h"
#define echoPin D7 //pin ultrasonik
#define trigPin D6 //pin ultrasonik
#include <Wire.h> 
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);
long duration;
int distance;
int ldrPin = D8; // pin LDR
int ledPin= D13; // pin relay
int val = 0;
AdafruitIO_Feed *ultrasonic = io.feed("ultrasonik");

AdafruitIO_Feed *rtc = io.feed("RTC");
int relay = D13;
AdafruitIO_Feed *ldr = io.feed("LDR");

void setup() {
//RTC Punya
Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        

        Serial.println("RTC lost confidence in the DateTime!");


        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

   
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
    // sampe sini
    
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relay,OUTPUT);

    Serial.begin(57600);
    
  while(! Serial);

  
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  
  io.run();
//untuk ULTRASONIC
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  Serial.println(distance);
  Serial.print("sending button -> ");
  Serial.println(distance);
  ultrasonic->save(distance);
  delay(1500); // sampe sini

//UNTUK LDR
  int val = digitalRead(ldrPin);
  Serial.println(val);
 if(val == 0 ){
    //digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
    delay(3000);
    ldr->save("Lampu On");
  }
  else{
    //digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
    ldr->save("Lampu Off");

    delay(3000);
  } //sampe sini
//untuk RTC
  if (!Rtc.IsDateTimeValid()) 
    {
        
        Serial.println("RTC lost confidence in the DateTime!");
    }

    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();


}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
   // Serial.print(datestring);
  rtc->save(datestring);
if ((dt.Hour() >= 17 ) || (dt.Hour() <= 6)) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(3000); // sampe kelar :*
  }



