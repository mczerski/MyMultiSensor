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

#include <MyMySensors.h>
#include <BH1750.h>
#include <BME280I2C.h>

#define DIGITAL_INPUT_SENSOR 3
#define BUTTON_PIN 2
#define MY_LED A2

using namespace mymysensors;

// Sleep time between sensor updates (in milliseconds)
static const uint64_t SLEEP_TIME = 600000;

bool metric = true;

MyValue<float> humidity(0, V_HUM, S_HUM);
MyValue<float> temperature(1, V_TEMP, S_TEMP);
MyValue<uint16_t> luminance(2, V_LIGHT_LEVEL, S_LIGHT_LEVEL);
MyValue<uint16_t> tripped(4, V_TRIPPED, S_MOTION);

PowerManager& powerManager = PowerManager::initInstance(-1, false);

BH1750 lightSensor;
BME280I2C bmeSensor;

void presentation()
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Multisensor", "1.3");

  humidity.presentValue();
  temperature.presentValue();
  luminance.presentValue();
  tripped.presentValue();
}

void setup()
{
  powerManager.setBatteryPin(A0, true);

  pinMode(MY_LED, OUTPUT);
  digitalWrite(MY_LED, LOW);

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
  checkTransport();
  sleep(100);
  digitalWrite(MY_LED, HIGH);

  float temp = bmeSensor.temp();
  float hum = bmeSensor.hum();
  uint16_t lux = lightSensor.readLightLevel();
  boolean trip = digitalRead(DIGITAL_INPUT_SENSOR);

  bool success = temperature.updateValue(temp);
  success &= humidity.updateValue(hum);
  success &= luminance.updateValue(lux);
  success &= tripped.updateValue(trip, true);

  unsigned long sleepTimeout = getSleepTimeout(success, SLEEP_TIME);

  // Sleep for a while to save energy
  int wakeUpCause = sleep(digitalPinToInterrupt(BUTTON_PIN), FALLING, digitalPinToInterrupt(DIGITAL_INPUT_SENSOR), CHANGE, sleepTimeout);
  if (wakeUpCause == digitalPinToInterrupt(BUTTON_PIN)) {
    digitalWrite(MY_LED, LOW);
    temperature.forceResend();
    humidity.forceResend();
    luminance.forceResend();
    tripped.forceResend();
    powerManager.forceResend();
  }
  else if (wakeUpCause == digitalPinToInterrupt(DIGITAL_INPUT_SENSOR)) {
    digitalWrite(MY_LED, LOW);
  }
}
