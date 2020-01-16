#include "DustSensor.h"

namespace mys_toolkit {

bool DustSensor::begin_()
{
  serial_.begin(PMS::BAUD_RATE);
  Serial.println("Setting passive Mode");
  pmsSensor_.passiveMode();
  Serial.println("OK");
  return true;
}

unsigned long DustSensor::preUpdate_()
{
  serial_.begin(PMS::BAUD_RATE);
  Serial.println("Waking up");
  pmsSensor_.wakeUp();
  pmsSensor_.passiveMode();
  Serial.println("OK");
  return 40000;
}

unsigned long DustSensor::update_()
{
  Serial.println("Reguest read");
  pmsSensor_.requestRead();
  Serial.println("OK");
  if (pmsSensor_.readUntil(data_))
  {
    Serial.print("pm10: ");
    Serial.println(data_.PM_AE_UG_1_0);
    Serial.print("pm25: ");
    Serial.println(data_.PM_AE_UG_2_5);
    Serial.print("pm100: ");
    Serial.println(data_.PM_AE_UG_10_0);
    pm10_.update(data_.PM_AE_UG_1_0);
    pm25_.update(data_.PM_AE_UG_2_5);
    pm100_.update(data_.PM_AE_UG_10_0);
  }
  delay(100);
  pmsSensor_.sleep();
  serial_.flush();
  pinMode(tx_pin_, INPUT);
  Serial.println("Done");
  Serial.flush();
  return 600000;
}

DustSensor::DustSensor(uint8_t pm10SensorId, uint8_t pm25SensorId, uint8_t pm100SensorId, uint8_t rx_pin, uint8_t tx_pin)
  : serial_(rx_pin, tx_pin),
    pmsSensor_(serial_),
    pm10_(pm10SensorId, V_UNIT_PREFIX, S_DUST, 0),
    pm25_(pm25SensorId, V_UNIT_PREFIX, S_DUST, 0),
    pm100_(pm100SensorId, V_UNIT_PREFIX, S_DUST, 0),
    tx_pin_(tx_pin)
{
}

} //mys_toolkit
