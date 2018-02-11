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

//#define MY_RADIO_RFM69
#define MY_RFM69_CS_PIN 10
#define MY_RFM69_IRQ_PIN 3
#define MY_RFM69_NEW_DRIVER
#define MY_RFM69_ATC_MODE_DISABLED
#define MY_RFM69_TX_POWER_DBM 0

#define MY_NODE_ID 4

#define BUTTON_PIN 2
#define MY_LED A1
#include "MyMySensors/BME280Sensor.h"
#include "MyMySensors/BH1750Sensor.h"
//#include "MyMySensors/MotionSensor.h"

using namespace mymysensors;

BME280Sensor bme280(0, 1, 3.0, 0.5);
BH1750Sensor bh1750(2, 20);
//MotionSensor motion(4, 3);

void presentation()
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Multisensor", "1.7");

  MyMySensor::present();
}

void setup()
{
  Serial.begin(115200);
  MyMySensor::begin(A7, false);
}

void loop()
{
  MyMySensor::update();
}
