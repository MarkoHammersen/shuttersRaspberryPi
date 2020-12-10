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
  uint32_t pinUp;
  uint32_t pinDown;
  uint32_t timerStuck;
  char const *name;
  buttonEvents_t lastEvent;
public:
  button(char const *name, IoPi *io, uint32_t u, uint32_t d);
  buttonEvents_t getSignal();
};

