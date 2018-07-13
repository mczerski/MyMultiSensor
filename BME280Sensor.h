#ifndef BME280Sensor_h
#define BME280Sensor_h

#include <MySensorsToolkit/Sensor/SensorBase.h>
#include <MySensorsToolkit/Sensor/SensorValue.h>
#include <BME280I2C.h>

namespace mys_toolkit {

class BME280Sensor: public SensorBase
{
  BME280I2C bmeSensor_;
  bool initialised_ = false;
  SensorValue<float> humidity_;
  SensorValue<float> temperature_;
  bool begin_() override;
  unsigned long update_() override;
public:
  BME280Sensor(uint8_t humSensorId, uint8_t tempSensorId, float humTreshold = 0, float tempTreshold = 0);
};

} //mys_toolkit

#endif //BME280Sensor_h
