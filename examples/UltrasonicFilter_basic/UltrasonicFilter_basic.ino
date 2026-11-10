// basic implimentation of single sonar sensor
// TESTED with: this library, Ultrasonic library and Arduino R3

// this filter library is specifically for use with  
// Ultrasonic 3.0.0 by by Erik Simoes
// install both from the arduino library interface 



#include <Ultrasonic.h>        // you may need to install these ... Ultrasonic 3.0.0 by by Erik Simoes
#include "UltrasonicFilter.h"  // by hex705 with help from Mark Argo and David Bouchard

// Ultra sonic sensor pin definitions
// NOTE: due to novel startup behaviour, UNO R4 cannot use pin 13 with this sensor / library
int trigpin = 11;  // trigger pin
int echopin = 12;  // echo pin

Ultrasonic ultra(trigpin, echopin);  // create sensor object as per Ultrasonic library 
UltrasonicFilter ultraFilter(&ultra);   // the & is important - not a mistake

// variable to hold current reading 
int filteredDistance;
int maxDistance = 178;  // set a max distance from this list ::  178,196,214,232,250,268,285,303 --> see distanceTable tab

// additional values from UltrasonicFilter Library
int emaDistance, raw, presence;


void setup() {
  Serial.begin(9600);
  ultraFilter.begin(maxDistance);  // max distance you want to read --> see distanceTable tab
  // emaProportion default 0.1
  ultraFilter.setEmaProportion(0.4); // 1.0 = no filter, 0.5 = weak filter - feels responsive, 0.001 = strong filter - less responsive, default 0.1
}

void loop() {

  // because of sensor hardware limits we need to ask if new data is ready
  // this runs at 12.5 reads per second ( 1/ 80 mS)
  // rather than a callback, this strategy follows the Serial.available() interface
  // at this time, the  .available() call is needed to read the sensor

  if (ultraFilter.available()){    // if new reading available (triggers the read)
    // typically use one or other of these
    filteredDistance = ultraFilter.getEma(); // return new basic filtered reading
  }

  Serial.print("emaFilter:");
  Serial.println(filteredDistance);   

}
