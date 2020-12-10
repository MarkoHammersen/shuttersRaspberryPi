#pragma once

typedef union _BYTE_TO_WORD {
  uint8_t b[2u];
  uint16_t w;
} BYTE_TO_WORD;

const uint16_t ON = 1u;
const uint16_t OFF = 0u;

class I2cRelayModule 
{
protected:
  void writeData(uint8_t data[], uint32_t count);

  uint8_t i2caddress; // I2C address
  BYTE_TO_WORD modeWord;
 
public:
  I2cRelayModule(uint8_t addr);
  void changeMode(uint8_t channel, uint16_t mode);
  void setAll(uint16_t mode);
};
