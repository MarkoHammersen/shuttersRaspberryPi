#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "ABE_IoPi.h"
#include "button.h"
#include "hsm.h"
#include "i2cRelayModule.h"
#include "shutter.h"

#define T_DEBOUNCE (1u)

#define trace(a) assert(name != NULL);printf(name);printf(": ");printf(a);

shutter::shutter(const char *name, 
  I2cRelayModule *dev,
  uint8_t u,
  uint8_t d,
  int32_t timoutShutter)
  : Hsm(name, (EvtHndlr)&shutter::topHandler),
  stStop(   "Stop",     &top, (EvtHndlr)&shutter::stopHandler),
  stUp(     "Up",       &top, (EvtHndlr)&shutter::upHandler),
  stDown(   "Down",     &top, (EvtHndlr)&shutter::downHandler),
  stRunning("Running",  &top, (EvtHndlr)&shutter::runningHandler)
{
  this->name = name;
  
  tRunning = 0u;
  tDebounce = 0u;

  assert(NULL != dev);
  relayModule = dev;
  up = u;
  down = d;
  assert(timoutShutter > T_DEBOUNCE);
  timeout = timoutShutter;

}

Msg const *shutter::downHandler(Msg const *msg)
{
  switch (msg->evt) 
  {
  case START_EVT:
    trace("down-INIT\n");
    return 0;

  case ENTRY_EVT:
    trace("down-ENTRY\n");
    if (NULL != relayModule)
    {
      relayModule->changeMode(up, OFF);
      relayModule->changeMode(down, ON);
    }
    return 0;

  case EXIT_EVT:
    trace("down-EXIT\n");
    return 0;

  default:
    tDebounce++;
    if (tDebounce >= T_DEBOUNCE)
    {
      tDebounce = 0;
      STATE_TRAN(&stRunning);
    }
    return 0;
  }
  return msg;
}

Msg const *shutter::stopHandler(Msg const *msg)
{
  switch (msg->evt) 
  {
  case START_EVT:
    trace("stop-INIT\n");
    return 0;
  
  case ENTRY_EVT:
    trace("stop-ENTRY\n");
    relayModule->changeMode(up, OFF);
    relayModule->changeMode(down, OFF);
    return 0;
  
  case EXIT_EVT:
    trace("stop-EXIT\n");
    return 0;

  case PressDownEvt:
    STATE_TRAN(&stDown);
    return 0;

  case PressUpEvt:
    STATE_TRAN(&stUp);
    return 0;
  }
  return msg;
}

Msg const *shutter::runningHandler(Msg const *msg)
{
  switch (msg->evt)
  {
  case START_EVT:
    trace("running-INIT\n");
    return 0;

  case ENTRY_EVT:
    trace("running-ENTRY\n");
    tRunning = 0u;
    return 0;

  case EXIT_EVT:
    trace("running-EXIT\n");
    return 0;

  case PressDownEvt:
  case PressUpEvt:
    STATE_TRAN(&stStop);
    return 0;
  default:
    tRunning++;
    if (tRunning >= (timeout - T_DEBOUNCE))
    {
      tRunning = 0u;
      STATE_TRAN(&stStop);
    }
    return 0;
  }
  return msg;
}

Msg const *shutter::upHandler(Msg const *msg)
{
  switch (msg->evt) 
  {
  case START_EVT:
    trace("up-INIT\n");
    return 0;

  case ENTRY_EVT:
    trace("up-ENTRY\n");
    relayModule->changeMode(down, OFF);
    relayModule->changeMode(up, ON);
    return 0;

  case EXIT_EVT:
    trace("up-EXIT\n");
    return 0;

  default:
    tDebounce++;
    if (tDebounce >= T_DEBOUNCE)
    {
      tDebounce = 0;
      STATE_TRAN(&stRunning);
    }
    return 0;
  }
  return msg;
}

Msg const *shutter::topHandler(Msg const *msg) 
{
  switch (msg->evt) {
  case START_EVT:
    trace("top-INIT\n");
    STATE_START(&stStop);
    return 0;
  case ENTRY_EVT:
    trace("top-ENTRY\n");
    return 0;
  case EXIT_EVT:
    trace("top-EXIT\n");
    return 0;
  }
  return msg;
}
