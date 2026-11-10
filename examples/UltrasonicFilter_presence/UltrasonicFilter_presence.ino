#include <Ultrasonic.h>        // you may need to install these ... Ultrasonic 3.0.0 by by Erik Simoes
#include "UltrasonicFilter.h"  // by hex705 with help from Mark Argo and David Bouchard

// Ultra sonic sensor pin definitions
// NOTE: due to novel startup behaviour, UNO R4 cannot use pin 13 with this sensor / library
int trigpin = 11;  // trigger pin
int echopin = 12;  // echo pin

// sensor and filter objects 
Ultrasonic ultra(trigpin, echopin);  // create object
UltrasonicFilter usFilter(&ultra);   // the & is important - not a mistake

// EMA filters to demo how to get values ... 
int local_ema;
float local_emaValue = 0.05;
 
// determine presence absence of users
#define PRESENT 1
#define ABSCENT 0
int USER_STATE= ABSCENT;

// reading variables
int distance;  // filtered reading from filter
int maxDistance = 196;  //  // set a max distance from this list :: 178,196,214,232,250,268,285,303 --> see distanceTable tab
unsigned long activeInterval = 500; // sort of like a sustain pedal, length of time filter ignores maxDistance values after last good reading


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); 
  usFilter.begin(maxDistance);  // max distance you want to read --> see distanceTable tab
  usFilter.setActiveInterval(activeInterval); // this value must be unsigned long - we can add variable or raw # with suffix UL- eg: 2000UL!
}


void loop() {

  // get a reading ...
  distance = usFilter.update(); 

  // a reading will be positive (distance > 0 ) if filter libarary timer has expired
  // if timer says not yet, update returns -1

  // do stuff with new reading
  if (distance > 0) {  // true when we have a new reading...

     // do stuff with the new reading 
     if (distance < 50) {
        digitalWrite(LED_BUILTIN, HIGH); 
      } else {
        digitalWrite(LED_BUILTIN, LOW); 
     }

    printStuff();

   } //  end distance positive > 0


    // take action ... this is very basic uses 
    // in addition to readings, the filter will be in ACTIVE (I see something) or STANDBY (nothing to see here)
    // novel thresholds could also be set ... 

    USER_STATE = usFilter.getPresence(); // getState() also works for now

    switch (USER_STATE) {

      case ABSCENT:  // 0
        noTone(10);  // no sound
        break;

      case PRESENT:  // 1
        int freq = map(distance, 0, usFilter.maximumDistance, 100, 1500);
        tone(10, freq);
        break;
    } // end switch ...
}


// see some values the filter can return ... 
void printStuff() {

  //must call update OR available before calling functions below
  Serial.print("raw:");
  Serial.print(usFilter.getRaw());  // 1 :: raw sensor no fliter 
  Serial.print("\t");
  
  Serial.print("basicFilter:");
  Serial.print(distance);           // 2 :: filtered value -- cleaner but lots of jaggy edges
  Serial.print("\t");

  Serial.print("EMAFilter:");
  Serial.print(usFilter.getEma());  // 3 :: filter can do some EMA smoothing 
  Serial.print("\t");

  Serial.print("Presence:");
  Serial.print(USER_STATE*-20);    // 5 :: presence / absence 1 = see something, 0 = nothing
  Serial.print("\t");

  Serial.println(usFilter.maximumDistance);  // 6
}