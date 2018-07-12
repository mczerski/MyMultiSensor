#ifndef BH1750Sensor_h
#define BH1750Sensor_h

#include <MySensorsToolkit/Sensor/SensorBase.h>
#include <MySensorsToolkit/Sensor/SensorValue.h>
#include <BH1750.h>

namespace mys_toolkit {

class BH1750Sensor: public SensorBase
{
  BH1750 lightSensor_;
  SensorValue<uint16_t> luminance_;
  void begin_() override;
  unsigned long preUpdate_() override;
  unsigned long update_() override;
public:
  BH1750Sensor(uint8_t sensorId, uint16_t treshold = 0);
};

} //mys_toolkit

#endif //BH1750Sensor_h
