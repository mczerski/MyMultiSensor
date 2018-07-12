#include "DS18B20Sensor.h"

namespace mys_toolkit {

void DS18B20Sensor::power(uint8_t value)
{
  if (powerPin_ != uint8_t(-1))
    digitalWrite(powerPin_, value);
}

void DS18B20Sensor::begin_()
{
  if (powerPin_ != uint8_t(-1))
    pinMode(powerPin_, OUTPUT);
  power(HIGH);
  sensor_.begin();
  sensor_.setWaitForConversion(false);
#ifdef MYS_TOOLKIT_DEBUG
  auto devCount = sensor_.getDeviceCount();
  Serial.print("DS18B20: Found ");
  Serial.print(devCount);
  Serial.println(" devices");
  auto isParasite = sensor_.isParasitePowerMode();
  Serial.print("DS18B20: parasite: ");
  Serial.println(isParasite);
#endif
}

unsigned long DS18B20Sensor::preUpdate_()
{
  power(HIGH);
  sensor_.requestTemperatures();
  return sensor_.millisToWaitForConversion(sensor_.getResolution());
}

unsigned long DS18B20Sensor::update_()
{
  auto temp = sensor_.getTempCByIndex(0);
#ifdef MYS_TOOLKIT_DEBUG
  auto devCount = sensor_.getDeviceCount();
  Serial.print("DS18B20: temp: ");
  Serial.println(temp);
#endif
  temperature_.update(temp);
  power(LOW);
  return SLEEP_TIME;
}

DS18B20Sensor::DS18B20Sensor(uint8_t tempSensorId, uint8_t dataPin, float tempTreshold, uint8_t powerPin)
  : oneWire_(dataPin),
    sensor_(&oneWire_),
    powerPin_(powerPin),
    temperature_(tempSensorId, V_TEMP, S_TEMP, tempTreshold)
{
}

} //mys_toolkit
