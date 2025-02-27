unsigned char inb(unsigned short port) {
  unsigned char result;
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

void outb(unsigned short port, unsigned char data) {
  __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Read a word (2 bytes) from the specified port
unsigned short inw(unsigned short port) {
  unsigned short result;
  __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

// Write a word (2 bytes) to the specified port
void outw(unsigned short port, unsigned short data) {
  __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

// Read a double word (4 bytes) from the specified port
unsigned int inl(unsigned short port) {
  unsigned int result;
  __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

// Write a double word (4 bytes) to the specified port
void outl(unsigned short port, unsigned int data) {
  __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}
