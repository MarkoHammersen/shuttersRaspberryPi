#pragma once

using namespace ABElectronics_CPP_Libraries;

union AllInputs
{
  uint8_t b[4u];
  uint32_t l;
};

enum class ButtonEvent
{
  UpEvt,
  DownEvt,
  ReleaseEvt,
};

class Buttons
{
protected:
  uint8_t pinAllUp;
  uint8_t pinAllDown;

public:
  AllInputs input;
  Buttons(uint8_t u, uint8_t d)
  {
    pinAllUp = u;
    pinAllDown = d;
    input.l = 0u;
  };
  uint8_t getPinAllUp() { return pinAllUp; }
  uint8_t getPinAllDown() { return pinAllDown; }
};

