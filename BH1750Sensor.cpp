#include "BH1750Sensor.h"

namespace mys_toolkit {

bool BH1750Sensor::begin_()
{
  auto sucess = lightSensor_.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  #ifdef MYS_TOOLKIT_DEBUG
  if(sucess){
    Serial.println("Found BH1750 sensor");
    logMsg("Found BH1750 sensor");
  }
  else {
    Serial.println("Could not find BH1750 sensor!");
    logMsg("Could not find BH1750 sensor!");
  }
  #endif
  return sucess;
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
