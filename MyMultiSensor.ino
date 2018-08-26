// Enable debug prints
//#define MY_DEBUG

#define LARGE_BATHROOM_MOTION
#define SKETCH_NAME "Multisensor"
#define SKETCH_MAJOR_VER "2"
#define SKETCH_MINOR_VER "0"

// Enable and select radio type attached 
#define MY_RADIO_RFM69
#define MY_RFM69_CS_PIN 10
#define MY_RFM69_IRQ_PIN 2
#define MY_RFM69_NEW_DRIVER
#define MY_RFM69_ATC_MODE_DISABLED
#define MY_RFM69_TX_POWER_DBM 0
#define MY_SIGNAL_REPORT_ENABLED

#ifdef LARGE_BATHROOM_MOTION
#define MY_NODE_ID 3
#define USE_BME280
#define MULTISENSOR
#endif
#ifdef CORRIDOR_MOTION
#define MY_NODE_ID 26
#define USE_BME280
#define MULTISENSOR
#endif
#ifdef LARGE_BEDROOM_MOTION
#define MY_NODE_ID 27
#define USE_BME280
#define MULTISENSOR
#endif
#ifdef SMALL_BEDROOM_MOTION
#define MY_NODE_ID 28
#define USE_BME280
#define MULTISENSOR
#endif
#ifdef KITCHEN_MOTION
#define MY_NODE_ID 29
#define MULTISENSOR
#endif

#ifdef BALCONY_MOTION
#define MY_NODE_ID 30
#define USE_MOTION
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#endif

#ifdef FRIDGE
#define MY_NODE_ID 4
#endif

#ifdef TEST
#define MY_NODE_ID 25
#endif

#include <Wire.h>
#include <MySensors.h>
#include <MySensorsToolkit.h>

#include "BME280Sensor.h"
#include "BH1750Sensor.h"
#include <MySensorsToolkit/Sensor/MotionSensor.h>
#include "DHTSensor.h"
#include "DS18B20Sensor.h"

#ifdef MULTISENSOR
#define USE_BH1750
#define USE_MOTION
#define INITIAL_BOOST false
#define ALWAYS_BOOST true
#define LI_ION_BATTERY true
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#endif

#ifdef FRIDGE
#define USE_DS18B20
#define INITIAL_BOOST true
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN 3
#endif

#ifdef TEST
//#define USE_BME280
//#define USE_BH1750
#define USE_MOTION
//#define USE_DHT
//#define USE_DS18B20
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#endif

#define LED_PIN A1
#define BATTERY_SENSE_PIN A7
#define POWER_BOOST_PIN A2

using namespace mys_toolkit;

#ifdef USE_BME280
BME280Sensor bme280(0, 1, 3.0, 0.5);
#endif
#ifdef USE_BH1750
BH1750Sensor bh1750(2, 20);
#endif
#ifdef USE_MOTION
MotionSensor motion(4, 3);
#endif
#ifdef USE_DHT
DHTSensor dht(5, 6, 6, 3.0, 0.5);
#endif
#ifdef USE_DS18B20
DS18B20Sensor ds18b20(7, A5, 0.5);
#endif

void presentation()
{
  // Send the sketch version information to the gateway
  sendSketchInfo(SKETCH_NAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

  SensorBase::present();
}

void setup()
{
  Wire.begin();
  SensorBase::begin(BATTERY_SENSE_PIN, LI_ION_BATTERY, POWER_BOOST_PIN,
                    INITIAL_BOOST, ALWAYS_BOOST, BUTTON_PIN, LED_PIN);
}

void loop()
{
  SensorBase::update();
}

void receive(const MyMessage &message) {
  SensorBase::receive(message);
}


