#include <cstdint>

#ifdef _WIN32
#include < windows.h>
#else
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#include "ABE_IoPi.h"
#include "ABE_I2CSwitch.h"
#include "i2cRelayModule.h"

#include <assert.h>
#include "hsm.h"
#include "button.h"
#include "shutter.h"

using namespace std;

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

static long long doSleep(uint32_t ms)
{
#ifdef _WIN32
    Sleep(ms);
    return ms;
#else
    struct timeval t1;
    struct timeval t2;
    long long elapsedTimeInUs;

    gettimeofday(&t1, NULL);
    while (1)
    {
        usleep((ms * 1000u) / 20u);
        gettimeofday(&t2, NULL);
        elapsedTimeInUs = ((static_cast<long long>(t2.tv_sec) * 1000000) + t2.tv_usec)
            - ((static_cast<long long>(t1.tv_sec) * 1000000) + t1.tv_usec);
        if (elapsedTimeInUs > (static_cast<long long>(ms) * 1000u))
        {
            break;
        }
    }
    return elapsedTimeInUs;
#endif
}

#if 0

static void testTimer()
{
  while (1)
  {
    long long elapsedTimeInUs = doSleep(15000);
    printf("t = %lld\n" , elapsedTimeInUs);
  }
}


int main()
{
  testTimer();
}
#else
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

  Shutter shutter[] = {
    Shutter("hall", 3u, 4u, &relayBoard1, 1u, 2u, 15000u),
    Shutter("bathroom",5u,6u, &relayBoard1, 3u, 4u, 15000u),
    Shutter("dirtlock",7u,8u,&relayBoard1, 5u, 6u,15000u),
    Shutter("westDoor", 7u,8u,&relayBoard1, 7u, 8u, 15000u),
    Shutter("westWindow", 7u,8u,&relayBoard1, 9u, 10u, 15000u),
    Shutter("northWindow", 7u,8u,&relayBoard1, 11u, 12u, 15000u),
    Shutter("eastWindowSmall", 7u,8u,&relayBoard1, 13u, 14u, 15000u),
    Shutter("northDoor", 7u,8u,&relayBoard1, 15u, 16u, 15000u),
    Shutter("eastWindowDining", 7u,8u,&relayBoard1, 1u, 2u, 15000u),
    Shutter("eastWindowKitchen", 7u,8u,&relayBoard1, 3u, 4u, 15000u),
    Shutter("eastDoor", 7u,8u,&relayBoard1, 5u, 6u, 15000u),
    Shutter("southWindowLarge", 7u,8u,&relayBoard1, 7u, 8u, 15000u),
    Shutter("southWindowSmall", 7u,8u,&relayBoard1, 9u, 10u, 15000u)
  };

  Buttons buttons = Buttons(1u, 2u);
  while (1u)
  {
    // get status of broadcast input
    buttons.input.b[0u] = busInput1.read_port(0u);
    buttons.input.b[1u] = busInput1.read_port(1u);
    buttons.input.b[2u] = busInput2.read_port(0u);
    buttons.input.b[3u] = busInput2.read_port(1u);

    for(uint32_t i = 0u; i < (sizeof(shutter)/sizeof(*shutter)); i++)
    {
      shutter[i].tick(buttons);
    }
  
    (void)doSleep(timeBase); // in ms
  }

  return 0;
}
#endif