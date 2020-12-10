#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "ABE_IoPi.h"
#include "button.h"

#define T_STUCK (4u)

button::button(char const *name, IoPi *io, uint8_t u, uint8_t d)
{
  this->name = name;
  timerStuck = 0;
  lastEvent = ReleaseEvt;

  assert(NULL != io);
  assert((u <= 16) && (u >= 1));
  assert((d <= 16) && (d >= 1));

  myIoPi = io;
  pinUp = u;
  pinDown = d;
}

buttonEvents_t button::getSignal()
{
  assert(NULL != myIoPi);

  uint8_t up = myIoPi->read_pin(pinUp);
  uint8_t down = myIoPi->read_pin(pinDown);

  if ((1u == up) && (ReleaseEvt == lastEvent))
  {
    // up pressed 
    lastEvent = PressUpEvt;
    return PressUpEvt;
  }

  if (1u == up) 
  {
    // up held
    lastEvent = HoldUpEvt;
    return HoldUpEvt;
  }
  
  if ((1u == down) && (ReleaseEvt == lastEvent))
  {
    // down pressed
    lastEvent = PressDownEvt;
    return PressDownEvt;
  }

  if (1u == down) 
  {
    // down held
    lastEvent = HoldDownEvt;
    return HoldDownEvt;
  }

  // released
  lastEvent = ReleaseEvt;
  return ReleaseEvt;
  
}