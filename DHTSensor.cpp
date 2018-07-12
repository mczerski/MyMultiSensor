#include "DHTSensor.h"

namespace mys_toolkit {

void DHTSensor::begin_()
{
  dhtSensor_.setup(dataPin_);
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

DHTSensor::DHTSensor(uint8_t humSensorId, uint8_t tempSensorId, uint8_t dataPin, float humTreshold = 0, float tempTreshold = 0)
  : dataPin_(dataPin),
    humidity_(humSensorId, V_HUM, S_HUM, humTreshold),
    temperature_(tempSensorId, V_TEMP, S_TEMP, tempTreshold)
{
}

} //mys_toolkit
