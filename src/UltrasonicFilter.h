#ifndef UltrasonicFilter_h
#define UltrasonicFilter_h


#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <Ultrasonic.h>


//=============================================================================
class UltrasonicFilter {

public:
	// constructor
  UltrasonicFilter(Ultrasonic* _sensor);

  int maximumDistance=303;
  int SONAR_STATE = 0;

	int returnValue;

	void begin();
	void begin(int  maximumDistance = 303);
  int  available();
	int  read(); 
	int  getRaw();
	int  getState();
	int  getDistance();
	int  getLastReading();
	int  getPreviousReading();
	int  getEma();
	int  getPresence();

	void setMaximumDistance(int);
	int  getMaximumDistance();

	unsigned long activeInterval = 500;  // active window after last good reading ... keep as small as possible was 1000
	void setActiveInterval(unsigned long);
	unsigned long getActiveInterval();

  int ema;
	float emaProportion = 0.1; // 1.0=no filter
	void setEmaProportion(float); // 1.0 default
	float getEmaProportion();

		int  update();
	
	
private:

	Ultrasonic* sensor; 
	

  int  update(unsigned long activeTimeout);

	int okToRead;
	int raw;
	int distanceReading;
	int currentReading;
	int previousReading;

	// make a timer for how often to read (60mS = 16 reads / second)
	unsigned long now;
	unsigned long senseInterval = 80;  // do NOT change, max rate -- there is a hardware limit
	unsigned long senseStartTime;

	// timer to keep active state alive despite errors from sensor
	unsigned long activeStart;
	
	// some constants to track what is happening
	// overall states ::  active == see someone, STANDBY - nothing detected for a while
	#define SONAR_ACTIVE  1
  #define SONAR_STANDBY 0


	// state variables to track when to add data to filters and when to exclude
	const int EXCLUDE = 0;
	const int INCLUDE = 1;
  int DATA = EXCLUDE;

};

#endif
