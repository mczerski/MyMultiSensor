#include "BH1750Sensor.h"

namespace mys_toolkit {

void BH1750Sensor::begin_()
{
  lightSensor_.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
}

unsigned long BH1750Sensor::preUpdate_()
{
  lightSensor_.configure(BH1750::ONE_TIME_HIGH_RES_MODE);
  return 120;
}

unsigned long BH1750Sensor::update_()
{
  luminance_.update(lightSensor_.readLightLevel());
  return SLEEP_TIME;
}

BH1750Sensor::BH1750Sensor(uint8_t sensorId, uint16_t treshold)
  : luminance_(sensorId, V_LIGHT_LEVEL, S_LIGHT_LEVEL, treshold)
{  
}

} //mys_toolkit
