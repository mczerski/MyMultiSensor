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
#define LARGE_BATHROOM

// Enable and select radio type attached 
//#define MY_RADIO_NRF24
#define MY_RF24_CE_PIN 9
#define MY_RF24_CS_PIN 10
#define MY_RF24_CHANNEL 100
#define MY_RF24_PA_LEVEL RF24_PA_MAX

#define MY_RADIO_RFM69
#define MY_RFM69_CS_PIN 10
#define MY_RFM69_IRQ_PIN 3
#define MY_RFM69_NEW_DRIVER
#define MY_RFM69_ATC_MODE_DISABLED
#define MY_RFM69_TX_POWER_DBM 0

#ifdef LARGE_BATHROOM
#define MY_NODE_ID 3
#define USE_BME280
#define USE_BH1750
#define USE_MOTION
#define INITIAL_BOOST false
#define ALWAYS_BOOST true
#define LI_ION_BATTERY true
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#endif

#ifdef FRIDGE
#define MY_NODE_ID 4
#define USE_DS18B20
#define INITIAL_BOOST true
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN 2
#endif

#ifdef TEMP_TEST
#define MY_NODE_ID 25
#define USE_BME280
#define USE_DHT
#define USE_DS18B20
#define INITIAL_BOOST true
#define ALWAYS_BOOST true
#define LI_ION_BATTERY false
#define BUTTON_PIN 2
#endif

#define MY_LED A1
#include "MyMySensors/BME280Sensor.h"
#include "MyMySensors/BH1750Sensor.h"
#include "MyMySensors/MotionSensor.h"
#include "MyMySensors/DHTSensor.h"
#include "MyMySensors/DS18B20Sensor.h"

using namespace mymysensors;

#ifdef USE_BME280
BME280Sensor bme280(0, 1, 3.0, 0.5);
#endif
#ifdef USE_BH1750
BH1750Sensor bh1750(2, 20);
#endif
#ifdef USE_MOTION
MotionSensor motion(4, 2);
#endif
#ifdef USE_DHT
DHTSensor dht(5, 6, 6, 3.0, 0.5);
#endif
#ifdef USE_DS18B20
DS18B20Sensor ds18b20(7, A5, 0.5, A4);
#endif
#ifdef TEMP_TEST
MyParameter<uint32_t> var1(0, V_VAR1, S_CUSTOM, 0);
MyParameter<uint16_t> var2(1, V_VAR2, S_CUSTOM, 0);
MyParameter<uint8_t> var3(2, V_VAR3, S_CUSTOM, 0);
bool ledValue() {
  return var1.get() == 0xdeadbeef or var2.get() == 0xaa55 or var3.get() == 42;
}
#endif

void presentation()
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Multisensor", "1.8");

  MyMySensor::present();
}

void setup()
{
  Serial.begin(115200);
  MyMySensor::begin(A7, LI_ION_BATTERY, A2, INITIAL_BOOST, ALWAYS_BOOST, BUTTON_PIN);
  #ifdef TEMP_TEST
  pinMode(A0, OUTPUT);
  digitalWrite(A0, ledValue());
  #endif
}

void loop()
{
  MyMySensor::update();
}

void receive(const MyMessage &message) {
  MyMySensor::receive(message);
  #ifdef TEMP_TEST
  digitalWrite(A0, ledValue());
  #endif
}

