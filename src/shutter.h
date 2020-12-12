#pragma once

const uint32_t timeBase = 200u; // in ms
const uint32_t timeMaxDebounce = 1000u; // in ms

enum class ShutterEvent
{
  TickEvt
};

class Shutter: public Hsm
{
protected:
  const char* name;
  // shutter
  State stUp;
  State stDown;
  State stStop;
  State stIdle;
  State stRunning;

  uint32_t timeout;
  uint32_t tRunning;
  uint32_t tDebounce;
  uint32_t tStopDebounce;

  uint8_t relayDown; // number of relay for down
  uint8_t relayUp; // number of relay for up
  I2cRelayModule *relayModule;

  uint8_t pinButtonUp;
  uint8_t pinButtonDown;
  ButtonEvent currentBtnEvt;

  Msg const* topHandler(Msg const* msg);
  Msg const* upHandler(Msg const* msg);
  Msg const* downHandler(Msg const* msg);
  Msg const* stopHandler(Msg const* msg);
  Msg const* idleHandler(Msg const* msg);
  Msg const* runningHandler(Msg const* msg);

public:
  ~Shutter();
  Shutter(char const* name,

    uint8_t pinButtonUp,
    uint8_t pinButtonDown,

    I2cRelayModule* relayModule,
    uint8_t relayUp,
    uint8_t relayDown,
    
    int32_t timoutShutterInMs);

  void tick(Buttons& buttons);
  uint8_t checkbit(uint32_t l, uint8_t bit);

};
