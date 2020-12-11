#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "ABE_IoPi.h"
#include "button.h"
#include "hsm.h"
#include "i2cRelayModule.h"
#include "shutter.h"

#define trace(a) assert(name != NULL);printf(name);printf(": ");printf(a);

static const Msg ShutterMsgs[] =
{
  static_cast <Event>(ShutterEvent::TickEvt)
};

Shutter::Shutter(char const* name,
  IoPi* buttonIoPi,
  uint8_t pinButtonUp,
  uint8_t pinButtonDown,
  I2cRelayModule* relayModule,
  uint8_t relayUp,
  uint8_t relayDown,
  int32_t timoutShutterInMs): Hsm(name, (EvtHndlr)&Shutter::topHandler),
  stUp(     "Up",       &top, (EvtHndlr)&Shutter::upHandler),
  stDown(   "Down",     &top, (EvtHndlr)&Shutter::downHandler),
  stRunning("Running",  &top, (EvtHndlr)&Shutter::runningHandler),
  stStop(   "Stop",     &top, (EvtHndlr)&Shutter::stopHandler),
  stIdle("idle", &top, (EvtHndlr)&Shutter::idleHandler),
  Button(buttonIoPi, pinButtonUp, pinButtonDown)
{
  this->name = name;
  
  tRunning = 0u;
  tDebounce = 0u;
  tStopDebounce = 0u;
  timeout = timoutShutterInMs;

  currentBtnEvt = ButtonEvent::ReleaseEvt;

  this->relayModule = relayModule;
  this->relayUp = relayUp;
  this->relayDown = relayDown;
}

Msg const *Shutter::downHandler(Msg const *msg)
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
      relayModule->changeMode(relayUp, OFF);
      relayModule->changeMode(relayDown, ON);
      tDebounce = 0u;
    }
    return 0;

  case EXIT_EVT:
    trace("down-EXIT\n");
    return 0;

  default:
    if (ButtonEvent::UpEvt == currentBtnEvt)
    {
      STATE_TRAN(&stStop);
      return 0;
    }
    tDebounce += timeBase;
    if (tDebounce >= timeMaxDebounce) // ticks
    {
      STATE_TRAN(&stRunning);
    }
    return 0;
  }
  return msg;
}

Msg const* Shutter::stopHandler(Msg const* msg)
{
  switch (msg->evt)
  {
  case START_EVT:
    trace("idle-INIT\n");
    return 0;

  case ENTRY_EVT:
    trace("idle-ENTRY\n");
    relayModule->changeMode(relayUp, OFF);
    relayModule->changeMode(relayDown, OFF);
    tStopDebounce = 0;
    return 0;

  case EXIT_EVT:
    trace("idle-EXIT\n");
    return 0;
  
  case static_cast <Event>(ShutterEvent::TickEvt):
    if (tStopDebounce > timeMaxDebounce) // ticks
    {
      STATE_TRAN(&stIdle);
    }
    else
    {
      tStopDebounce += timeBase;
    }
    return 0;
  }
  return msg;
}

Msg const *Shutter::idleHandler(Msg const *msg)
{
  switch (msg->evt)
  {
  case START_EVT:
    trace("idle-INIT\n");
    return 0;

  case ENTRY_EVT:
    trace("idle-ENTRY\n");
    return 0;

  case EXIT_EVT:
    trace("idle-EXIT\n");
    return 0;

    case static_cast<Event>(ShutterEvent::TickEvt) :
    {
      if (ButtonEvent::UpEvt == currentBtnEvt)
      {
        STATE_TRAN(&stUp);
        return 0;
      }

      if (ButtonEvent::DownEvt == currentBtnEvt)
      {
        STATE_TRAN(&stDown);
        return 0;
      }
    }
  }
  return msg;
}

Msg const *Shutter::upHandler(Msg const *msg)
{
  switch (msg->evt) 
  {
  case START_EVT:
    trace("up-INIT\n");
    return 0;

  case ENTRY_EVT:
    trace("up-ENTRY\n");
    relayModule->changeMode(relayDown, OFF);
    relayModule->changeMode(relayUp, ON);
    tDebounce = 0u;
    return 0;

  case EXIT_EVT:
    trace("up-EXIT\n");
    return 0;

  default:
  {
    if (ButtonEvent::DownEvt == currentBtnEvt)
    {
      STATE_TRAN(&stStop);
      return 0;
    }

    tDebounce += timeBase;
    if (tDebounce >= timeMaxDebounce ) // ticks
    {
      STATE_TRAN(&stRunning);
    }
  }
    return 0;
  }
  return msg;
}

Msg const *Shutter::topHandler(Msg const *msg) 
{
  switch (msg->evt) {
  case START_EVT:
    trace("top-INIT\n");
    if (NULL != relayModule)
    {
      STATE_START(&stStop);
    }
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

Msg const* Shutter::runningHandler(Msg const* msg)
{
  switch (msg->evt) {
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
  default:
    if (ButtonEvent::ReleaseEvt != currentBtnEvt)
    {
      STATE_TRAN(&stStop);
      return 0;
    }

    tRunning = tRunning + timeBase;
    if (tRunning >= timeout)
    {
      STATE_TRAN(&stStop);
    }
    return 0;
  }
  return msg;
}

void Shutter::tick(ButtonEvent all)
{
  if (ButtonEvent::ReleaseEvt == all)
  {
    currentBtnEvt = getSignal();
  }
  else
  {
    currentBtnEvt = all;
  }
  onEvent(&ShutterMsgs[static_cast <Event>(ShutterEvent::TickEvt)]);
}