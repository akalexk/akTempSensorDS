/*
 * akTempSensorDS.h
 * Version 0.2.1 - Dtum 17.02.2019
 * <alexander@koehler-kassel.de>
 *
*/
#ifndef akTempSensorDS_h
#define akTempSensorDS_h

#include "Arduino.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class akTempSensorDS
{
  public:
    akTempSensorDS(int pin = 15, bool autoSearch = true): _oneWire(new OneWire(pin))
    {
      _sensors = DallasTemperature(_oneWire);
      _pin = pin;
      _autoSearch = autoSearch;
    }
    void begin();
    void begin(char mqttBasis[]);
    void dsSensor(uint8_t deviceIndex, char msg[]);
    uint8_t TempMessen(void);
    uint8_t findDevices(int pin);
    uint8_t searchDevices(int pinMin = 0, int pinMax = 20);
    void setPin(int pin);
    char* _commands[10];
    uint8_t _cmdCount;
    char _cmdCountStr[250];
    char _mqttBasis[250];
    bool _autoSearch;
    int _pin;

  private:
    void array_to_string(byte array[], unsigned int len, char buffer[]);
    DallasTemperature _sensors;
    OneWire* _oneWire;
    DeviceAddress _DSdeviceAddress;
    char DSdeviceAddressStr[20];
    char DSmsg[250];
};
#endif
