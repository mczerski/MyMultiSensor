#include "DS18B20Sensor.h"

#ifdef MYS_TOOLKIT_DEBUG
extern HardwareSerial MYS_TOOLKIT_SERIAL;
#endif

namespace mys_toolkit {

void DS18B20Sensor::power(uint8_t value)
{
  if (powerPin_ != uint8_t(-1))
    digitalWrite(powerPin_, value);
}

bool DS18B20Sensor::begin_()
{
  if (powerPin_ != uint8_t(-1))
    pinMode(powerPin_, OUTPUT);
  power(HIGH);
  sensor_.begin();
  sensor_.setWaitForConversion(false);
#ifdef MYS_TOOLKIT_DEBUG
  auto devCount = sensor_.getDeviceCount();
  MYS_TOOLKIT_SERIAL.print("DS18B20: Found ");
  MYS_TOOLKIT_SERIAL.print(devCount);
  MYS_TOOLKIT_SERIAL.println(" devices");
  auto isParasite = sensor_.isParasitePowerMode();
  MYS_TOOLKIT_SERIAL.print("DS18B20: parasite: ");
  MYS_TOOLKIT_SERIAL.println(isParasite);
#endif
  return true;
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
  MYS_TOOLKIT_SERIAL.print("DS18B20: temp: ");
  MYS_TOOLKIT_SERIAL.println(temp);
#endif
  power(LOW);
  if (temp == DEVICE_DISCONNECTED_C) {
    return 2000;
  }
  else {
    temperature_.update(temp);
    return SLEEP_TIME;
  }
}

DS18B20Sensor::DS18B20Sensor(uint8_t tempSensorId, uint8_t dataPin, float tempTreshold, uint8_t powerPin)
  : oneWire_(dataPin),
    sensor_(&oneWire_),
    powerPin_(powerPin),
    temperature_(tempSensorId, V_TEMP, S_TEMP, tempTreshold)
{
}

} //mys_toolkit
