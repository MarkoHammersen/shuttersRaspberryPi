#pragma once

using namespace ABElectronics_CPP_Libraries;

enum class ButtonEvent
{
  UpEvt,
  DownEvt,
  ReleaseEvt,
};

class Button
{
protected:
  IoPi * myIoPi;
  uint8_t pinButtonUp;
  uint8_t pinButtonDown;

public:
  Button(IoPi *io, uint8_t u, uint8_t d);
  ButtonEvent getSignal();
};

