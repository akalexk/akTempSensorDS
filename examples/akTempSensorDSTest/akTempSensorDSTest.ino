/* AK_ESP32_TempSensoren_DS1820.ino
 * Biblothek : akTempSensorDS.cpp - example
 * Version 0.2.1 - Dtum 17.02.2019
 * <alexander@koehler-kassel.de>
 *
*/
//Include
#include "akTempSensorDS.h"
#include <OneWire.h>
#include <DallasTemperature.h>

akTempSensorDS akTempSensorDS(15, true);

/*
 * SETUP
 */
void setup(void)
{
  // Start serial port
  Serial.begin(115200);
  Serial.println("AK_ESP32_TempSensoren_DS1820 - OneWire Dallas Temperature (DS) Beispiel");
  // Start DS
  akTempSensorDS.begin("server1/temp");
  Serial.print("OneWire Bus : ");
  Serial.println(akTempSensorDS._pin);
}

/*
 * LOOP
 */
void loop(void)
{
  uint8_t cmdCount = akTempSensorDS.TempMessen();
  Serial.println(akTempSensorDS._cmdCountStr);
  for (int i = 0; i < cmdCount; i++) {
    Serial.println(akTempSensorDS._commands[i]);
  }
  delay(2000);
}
