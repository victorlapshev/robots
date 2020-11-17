#include <Wire.h>
#include "ClosedCube_TCA9548A.h"
#include <Adafruit_MCP4725.h>

#define UART_BAUD 9600

#define TCA9548A_I2C_ADDRESS 0x70
#define MCP4725_I2C_ADDRESS 0x60

#define CHANNELS_COUNT 4

ClosedCube::Wired::TCA9548A tca9548a;
Adafruit_MCP4725 MCP4725;

void setup()
{
  Serial.begin(UART_BAUD);
  Wire.begin();
  MCP4725.begin(MCP4725_I2C_ADDRESS);

  tca9548a.address(TCA9548A_I2C_ADDRESS);
}

char buffer[CHANNELS_COUNT];

void loop()
{
  if (Serial.available() > 0)
  {
    Serial.readBytes(buffer, CHANNELS_COUNT);

    for (int i = 0; i < CHANNELS_COUNT; i++)
    {
      tca9548a.selectChannel(i + 1);
      MCP4725.setVoltage(buffer[i] * 16, false);
    }
  }
}