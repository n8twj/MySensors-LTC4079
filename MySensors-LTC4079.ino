// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable and select radio type attached
#define MY_RADIO_NRF24
#define MY_RF24_CE_PIN 7
#define MY_RF24_CS_PIN 8

#define MY_NODE_ID 10  // STATIC Node Assignment

#include <MySensors.h>

#define ID_S_SOLAR 1
#define ID_S_BATTERY 2
#define ID_S_CHARGE 3

#define SendDelay 500

#define BATTERY_VOLTAGE_PIN A0
#define SOLAR_VOLTAGE_PIN A2
#define CHARGE_CURRENT_PIN A6
#define CHRG_PIN A7

float VccReference = 3.34; // actual measured output of LDO

unsigned long SLEEP_TIME = 60000;
unsigned long DEEP_SLEEP_TIME = 3600000;
                                  
MyMessage msg_S_BATTERY_VOLTAGE(ID_S_BATTERY,V_VOLTAGE);
MyMessage msg_S_BATTERY_CURRENT(ID_S_BATTERY,V_CURRENT);
MyMessage msg_S_SOLAR_VOLTAGE(ID_S_SOLAR,V_VOLTAGE);
MyMessage msg_S_SOLAR_CURRENT(ID_S_SOLAR,V_CURRENT);
MyMessage msg_S_CHARGE(ID_S_CHARGE,V_CUSTOM);


void setup() { 
  analogReference(DEFAULT);            // default external reference = 3.3v for Ceech board
}
void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("LCT4079 Sensor", "1.0");  wait(SendDelay);
  present(ID_S_BATTERY,S_MULTIMETER,"Battery"); wait(SendDelay);
  present(ID_S_SOLAR,S_MULTIMETER,"Solar"); wait(SendDelay);
  present(ID_S_CHARGE,S_CUSTOM,"Charge"); wait(SendDelay);
}

void loop() { 
 float batteryVoltage = ((float)analogRead(BATTERY_VOLTAGE_PIN) * VccReference / 1024 ) * 2; // measuring battery voltage
 float solarVoltage   = ((float)analogRead(SOLAR_VOLTAGE_PIN)   * VccReference /1024 ) * 2; // measuring battery voltage
 float chargeCurrent  = ((float)analogRead(CHARGE_CURRENT_PIN)  * batteryVoltage / 1024 ) / 2.5 * 1000; // battery charge current (mA)
 int charging = analogRead(CHRG_PIN);
 
#ifdef MY_DEBUG
    Serial.print("Battery Voltage: ");
    Serial.println(batteryVoltage);
    Serial.print("Solar Voltage: ");
    Serial.println(solarVoltage);
    Serial.print("Charge Current: " );
    Serial.println(chargeCurrent);
    Serial.print("Charge: " );
    Serial.println(charging);
    
#endif
  send(msg_S_SOLAR_VOLTAGE.set(solarVoltage,2)); wait(SendDelay);
  send(msg_S_SOLAR_CURRENT.set(chargeCurrent,2)); wait(SendDelay);
  send(msg_S_BATTERY_VOLTAGE.set(batteryVoltage,2)); wait(SendDelay);
  send(msg_S_CHARGE.set(charging));
  if (batteryVoltage < 3.55) { 
    smartSleep(DEEP_SLEEP_TIME);
  }  else {
    smartSleep(SLEEP_TIME);
  }
}
