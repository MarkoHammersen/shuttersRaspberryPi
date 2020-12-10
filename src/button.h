#pragma once

using namespace ABElectronics_CPP_Libraries;

typedef enum
{
  PressUpEvt,
  HoldUpEvt,
  PressDownEvt,
  HoldDownEvt,
  ReleaseEvt,
  
}buttonEvents_t;

class button
{
protected:
  IoPi * myIoPi;
  uint8_t pinUp;
  uint8_t pinDown;
  uint32_t timerStuck;
  char const *name;
  buttonEvents_t lastEvent;
public:
  button(char const *name, IoPi *io, uint8_t u, uint8_t d);
  buttonEvents_t getSignal();
};

