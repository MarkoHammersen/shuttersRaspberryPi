#pragma once

int32_t wiringPiSetup(void);


const uint32_t OUTPUT = 1u;
const uint32_t INTPUT = 0u;
const unsigned long I2C_SLAVE = 0x66u;

void pinMode(uint32_t pin, uint32_t direction);
void digitalWrite(uint32_t pin, uint32_t val);
void usleep(uint32_t us);
int open(const char *fileName, int32_t flag);
void close(int fd);
int ioctl(int fd, unsigned long request, uint8_t i2caddress);
int write(int fd, const void *buf, size_t count);
int read(int fd, void *buf, size_t count);
