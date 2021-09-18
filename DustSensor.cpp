#include "DustSensor.h"

#ifdef MYS_TOOLKIT_DEBUG
extern HardwareSerial MYS_TOOLKIT_SERIAL;
#endif

namespace mys_toolkit {

bool DustSensor::begin_()
{
  serial_.begin(PMS::BAUD_RATE);
  MYS_TOOLKIT_SERIAL.println("Setting passive Mode");
  pmsSensor_.passiveMode();
  MYS_TOOLKIT_SERIAL.println("OK");
  return true;
}

unsigned long DustSensor::preUpdate_()
{
  serial_.begin(PMS::BAUD_RATE);
  MYS_TOOLKIT_SERIAL.println("Waking up");
  pmsSensor_.wakeUp();
  pmsSensor_.passiveMode();
  MYS_TOOLKIT_SERIAL.println("OK");
  return 40000;
}

unsigned long DustSensor::update_()
{
  MYS_TOOLKIT_SERIAL.println("Reguest read");
  pmsSensor_.requestRead();
  MYS_TOOLKIT_SERIAL.println("OK");
  if (pmsSensor_.readUntil(data_))
  {
    MYS_TOOLKIT_SERIAL.print("pm10: ");
    MYS_TOOLKIT_SERIAL.println(data_.PM_AE_UG_1_0);
    MYS_TOOLKIT_SERIAL.print("pm25: ");
    MYS_TOOLKIT_SERIAL.println(data_.PM_AE_UG_2_5);
    MYS_TOOLKIT_SERIAL.print("pm100: ");
    MYS_TOOLKIT_SERIAL.println(data_.PM_AE_UG_10_0);
    pm10_.update(data_.PM_AE_UG_1_0);
    pm25_.update(data_.PM_AE_UG_2_5);
    pm100_.update(data_.PM_AE_UG_10_0);
  }
  delay(100);
  pmsSensor_.sleep();
  serial_.flush();
  pinMode(tx_pin_, INPUT);
  MYS_TOOLKIT_SERIAL.println("Done");
  MYS_TOOLKIT_SERIAL.flush();
  return 600000;
}

DustSensor::DustSensor(uint8_t pm10SensorId, uint8_t pm25SensorId, uint8_t pm100SensorId, uint8_t rx_pin, uint8_t tx_pin)
  : serial_(rx_pin, tx_pin),
    pmsSensor_(serial_),
    tx_pin_(tx_pin),
    pm10_(pm10SensorId, V_UNIT_PREFIX, S_DUST, 0),
    pm25_(pm25SensorId, V_UNIT_PREFIX, S_DUST, 0),
    pm100_(pm100SensorId, V_UNIT_PREFIX, S_DUST, 0)
{
}

} //mys_toolkit
