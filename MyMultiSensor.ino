// Enable debug prints
//#define MY_DEBUG

#define DUST_SENSOR
#define SKETCH_NAME "Multisensor"
#define SKETCH_MAJOR_VER "2"
#define SKETCH_MINOR_VER "5"

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
#define SKETCH_SUBNAME "LargeBathroom"
#define USE_BME280
#define USE_BH1750
#define MULTISENSOR
#endif
#ifdef SMALL_BATHROOM
#define MY_NODE_ID 21
#define SKETCH_SUBNAME "SmallBathroom"
#define USE_BME280
#define USE_BH1750
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN 3
#endif
#ifdef CORRIDOR_MOTION
#define MY_NODE_ID 26
#define SKETCH_SUBNAME "Corridor"
#define MULTISENSOR
#endif
#ifdef LARGE_BEDROOM_MOTION
#define MY_NODE_ID 27
#define SKETCH_SUBNAME "LargeBedroom"
#define USE_BME280
#define USE_BH1750
#define MULTISENSOR
#endif
#ifdef SMALL_BEDROOM_MOTION
#define MY_NODE_ID 28
#define SKETCH_SUBNAME "SmallBedroom"
#define USE_BME280
#define USE_BH1750
#define MULTISENSOR
#endif
#ifdef KITCHEN_MOTION
#define MY_NODE_ID 29
#define SKETCH_SUBNAME "Kitchen"
#define USE_BH1750
#define MULTISENSOR
#endif

#ifdef BALCONY_MOTION
#define MY_NODE_ID 30
#define SKETCH_SUBNAME "Balcony"
#define USE_MOTION
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#endif

#ifdef FRIDGE
#define MY_NODE_ID 4
#define SKETCH_SUBNAME "Fridge"
#endif

#ifdef SMALL_BEDROOM_BUTTON1
#define MY_NODE_ID 31
#undef SKETCH_NAME
#define SKETCH_NAME "Button"
#define SKETCH_SUBNAME "SmallBedroom"
#endif

#ifdef SMALL_BEDROOM_BUTTON2
#define MY_NODE_ID 32
#undef SKETCH_NAME
#define SKETCH_NAME "Button2"
#define SKETCH_SUBNAME "SmallBedroom"
#endif

#ifdef DUST_SENSOR
#define MY_NODE_ID 33
#define MY_DISABLED_SERIAL
#undef SKETCH_NAME
#define SKETCH_NAME "Dust"
#define SKETCH_SUBNAME "Sensor"
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
#include <MySensorsToolkit/Sensor/ButtonSensor.h>
#include "DustSensor.h"

using namespace mys_toolkit;

#ifdef MULTISENSOR
#define USE_MOTION
#define INITIAL_BOOST false
#define ALWAYS_BOOST true
#define LI_ION_BATTERY true
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#define LED_PIN -1
#endif

#ifdef FRIDGE
#define USE_DS18B20
#define INITIAL_BOOST true
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN 3
#define LED_PIN -1
#endif

#ifdef SMALL_BEDROOM_BUTTON1
#define USE_BUTTON
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#define LED_PIN -1
#endif

#ifdef SMALL_BEDROOM_BUTTON2
#define USE_BUTTON
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN INTERRUPT_NOT_DEFINED
#define LED_PIN -1
#endif

#ifdef DUST_SENSOR
#define CLOCK_PRESCALER CLOCK_PRESCALER_1
DustSensor ds(Serial, 1, 2, 3);
#define USE_BUTTON
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY true
#define BUTTON_PIN 3
#define LED_PIN A1
#endif

#ifdef TEST
//#define USE_BME280
//#define USE_BH1750
//#define USE_MOTION
//#define USE_DHT
//#define USE_DS18B20
#define INITIAL_BOOST false
#define ALWAYS_BOOST false
#define LI_ION_BATTERY false
#define BUTTON_PIN 3//INTERRUPT_NOT_DEFINED
#endif

#define BATTERY_SENSE_PIN A7
#define POWER_BOOST_PIN A2

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
#ifdef USE_BUTTON
ButtonSensor button(8, 3);
#endif

void presentation()
{
  // Send the sketch version information to the gateway
  sendSketchInfo(SKETCH_NAME "-" SKETCH_SUBNAME, SKETCH_MAJOR_VER "." SKETCH_MINOR_VER);

  SensorBase::present();
}

void setup()
{
  Wire.begin();
#ifdef DUST_SENSOR
  Serial.begin(9600);
#endif
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
