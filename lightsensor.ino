// This #include statement was automatically added by the Particle IDE.
#include <BH1750.h>

#define LIGHT_THRESHOLD 500.0 //lux
#define DELAY 1000.0 //milliseconds
bool isLight;
float lux;
#define LIGHT_MESSAGE "Sunlight detected" //message when we've switched to light mode
#define DARK_MESSAGE "No sunlight detected" //message when we've switched to dark mode


BH1750 sensor(0x23, Wire);

void setup()
{
  sensor.begin();

  sensor.set_sensor_mode(BH1750::forced_mode_high_res2);

  Serial.begin();
  init();
}

void loop()
{
    getLight();
    detectChange();
}

//first time reading, gets the light level and sets initial state
void init()
{
    getLight();
    
    //if light is beyond threshold, set light mode to light
    if (lux >= LIGHT_THRESHOLD) { 
        setLight();
    }
  
    //if light is below threshold, set light mode to dark
    else {
        setDark();
    }
    
    //can't detect more than one change per second, so set delay before checking again
    delay(DELAY);
}

void setLight() {
    isLight = true;
    Particle.publish("lightSwitch", String(LIGHT_MESSAGE), PRIVATE);    
}


void setDark() {
    isLight = false; 
    Particle.publish("lightSwitch", String(DARK_MESSAGE), PRIVATE);    
}

//receives a reading from the light sensor
void getLight() {
    sensor.make_forced_measurement();
    lux = sensor.get_light_level();
}

//detects if the light level has crossed the threshold and sends an alert when it does
void detectChange() {
    //if we started in the dark state and now go over the threshold, then we've gotten sunlight and need to send alert
    if ((lux >= LIGHT_THRESHOLD) && (!isLight)) {
        setLight();
    }
    
    //if we started in the light state and now go below the threshold, then we've lost sunlight and need to send alert
    else if ((lux < LIGHT_THRESHOLD) && (isLight)) {
        setDark();
    }
    
    //can't detect more than one change per second, so set delay before checking again
    delay(DELAY);
}