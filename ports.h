#include <stdint.h>
#define NULL (char*)0x0
unsigned char inb(uint16_t port);

void outb(uint16_t port, uint8_t data);
unsigned short inw(unsigned short port);
void outw(unsigned short port, unsigned short data);
unsigned int inl(unsigned short port);
void outl(unsigned short port, unsigned int data);
