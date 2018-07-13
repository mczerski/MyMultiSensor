#ifndef DS18B20Sensor_h
#define DS18B20Sensor_h

#include <MySensorsToolkit/Sensor/SensorBase.h>
#include <MySensorsToolkit/Sensor/SensorValue.h>
#include <DallasTemperature.h>
#include <OneWire.h>

namespace mys_toolkit {

class DS18B20Sensor: public SensorBase
{
  OneWire oneWire_;
  DallasTemperature sensor_;
  uint8_t powerPin_;
  SensorValue<float> temperature_;
  void power(uint8_t value);
  bool begin_() override;
  unsigned long preUpdate_() override;
  unsigned long update_() override;

public:
  DS18B20Sensor(uint8_t tempSensorId, uint8_t dataPin, float tempTreshold = 0, uint8_t powerPin = -1);
};

} //mys_toolkit

#endif //DS18B20Sensor_h
