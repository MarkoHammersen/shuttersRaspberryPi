#include <cstdint>
#include <assert.h>

#ifdef _WIN32
#include < windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "hsm.h"
#include "ABE_IoPi.h"
#include "ABE_I2CSwitch.h"
#include "i2cRelayModule.h"

#include "button.h"
#include "shutter.h"
#include "window.h"


int initI2CSwitch(unsigned char address)
{
  unsigned int i;
  unsigned char i2cSwitchChannels[3] = { 1u, 2u ,3u }; // channel to select
  I2CSwitch i2cswitch(address);     
  
  // reset the i2c switch
  i2cswitch.reset();

  for (i = 0u; i < sizeof(i2cSwitchChannels) / sizeof(*i2cSwitchChannels); i++)
  {
    // switch to the selected channel
    i2cswitch.switch_channel(i2cSwitchChannels[i]);

    // check the state of the selected channel
    if (i2cswitch.get_channel_state(i2cSwitchChannels[i]) != 1)
    {
#ifdef NDEBUG
      break;
#endif // !NDEBUG
    }
  }

  if (i < sizeof(i2cSwitchChannels) / sizeof(*i2cSwitchChannels))
  {
    return -1;
  }
  return 0;
}

static void doSleep(uint32_t ms)
{
    struct timeval t1;
    struct timeval t2;
    long long elapsedTimeInUs;

    gettimeofday(&t1, NULL);
    while (1)
    {
        sleep(1);
        gettimeofday(&t2, NULL);
        elapsedTimeInUs = ((t2.tv_sec * 1000000) + t2.tv_usec)
            - ((t1.tv_sec * 1000000) + t1.tv_usec);
        if (elapsedTimeInUs > (ms * 1000u))
        {
            break;
        }
    }
}

int main()
{
  IoPi busInput1(0x20u, false);
  IoPi busInput2(0x21u, false);
  
  if (0 != initI2CSwitch(0x70u))
  {
    return -1;
  }

  I2cRelayModule relayBoard1(0x24);
  I2cRelayModule relayBoard2(0x25);
  I2cRelayModule relayBoard3(0x26);
  
 
  busInput1.set_port_direction(0, 0xFF); // set bank 0 to be inputs
  busInput1.set_port_direction(1, 0xFF); // set bank 1 to be inputs
  busInput1.set_port_pullups(0, 0xFF);   // enable internal pullups for bank 0
  busInput1.invert_port(0, 0xFF);		  // invert output so bank will read as 0
  busInput1.set_port_pullups(1, 0xFF);   // enable internal pullups for bank 0
  busInput1.invert_port(1, 0xFF);		  // invert output so bank will read as 0

  busInput2.set_port_direction(0, 0xFF); // set bank 0 to be inputs
  busInput2.set_port_direction(1, 0xFF); // set bank 1 to be inputs
  busInput2.set_port_pullups(0, 0xFF);   // enable internal pullups for bank 0
  busInput2.invert_port(0, 0xFF);		  // invert output so bank will read as 0
  busInput2.set_port_pullups(1, 0xFF);   // enable internal pullups for bank 0
  busInput2.invert_port(1, 0xFF);		  // invert output so bank will read as 0

  button all("AllShuttersButton", &busInput1, 11u, 12u);

  window hall("hall",
    &busInput1, 1u, 2u,
    &relayBoard1, 1u, 2u,
    1500u);
/*
  window bathroom("bathroom",
    &busInput1, 3u, 4u,
    1500u);

  window dirtlock("dirtlock",
    &busInput1, 5u, 6u,
    1500u);

  window westDoor("westDoor",
    &busInput1, 7u, 8u,
    1500u);

  window westWindow("westWindow",
    &busInput1, 9u, 10u,
    1500u);

  window northWindow("northWindow",
    &busInput1, 11u, 12u,
    1500u);

  window eastWindowSmall("eastWindowSmall",
    &busInput1, 13u, 14u,
    1500u);

  window northDoor("northDoor",
    &busInput1, 15u, 16u,
    1500u);

  window eastWindowDining("eastWindowDining",
    &busInput2, 1u, 2u,
    1500u);

  window eastWindowKitchen("eastWindowKitchen",
    &busInput2, 3u, 4u,
    1500u);

  window eastDoor("eastDoor",
    &busInput2, 5u, 6u,
    1500u);

  window southWindowLarge("southWindowLarge",
    &busInput2, 7u, 8u,
    1500u);

  window southWindowSmall("southWindowSmall",
    &busInput2, 9u, 10u,
    1500u);*/

  while (1u)
  {
    // get status of broadcast input
    buttonEvents_t AllWindowsSig = all.getSignal();

    hall.tick(AllWindowsSig);
    //bathroom.tick(AllWindowsSig);
    //dirtlock.tick(AllWindowsSig);
    //westDoor.tick(AllWindowsSig);
    //westWindow.tick(AllWindowsSig);
    //northWindow.tick(AllWindowsSig);
    //eastWindowSmall.tick(AllWindowsSig);
    //northDoor.tick(AllWindowsSig);
    //eastWindowDining.tick(AllWindowsSig);
    //eastWindowKitchen.tick(AllWindowsSig);
    //eastDoor.tick(AllWindowsSig);
    //southWindowLarge.tick(AllWindowsSig);
    //southWindowSmall.tick(AllWindowsSig);

#ifdef _WIN32
    Sleep(10u);
#else
    doSleep(10u); // in ms
#endif
  }

  return 0;
}