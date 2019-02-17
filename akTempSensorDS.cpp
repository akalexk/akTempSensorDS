/*
 * akTempSensorDS.cpp
 * Version 0.2.1 - Dtum 17.02.2019
 * <alexander@koehler-kassel.de>
 *
*/
//Include
#include "akTempSensorDS.h"
//#define DEBUG
/*
 * begin()
*/
void akTempSensorDS::begin()
{
#ifdef DEBUG
  Serial.println("begin()");
#endif
  // Variablen definieren
  _DSdeviceAddress;
  char DSdeviceAddressStr[20] = "";
  char DSmsg[250] = "";
  char _commands[10] = "";
  char _cmdCountStr[250] = "";
  char _mqttBasis[] = "";
  if (_autoSearch && _sensors.getDeviceCount() == 0) {setPin(searchDevices(1, 35));}
}
void akTempSensorDS::begin(char mqttBasis[])
{
#ifdef DEBUG
  Serial.println("begin(char mqttBasis[])");
#endif
  begin();
  //char _mqttBasis[strlen(mqttBasis)];
  strcpy(_mqttBasis, mqttBasis);
#ifdef DEBUG
  Serial.println(_mqttBasis);
#endif
}

/*
 * setPin()
*/
void akTempSensorDS::setPin(int pin){
  if (pin > 0) {
    _oneWire = new OneWire(pin);
    _sensors = DallasTemperature(_oneWire);
    _pin = pin;
  }
}

/*
 * array_to_string() (HEX to String)
*/
void akTempSensorDS::array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++) {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len*2] = '\0';
}

/*
 * dsSensor() - Einzelnene Sensor Messen
*/
void akTempSensorDS::dsSensor(uint8_t deviceIndex, char msg[]) {
#ifdef DEBUG
  Serial.println("dsSensor()");
#endif
  float tempC = _sensors.getTempCByIndex(deviceIndex);
  // Wenn der Check des OneWire lesen OK ist
  if(tempC != DEVICE_DISCONNECTED_C) {
    char DSdeviceAddressStr[20] = "";
    if (_sensors.getAddress(_DSdeviceAddress, deviceIndex)) array_to_string(_DSdeviceAddress, sizeof(_DSdeviceAddress), DSdeviceAddressStr);
    sprintf(msg, "%s/%s/%f", _mqttBasis, DSdeviceAddressStr, tempC);
  } else {
    strcpy(msg, _mqttBasis);
    strcat(msg, "Fehler: Der DS Sensor kann nicht ausgelesen werden.");
#ifdef DEBUG
    Serial.println(msg);
#endif
  }
}

/*
 * TempMessen() - Alle Sensoren erkennen und Messen
*/
uint8_t akTempSensorDS::TempMessen(void) {
  _sensors.begin();
  uint8_t AnzahlDsSensor = _sensors.getDeviceCount();
  sprintf(_cmdCountStr, "%s/count/%d", _mqttBasis, AnzahlDsSensor);
  _cmdCount = AnzahlDsSensor;
#ifdef DEBUG
  Serial.print("TempMessen()  ->  Requesting temperatures...");
#endif
  // lese alle Sensoren am Bus
  _sensors.requestTemperatures();
#ifdef DEBUG
  Serial.print("DONE ");
  Serial.println(AnzahlDsSensor);
#endif
  // alle Sensoren einzel auslesen
  for (uint8_t i = 0; i < AnzahlDsSensor; i++){
    dsSensor(i, DSmsg);
    _commands[i] = DSmsg;
#ifdef DEBUG
    Serial.println(DSmsg);
#endif
  }
  return AnzahlDsSensor;
}

/*
 * findDevices()
*/
uint8_t akTempSensorDS::findDevices(int pin)
{
  OneWire ow(pin);
  uint8_t address[8];
  uint8_t count = 0;
  if (ow.search(address))
  {
#ifdef DEBUG
    Serial.print("\nuint8_t pin");
    Serial.print(pin, DEC);
    Serial.println("[][8] = {");
#endif
    do {
      count++;
#ifdef DEBUG
      Serial.println("  {");
      for (uint8_t i = 0; i < 8; i++)
      {
        Serial.print("0x");
        if (address[i] < 0x10) Serial.print("0");
        Serial.print(address[i], HEX);
        if (i < 7) Serial.print(", ");
      }
      Serial.println("  },");
#endif
    } while (ow.search(address));
#ifdef DEBUG
    Serial.println("};");
    Serial.print("// nr devices found: ");
    Serial.println(count);
#endif
  }
  ow = 0;
  return count;
}

/*
 * searchDevices()
*/
uint8_t akTempSensorDS::searchDevices(int pinMin, int pinMax )
{
#ifdef DEBUG
  Serial.println("//\n// Start searchDevices() \n//");
#endif
  uint8_t find = 0;
  for (uint8_t pin = pinMin; pin <= pinMax; pin++)
  {
    if (pin > 6 && pin < 33 )
    {
      find = findDevices(pin);
#ifdef DEBUG
      Serial.print(pin);
      Serial.print(" = ");
      Serial.println(find);
#endif
      if (find>0) {return pin;}
      //delay(100);
    }
  }
#ifdef DEBUG
  Serial.println("\n//\n// End searchDevices() \n//");
#endif
  return 0;
}
