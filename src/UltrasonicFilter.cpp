#include <UltrasonicFilter.h>

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


UltrasonicFilter::UltrasonicFilter(Ultrasonic * _sensor){ 
   sensor = _sensor;
  // sensor -> setTimeout(17000UL); // largest suggested by library ... 
}

void UltrasonicFilter::begin(){
  //sensor -> setTimeout(17000UL); // largest suggested by library ... 
  //maximumDistance = 303;
}

void UltrasonicFilter::begin( int  _maxDistance ){
  setMaximumDistance(_maxDistance);

  //  switch (_maxDistance)
  // sensor -> setTimeout(17000UL); // largest suggested by library ... 
  // maximumDistance = _maxDistance;
}

// int UltrasonicFilter::poll(){
//    return available();
// }

// update -- uses default timing of 1 sec or override with own value 
int  UltrasonicFilter::available(){
  returnValue = update();

  if ( returnValue == -1 ) {
   okToRead = 0;
   return 0 ;
  } else {
   okToRead = 1;
   return 1;
  } 
}

// read ?? 
int  UltrasonicFilter::read(){
  if (okToRead == 1){
    okToRead = 0;
  //  if (Serial) Serial.println(returnValue);
    return returnValue;
  } else {
  //  if(Serial)Serial.println('[ERR] - call available() before read!');
    return -1;
  }
}


// update -- uses default timing of 500mS 
//or 
// override with own value via .setActiveInterval(x);
int  UltrasonicFilter::update(){
  return update(activeInterval);
}

/* 

this filter takes advantage of two timeouts::

  the first :
    -- is a hardware timeout that limits readings to every 60mS (main timer in function below)
    -- returns last valid reading

  the second:
    -- is a software max distance timeout.  the sensor returns this value when it throws an error
    (all the noise these sensors are famous for)  
    -- returns last valid reading
 
  -- NOTE -- if the sensor has no detection the time out reading is consider 


  -- if neither timeout occurs, the function returns an updated reading

*/
int  UltrasonicFilter::update(unsigned long activeTimeout) {

  now = millis();
  //returnValue = previousReading; // if timer doesn't fire, use old reading
 
  // every 60 mS get a new reading ... and see if an object is present
  if (now - senseStartTime > senseInterval) {

    raw = sensor->read(); // get reading from sensor 
    distanceReading = raw; // redundant? 
  if (distanceReading > maximumDistance) distanceReading = maximumDistance;
      // store limit data when nothing is happening -- result = limit as baseline
    if (distanceReading == maximumDistance)  {
        
        switch (SONAR_STATE) {

          case SONAR_STANDBY: // we have not seen anything recently, so software timout
            DATA = INCLUDE; 
            currentReading = distanceReading; 
            break;
        
          case SONAR_ACTIVE: // error :: have a recent object -- software timeout error
            DATA = EXCLUDE;  // this is an error spike
           // currentReading = previousReading; // should = last good reading 
            break;
        }

    } 
    
    if ( distanceReading != maximumDistance) { // NOT maximumDistance :: something in range
      currentReading = distanceReading;  // store for return
      // set state to active, start active reading timer 
      SONAR_STATE = SONAR_ACTIVE; 
      activeStart = now; // active timer start
      DATA = INCLUDE;
    } 

    // // store limit data when nothing is happening -- result = limit as baseline
    // if (( distanceReading == maximumDistance) and (SONAR_STATE == SONAR_STANDBY)){
    //   DATA = INCLUDE; 
    //   //new
    //   currentReading = distanceReading; // raw 
    // } 

    // // error spike -- timeout while reading
    // if ((distanceReading == maximumDistance) and (SONAR_STATE == SONAR_ACTIVE)){
    //     // this is an error spike
    //     DATA = EXCLUDE;
    //   // currentReading = previousReading; // should = last good reading - try without this ... 
    // }

    
    // // if the reading is not a software time out
    // if (distanceReading !=  maximumDistance) {  // this if..then removes timeout noise 

    //  // previousReading = distanceReading; // store for errors
    //   currentReading = distanceReading;  // store for return
    //   SONAR_STATE = SONAR_ACTIVE;
    //   activeStart = now;
    //   DATA = INCLUDE;
    // } 
    
    if (DATA == EXCLUDE){
      currentReading = previousReading;
    } else {
      ema = emaProportion * distanceReading + (1.0 - emaProportion) * ema;
      previousReading = distanceReading;
      DATA = EXCLUDE;
    }

        // need an activate keep alive timer before timeout and go back to waiting
    if ((now - activeStart) > activeTimeout) { // checks every loop -- default 1000mS (1Sec)
      SONAR_STATE = SONAR_STANDBY; // have not seen anything for a while, enter STADNBY mode
    }
    
    senseStartTime += senseInterval;  // reset 60mS timer for readings
    
    return currentReading; // we get to here we have a new good reading return it  

    
    // check to see if sensed objects have left range 



  }// end senseInterval timer 

  // everything below here fires EVERY call ....



 // timer says not yet, send back a -1 (ok for now) - try this 
 return -1; 

 //return previousReading; // return the last good value ... 

}

int UltrasonicFilter::getRaw() {
  return raw;
}

int UltrasonicFilter::getState(){
  return SONAR_STATE;
}

int UltrasonicFilter::getDistance(){
   return currentReading; // this repeats good readings for filtering 
}

int UltrasonicFilter::getLastReading(){
  return distanceReading; // this can return errors -- for debug only 
}

int UltrasonicFilter::getPreviousReading(){
  return previousReading; // this can return errors -- for debug only 
}

int UltrasonicFilter::getEma(){
  return ema;
}


// returns 1 if something detected, 0 if nothing detected. 
int UltrasonicFilter::getPresence(){ // slightly more human version of above 
  return SONAR_STATE; 
}

void UltrasonicFilter::setActiveInterval(unsigned long activeInt){
   activeInterval = activeInt;
}

unsigned long UltrasonicFilter::getActiveInterval() {
  return activeInterval;
}

void UltrasonicFilter::setEmaProportion(float f){ 
   emaProportion = f;
}

float UltrasonicFilter::getEmaProportion(){ 
   return emaProportion;
}

int UltrasonicFilter::getMaximumDistance(){
  return maximumDistance;
}
void UltrasonicFilter::setMaximumDistance(int md){

   maximumDistance = md;

   switch (md) {
    case 178:
          sensor -> setTimeout(10000UL);
    break;

    case 196:
          sensor -> setTimeout(11000UL);  
    break;

    case 214:
          sensor -> setTimeout(12000UL);
    break;

    case 232:
          sensor -> setTimeout(13000UL);
    break;

    case 250:
          sensor -> setTimeout(14000UL);
    break;

    case 268:
          sensor -> setTimeout(15000UL);
    break;

    case 285:
          sensor -> setTimeout(16000UL);
    break;

    case 303:
         sensor -> setTimeout(17000UL);   
    break;

    default:
        if(Serial){Serial.println("Ultrasonic Library error :: max distance is invalid -- choose from list");}
    break;

   } // end switch 
  
}



