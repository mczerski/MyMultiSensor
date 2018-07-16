#include "DHTSensor.h"

namespace mys_toolkit {

bool DHTSensor::begin_()
{
  dhtSensor_.setup(dataPin_);
  return true;
}

unsigned long DHTSensor::preUpdate_()
{
  return dhtSensor_.getMinimumSamplingPeriod();
}

unsigned long DHTSensor::update_()
{
  dhtSensor_.readSensor(true);
  humidity_.update(dhtSensor_.getHumidity());
  temperature_.update(dhtSensor_.getTemperature());
  return SLEEP_TIME;
}

DHTSensor::DHTSensor(uint8_t humSensorId, uint8_t tempSensorId, uint8_t dataPin, float humTreshold, float tempTreshold)
  : dataPin_(dataPin),
    humidity_(humSensorId, V_HUM, S_HUM, humTreshold),
    temperature_(tempSensorId, V_TEMP, S_TEMP, tempTreshold)
{
}

} //mys_toolkit
