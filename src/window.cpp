#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "hsm.h"
#include "ABE_IoPi.h"
#include "button.h"
#include "i2cRelayModule.h"
#include "shutter.h"
#include "window.h"

static const Msg buttonMsgs[] =
{
PressUpEvt,
  HoldUpEvt,
  PressDownEvt,
  HoldDownEvt,
  ReleaseEvt,

};


void window::tick(buttonEvents_t allWindowsSig)
{
  if (ReleaseEvt == allWindowsSig)
  {
    buttonEvents_t single = btn.getSignal();
    sh.onEvent(&buttonMsgs[single]);
  }
  else
  {
    sh.onEvent(&buttonMsgs[allWindowsSig]);
  }
}

window::window(char const *name, 
  IoPi *ButtonIoPi,
  uint8_t upButton,
  uint8_t downButton,
  I2cRelayModule *relayModule,
  uint8_t up,
  uint8_t down,
  int32_t timoutShutter):
  sh(name, relayModule, up, down, timoutShutter), 
  btn(name, ButtonIoPi, upButton, downButton)
{
  sh.onStart();

}