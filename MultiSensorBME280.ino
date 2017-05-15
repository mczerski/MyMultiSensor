/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0: Henrik EKblad
 * Version 1.1 - 2016-07-20: Converted to MySensors v2.0 and added various improvements - Torben Woltjen (mozzbozz)
 * 
 * DESCRIPTION
 * This sketch provides an example of how to implement a humidity/temperature
 * sensor using a DHT11/DHT-22.
 *  
 * For more information, please visit:
 * http://www.mysensors.org/build/humidity
 * 
 */

// Enable debug prints
//#define MY_DEBUG

// Enable and select radio type attached 
#define MY_RADIO_NRF24
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 8
#define MY_RF24_CHANNEL 100
//#define MY_RADIO_RFM69
//#define MY_RS485

#define MY_NODE_ID 5

#include <SPI.h>
#include <MySensors.h>
#include <BH1750.h>
#include <BME280I2C.h>

// Set this to the pin you connected the DHT's data pin to
#define DHT_DATA_PIN 6
#define DIGITAL_INPUT_SENSOR 3
#define BUTTON_PIN 2
#define BATTERY_PIN A0
#define MY_LED A2

// Set this offset if the sensor has a permanent small offset to the real temperatures
#define SENSOR_TEMP_OFFSET 0

// Sleep time between sensor updates (in milliseconds)
static const uint64_t UPDATE_INTERVAL = 600000;

// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_LIGHT 2
#define CHILD_ID_VOLT 3
#define CHILD_ID_TRIPPED 4

float lastTemp = 0;
float lastHum = 0;
uint16_t lastlux = 0;
int lastVolt = 0;
boolean lastTripped = false;
uint8_t nNoUpdatesTemp = 0;
uint8_t nNoUpdatesHum = 0;
uint8_t nNoUpdatesLux = 0;
uint8_t nNoUpdatesVolt = 0;
bool metric = true;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgLux(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
MyMessage msgVolt(CHILD_ID_VOLT, V_VOLTAGE);
MyMessage msgTripped(CHILD_ID_TRIPPED, V_TRIPPED);
BH1750 lightSensor;
BME280I2C bmeSensor;

void presentation()  
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Multisensor", "1.2");
  
  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);
  present(CHILD_ID_LIGHT, S_LIGHT_LEVEL);
  present(CHILD_ID_VOLT, S_MULTIMETER);
  present(CHILD_ID_TRIPPED, S_MOTION);

  metric = getConfig().isMetric;
}

int convert2mV(int v) {
  int voltage_mV = v * 1.1 * 1.1 * 1000 * 10 / 1024; //includes 1/11 divider
  return voltage_mV;
}

int convertmV2Pcnt(int v) {
  if (v >= 4200)
    return 100;
  else if (v <= 2750)
    return 0;
  else
    return (v - 2750) / (42 - 27.5);
}

void setup()
{
  //setup led
  pinMode(MY_LED, OUTPUT);
  digitalWrite(MY_LED, LOW);
  analogReference(INTERNAL);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  pinMode(DIGITAL_INPUT_SENSOR, INPUT);

  lightSensor.begin();

  if(!bmeSensor.begin()){
    #ifdef MY_DEBUG
    Serial.println("Could not find BME280 sensor!");
    #endif
  }

}


void loop()      
{
  // Force reading sensor, so it works also after sleep()
  //sleep(200);

  // Get temperature from BME280 library
  float temperature = bmeSensor.temp(metric);
  if (isnan(temperature)) {
    #ifdef MY_DEBUG
    Serial.println("Failed reading temperature from BME280!");
    #endif
  } else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp = temperature;
    // Reset no updates counter
    nNoUpdatesTemp = 0;
    temperature += SENSOR_TEMP_OFFSET;
    send(msgTemp.set(temperature, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp++;
  }

  // Get humidity from BME280 library
  float humidity = bmeSensor.hum();
  if (isnan(humidity)) {
    #ifdef MY_DEBUG
    Serial.println("Failed reading humidity from DHT");
    #endif
  } else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum = humidity;
    // Reset no updates counter
    nNoUpdatesHum = 0;
    send(msgHum.set(humidity, 1));
    
    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
    #endif
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum++;
  }

  // Get luminance from BH1750 library
  uint16_t lux = lightSensor.readLightLevel();
  if (lux != lastlux || nNoUpdatesLux == FORCE_UPDATE_N_READS) {
    lastlux = lux;
    // Reset no updates counter
    nNoUpdatesLux = 0;
    send(msgLux.set(lux));
    
    #ifdef MY_DEBUG
    Serial.print("L: ");
    Serial.println(lux);
    #endif
  } else {
    // Increase no update counter if the luminance stayed the same
    nNoUpdatesLux++;
  }

  // Read digital motion value
  boolean tripped = digitalRead(DIGITAL_INPUT_SENSOR) == HIGH;
  digitalWrite(MY_LED, lastTripped != tripped ? LOW : HIGH);
  lastTripped = tripped;
  send(msgTripped.set(tripped?"1":"0"));

  // Get battery voltage
  int voltage = convert2mV(analogRead(BATTERY_PIN));
  int batteryPcnt = convertmV2Pcnt(voltage);
  if (batteryPcnt != lastVolt || nNoUpdatesVolt == FORCE_UPDATE_N_READS) {
    // Only send battery voltage if it changed since the last measurement or if we didn't send an update for n times
    lastVolt = batteryPcnt;
    // Reset no updates counter
    nNoUpdatesVolt = 0;
    send(msgVolt.set(voltage));
    sendBatteryLevel(batteryPcnt);

    #ifdef MY_DEBUG
    Serial.print("V: ");
    Serial.println(voltage);
    #endif
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesVolt++;
  }

  for (int i=0; i<20; i++) {
    if (isTransportOK())
      break;
    digitalWrite(MY_LED, LOW);
    wait(500); // transport is not operational, allow the transport layer to fix this
  }
  digitalWrite(MY_LED, HIGH);

  // Sleep for a while to save energy
  int wakeUpCause = sleep(digitalPinToInterrupt(BUTTON_PIN), FALLING, digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), CHANGE, UPDATE_INTERVAL);
  if (wakeUpCause == digitalPinToInterrupt(BUTTON_PIN)) {
    nNoUpdatesTemp = FORCE_UPDATE_N_READS;
    nNoUpdatesVolt = FORCE_UPDATE_N_READS;
    nNoUpdatesLux = FORCE_UPDATE_N_READS;
    nNoUpdatesHum = FORCE_UPDATE_N_READS;
    digitalWrite(MY_LED, LOW);
  }
  else if (wakeUpCause == digitalPinToInterrupt(DIGITAL_INPUT_SENSOR)) {
    digitalWrite(MY_LED, LOW);
  }
}
