#include <cstdint>
#include "ABE_IoPi.h"
#include "button.h"

Button::~Button()
{}

Button::Button(uint8_t u, uint8_t d)
{
  pinButtonDown = 0xFFu;
  pinButtonUp = 0xFFu;

  if ((u <= 16) && (u >= 1))
  {
    if ((d <= 16) && (d >= 1))
    {
      pinButtonUp = u;
      pinButtonDown = d;
    }
  }
}

ButtonEvent Button::getSignal()
{
 
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
