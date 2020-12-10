#pragma once

class shutter : public Hsm
{
protected:
  State stUp;
  State stDown;
  State stStop;
  State stRunning;

  uint32_t timeout;
  uint32_t tRunning;
  uint32_t tDebounce;
  const char *name;

  uint8_t down;
  uint8_t up;
  I2cRelayModule *relayModule;

public:
  shutter(const char *name, I2cRelayModule *dev,
    uint8_t u,
    uint8_t d,
    int32_t timoutShutter);
  
  Msg const *topHandler(Msg const *msg);
  Msg const *upHandler(Msg const *msg);
  Msg const *downHandler(Msg const *msg);
  Msg const *stopHandler(Msg const *msg);
  Msg const *runningHandler(Msg const *msg);
};
