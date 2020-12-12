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
public:
  uint8_t pinAllUp;
  uint8_t pinAllDown;
  AllInputs input;
  Buttons(uint8_t u, uint8_t d)
  {
    pinAllUp = u;
    pinAllDown = d;
  };
};

