#ifndef DustSensor_h
#define DustSensor_h

#include <MySensorsToolkit/Sensor/SensorBase.h>
#include <MySensorsToolkit/Sensor/SensorValue.h>
#include <PMS.h>

namespace mys_toolkit {

class DustSensor: public SensorBase
{
  PMS pmsSensor_;
  PMS::DATA data_;
  SensorValue<uint16_t> pm10_;
  SensorValue<uint16_t> pm25_;
  SensorValue<uint16_t> pm100_;
  bool begin_() override;
  unsigned long preUpdate_() override;
  unsigned long update_() override;

public:
  DustSensor(Stream& serial, uint8_t pm10SensorId, uint8_t pm25SensorId, uint8_t pm100SensorId);
};

} //mys_toolkit

#endif //DustSensor_h
