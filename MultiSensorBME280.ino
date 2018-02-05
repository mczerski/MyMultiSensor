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
//#define MY_MY_DEBUG

// Enable and select radio type attached 
#define MY_RADIO_NRF24
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 10
#define MY_RF24_CHANNEL 100
#define MY_RF24_PA_LEVEL RF24_PA_MAX

#define MY_NODE_ID 4

#include "MyMySensors/MyMySensors.h"
#include <BH1750.h>
#include <BME280I2C.h>

#define DIGITAL_INPUT_SENSOR 3
#define BUTTON_PIN 2
#define MY_LED A1

using namespace mymysensors;

// Sleep time between sensor updates (in milliseconds)
static const uint64_t SLEEP_TIME = 600000;

bool metric = true;

MyValue<float> humidity(0, V_HUM, S_HUM, 3.0);
MyValue<float> temperature(1, V_TEMP, S_TEMP, 0.5);
MyValue<uint16_t> luminance(2, V_LIGHT_LEVEL, S_LIGHT_LEVEL, 20);
MyValue<uint16_t> tripped(4, V_TRIPPED, S_MOTION);

PowerManager& powerManager = PowerManager::initInstance(A2, true);

BH1750 lightSensor;
BME280I2C bmeSensor(1, 1, 1, 0);

void presentation()
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Multisensor", "1.7");

  humidity.presentValue();
  temperature.presentValue();
  luminance.presentValue();
  tripped.presentValue();
}

void setup()
{
  Serial.begin(115200);
  powerManager.setBatteryPin(A7, false);

  pinMode(MY_LED, OUTPUT);
  digitalWrite(MY_LED, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(DIGITAL_INPUT_SENSOR, INPUT_PULLUP);

  if(!bmeSensor.begin()){
    #ifdef MY_MY_DEBUG
    Serial.println("Could not find BME280 sensor!");
    #endif
  }
  lightSensor.begin(BH1750_ONE_TIME_HIGH_RES_MODE);
}

void loop()
{
  powerManager.turnBoosterOn();
  //wait for everything to setup (100ms for dc/dc converter)
  wait(100);

  lightSensor.configure(BH1750_ONE_TIME_HIGH_RES_MODE);
  bmeSensor.setMode(1);
  checkTransport();
  wait(120);
  digitalWrite(MY_LED, HIGH);

  float temp = bmeSensor.temp();
  float hum = bmeSensor.hum();
  uint16_t lux = lightSensor.readLightLevel();
  boolean trip = digitalRead(DIGITAL_INPUT_SENSOR);

  bool success = true;
  success &= temperature.updateValue(temp);
  success &= humidity.updateValue(hum);
  success &= luminance.updateValue(lux);
  success &= tripped.updateValue(trip, true);

  unsigned long sleepTimeout = getSleepTimeout(success, SLEEP_TIME);

  powerManager.turnBoosterOff();

  // Sleep for a while to save energy
  int wakeUpCause = sleep(digitalPinToInterrupt(BUTTON_PIN), FALLING, digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), CHANGE, sleepTimeout);
  if (wakeUpCause == digitalPinToInterrupt(BUTTON_PIN)) {
    digitalWrite(MY_LED, LOW);
    temperature.forceResend();
    humidity.forceResend();
    luminance.forceResend();
    tripped.forceResend();
    powerManager.forceResend();
    #ifdef MY_MY_DEBUG
    Serial.println("Wake up from button");
    #endif
  }
  else if (wakeUpCause == digitalPinToInterrupt(DIGITAL_INPUT_SENSOR)) {
    digitalWrite(MY_LED, LOW);
    #ifdef MY_MY_DEBUG
    Serial.println("Wake up from motion");
    #endif
  }
}
