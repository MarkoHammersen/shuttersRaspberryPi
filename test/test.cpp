#include <Windows.h>
#include <stdint.h>
#include <assert.h>

#include <Windows.h>
#include <direct.h>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

//
//int main()
//{
//IoPi busInput1(0x20, false);
//  IoPi busOutput1(0x21, false);
//
//  button all("AllShuttersButton");
//  all.configurePins(&busInput1, 11u, 12u);
//
//  window hall("hall",
//    &busInput1, 1u, 2u,
//    &busOutput1, 1u, 2u,
//    1500u);
///*
//  window bathroom("bathroom",
//    &busInput1, 3u, 4u,
//    &busOutput1, 3u, 4u,
//    1500u);
//
//*/
//  while (1u)
//  {
//    // get status of broadcast input
//    buttonEvents_t AllWindowsSig = all.getSignal();
//
//    hall.tick(AllWindowsSig);
//    //bathroom.tick(AllWindowsSig);
//
//    Sleep(10u);
//  }
//
//  return 0;
//
//}

int32_t wiringPiSetup(void)
{
  return 0;
}

void pinMode(uint32_t pin, uint32_t direction)
{

}

// set reset pin high
void digitalWrite(uint32_t pin, uint32_t val)
{

}

void usleep(uint32_t us)
{
  std::this_thread::sleep_for(std::chrono::microseconds(us));
}

int open(const char *fileName, int32_t flag)
{
  return 6;
}

void close(int fd)
{
  
}

int ioctl(int fd, unsigned long request, uint8_t i2caddress)
{
  return(0);
}

int write(int fd, const void *buf, size_t count)
{
  return (count);
}

int read(int fd, void *buf, size_t count)
{
  int i;
  for (i = 0; i < (int)count; i++)
  {
    ((char*)buf)[i] = 1;
  }
  return (count);
}
