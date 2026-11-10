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

  ultraFilter.setEmaProportion(0.3); // 1.0 = no filter, 0.001 = strong filter, very slow response, default 0.1
  //ultraFilter.setActiveInterval(500);  // default is 1000 this parameter is like a sustain pedal? attention span?
}

void loop() {

  // because of sensor hardware limits we need to ask if new data is ready
  // this runs at 12.5 reads per second ( 1/ 80 mS)
  // rather than a callback, this strategy follows the Serial.available() interface
  // at this time, the  .available() call is needed to read the sensor

  if (ultraFilter.available()){    // if new reading available (triggers the read)
    // typically use one or other of these
    filteredDistance = ultraFilter.read(); // return new basic filtered reading
    emaDistance = ultraFilter.getEma();    // return EMA filter of readingS
   
    // extras
    // raw -- to see more clearly what its doing
    raw = ultraFilter.getRaw();    // return reading with no filter
    // use sensor as a digital presence / absense detector 
    presence = ultraFilter.getState();  // return presence / absence
  }

  printLessStuff();
 // printMoreStuff(); // VERBOSE -- prints raw, distance, ema, state, maxDistance, and 0 (for scale). 

}

void printLessStuff(){
   //https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter/

  Serial.print("basicFilter:");
  Serial.print(filteredDistance);    // 1 :: filters hardware timer noise -- cleaner but lots of jaggy edges
  Serial.print("\t");

  Serial.print("EMAFilter:");
  Serial.println(emaDistance);       // 2 :: ema filtered added to distance -- see setup for config.
}

// see some values the filter can return ... 
void printMoreStuff() {
  //https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-plotter/
 
  Serial.print("basicFilter:");
  Serial.print(filteredDistance);  // 1 :: software timeout filtered value -- cleaner but lots of jaggy edges
  Serial.print("\t");

  Serial.print("EMAFilter:");
  Serial.print(emaDistance);  // 2 :: ema filtered data try 0.1
  Serial.print("\t");
  
  Serial.print("Raw:");
  Serial.print(raw);          // 3 :: raw sensor value (timer limited) no fliter 
  Serial.print("\t");

  Serial.print("Presence:");
  Serial.print(presence *-50); // 4 :: sensor state (0,1): 1 = something present -- *-50 scales for plotter
  Serial.print("\t");

  Serial.print("zero:");
  Serial.print(0);             // 5 :: force scaling in plotter
  Serial.print("\t");

  Serial.print("max:");
  Serial.println(maxDistance); // 6 :: max distance 

}