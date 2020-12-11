//
// I2C slave addresses are 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27
// Addresses are changeable via dip switch on pcb
// Tested with Arduino Uno ATMEGA328 and 16CH I2C Electromagnetic Relay Module
//


/* Required package{
apt - get install libi2c - dev
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#ifndef _WIN32
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#else
#include "test.h"
#endif

#include "i2cRelayModule.h"

#define fileName "/dev/i2c-1" // change to /dev/i2c-0 if you are using a Raspberry Pi revision 0002 or 0003 model B



// stops file handle leakage on exceptions
class ScopedFile
{
public:
  ScopedFile(int fd) : _fd(fd) {}
  ~ScopedFile()
  {
    if (_fd >= 0)
      close(_fd);
  }
  operator int() const { return _fd; }

private:
  int _fd;
};

I2cRelayModule::I2cRelayModule(uint8_t addr)
{
  uint8_t data[2u];
  i2caddress = addr;

  modeWord.w = 0u;
  
  data[0u] = 0x00u; // register A
  data[1u] = 0x00u; // set all of port A to outputs
  writeData(data, 2);

  data[0u] = 0x01u; // register B
  data[1u] = 0x00u; // set all of port B to outputs
  writeData(data, 2);
  
  setAll(OFF);
}

void I2cRelayModule::writeData(uint8_t data[], int32_t count)
{
  /**
  * private method for writing a byte to the I2C port
  */
  ScopedFile i2cbus(open(fileName, O_RDWR));
  if (i2cbus < 0)
  {
    throw std::runtime_error("Failed to open i2c port for write");
  }

  if (ioctl(i2cbus, I2C_SLAVE, i2caddress) < 0)
  {
    throw std::runtime_error("Failed to write to i2c port for write");
  }

  if ((write(i2cbus, data, count)) != count)
  {
    throw std::runtime_error("Failed to write to i2c device for write");
  }

  close(i2cbus);
}

//
// This function control each channel separately ON/OFF
//
void I2cRelayModule::changeMode(uint8_t relay, uint16_t mode)
{
  uint8_t data[2u];
  uint8_t variable_LOW;
  uint8_t variable_HIGH;

  switch (relay) {
    case 16: relay = 7; break;
    case 15: relay = 6; break;
    case 14: relay = 5; break;
    case 13: relay = 4; break;
    case 12: relay = 3; break;
    case 11: relay = 2; break;
    case 10: relay = 1; break;
    case  9: relay = 0; break;
    case  8: relay = 15; break;
    case  7: relay = 14; break;
    case  6: relay = 13; break;
    case  5: relay = 12; break;
    case  4: relay = 11; break;
    case  3: relay = 10; break;
    case  2: relay = 9; break;
    case  1: relay = 8; break;
    default:
      relay = 0xFF;
      break;
  }

  if (0xFFu == relay)
  {
    return;
  }

  modeWord.w &= ~(1u << (relay));
  modeWord.w |= mode << relay;

  variable_LOW = modeWord.b[0u];
  variable_HIGH = modeWord.b[1u];

  data[0u] = 0x12u;  // address bank A
  data[1u] = variable_LOW;
  writeData(data, 2);

  data[0u] = 0x13u;  // address bank B
  data[1u] = variable_HIGH;
  writeData(data, 2);
}

//
// This function turn off all channels
//
void I2cRelayModule::setAll(uint16_t mode)
{
  uint8_t data[2u];
  uint8_t variable_LOW;
  uint8_t variable_HIGH;

  if (ON == mode)
  {
    modeWord.w = 0xFFFFu;
  }
  else if (OFF == mode)
  {
    modeWord.w = 0u;
  }
  else
  {
    return;
  }

  variable_LOW = modeWord.b[0u];
  variable_HIGH = modeWord.b[1u];

  data[0u] = 0x12u; // address bank A
  data[1u] = variable_LOW;
  writeData(data, 2);

  data[0u] = 0x13u; // address bank B
  data[1u] = variable_HIGH;
  writeData(data, 2);
}
