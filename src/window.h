#pragma once

class window
{
protected:
  shutter sh;
  button btn;

public:
  window(char const *name, 
    IoPi *ButtonIoPi,
    uint8_t upButton,
    uint8_t downButton,
    I2cRelayModule *relayModule,
    uint8_t up,
    uint8_t down,
    int32_t timoutShutter);
  void tick(buttonEvents_t allWindowsSig);
};
