#include <cstdint>
#include "ABE_IoPi.h"
#include "button.h"

Button::Button(IoPi *io, uint8_t u, uint8_t d)
{
  myIoPi = 0;
  pinButtonDown = 0xFFu;
  pinButtonUp = 0xFFu;

  if (0 != io)
  {
    if ((u <= 16) && (u >= 1))
    {
      if ((d <= 16) && (d >= 1))
      {
        myIoPi = io;
        pinButtonUp = u;
        pinButtonDown = d;
      }
    }
  }  
}

ButtonEvent Button::getSignal()
{
  if (0 != myIoPi)
  {
    uint8_t up = myIoPi->read_pin(pinButtonUp);
    uint8_t down = myIoPi->read_pin(pinButtonDown);

    if (1u == up)
    {
      // up pressed 
      return ButtonEvent::UpEvt;
    }

    if (1u == down)
    {
      // down pressed
      return ButtonEvent::DownEvt;
    }

    // released
    return ButtonEvent::ReleaseEvt;
  }

  // released
  return ButtonEvent::ReleaseEvt;
}
