#include "DustSensor.h"

namespace mys_toolkit {

bool DustSensor::begin_()
{
  pmsSensor_.passiveMode();
  return true;
}

unsigned long DustSensor::preUpdate_()
{
  pmsSensor_.wakeUp();
  pmsSensor_.passiveMode();
  return 40000;
}

unsigned long DustSensor::update_()
{
  pmsSensor_.requestRead();
  if (pmsSensor_.readUntil(data_))
  {
    pm10_.update(data_.PM_AE_UG_1_0);
    pm25_.update(data_.PM_AE_UG_2_5);
    pm100_.update(data_.PM_AE_UG_10_0);
  }
  delay(100);
  pmsSensor_.sleep();
  pmsSensor_.sleep();
  return 600000;
}

DustSensor::DustSensor(Stream &serial, uint8_t pm10SensorId, uint8_t pm25SensorId, uint8_t pm100SensorId)
  : pmsSensor_(serial),
    pm10_(pm10SensorId, V_UNIT_PREFIX, S_DUST, 0),
    pm25_(pm25SensorId, V_UNIT_PREFIX, S_DUST, 0),
    pm100_(pm100SensorId, V_UNIT_PREFIX, S_DUST, 0)
{
}

} //mys_toolkit
