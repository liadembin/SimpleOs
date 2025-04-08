#include "pci.h"
#include "vga.h"
#include <stdint.h>

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func,
                           uint8_t offset) {
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;
  uint16_t tmp = 0;
  // Create configuration address as per Figure 1
  address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) |
                       (offset & 0xFC) | ((uint32_t)0x80000000));
  // Write out the address
  outl(0xCF8, address);
  // Read in the data
  // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
  tmp = (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
  return tmp;
}
uint8_t getInterruptLine(uint8_t bus, uint8_t slot, uint8_t func) {
  // The Interrupt Line register is at offset 0x3C, lowest byte
  return (uint8_t)(pciConfigReadDWord(bus, slot, func, 0x3C) & 0xFF);
}
uint32_t pciConfigReadDWord(uint8_t bus, uint8_t slot, uint8_t func,
                            uint8_t offset) {
  uint32_t address;
  uint32_t lbus = (uint32_t)bus;
  uint32_t lslot = (uint32_t)slot;
  uint32_t lfunc = (uint32_t)func;

  // Create configuration address
  address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) |
                       (offset & 0xFC) | ((uint32_t)0x80000000));

  // Write out the address
  outl(0xCF8, address);

  // Read in the data (full 32 bits)
  return inl(0xCFC);
}

uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function) {
  uint32_t r0 = pciConfigReadWord(bus, device, function, 0xA);
  return (r0 & ~0x00FF) >> 8;
}

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function) {
  uint32_t r0 = pciConfigReadWord(bus, device, function, 0xA);
  return (r0 & ~0xFF00);
}

uint16_t getDeviceVendor(uint8_t bus, uint8_t slot, uint8_t func) {
  return pciConfigReadWord(bus, slot, func, 0);
}

uint16_t getDeviceId(uint8_t bus, uint8_t slot, uint8_t func) {
  return pciConfigReadWord(bus, slot, func, 2);
}

uint32_t getBaseAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar) {
  // BAR starts at offset 0x10 and each BAR is 4 bytes (32 bits)
  uint8_t offset = 0x10 + (bar * 4);
  uint32_t bar_value = pciConfigReadDWord(bus, slot, func, offset);

  // Check if this is an I/O space BAR (bit 0 set)
  if (bar_value & 0x1) {
    // I/O space - mask out the lower 2 bits (bit 0 is always 1 for I/O)
    return bar_value & 0xFFFFFFFC;
  } else {
    // Memory space - mask out the lower 4 bits (type and prefetchable bits)
    return bar_value & 0xFFFFFFF0;
  }
}

int pci_print_devices(pci_device *devices[16]) {
  int cnt = 0;
  for (uint32_t bus = 0; bus < 256; bus++) {
    for (uint32_t slot = 0; slot < 32; slot++) {
      for (uint32_t func = 0; func < 8; func++) {
        // Brute force, should use recursion but only 2^15 not that bad
        uint16_t ven = getDeviceVendor(bus, slot, func);
        if (ven == 0xFFFF)
          continue;

        uint16_t id = getDeviceId(bus, slot, func);

        if (cnt < 16) {
          pci_device *dv = devices[cnt++];
          dv->vendor = ven;
          dv->func = func;
          dv->device = id;
          dv->bus = bus;
          dv->slot = slot;
          dv->irq_num = getInterruptLine(bus, slot, func);
          // Get the first BAR (BAR0) for this device
          // You can modify this to scan all BARs (typically 6) if needed
          dv->baseaddr = getBaseAddress(bus, slot, func, 0);
        }
      }
    }
  }
  return cnt;
}
void pciConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset,
                        uint8_t value) {
  // Create the address for the PCI config space
  uint32_t address =
      (1U << 31)               // Enable bit
      | ((uint32_t)bus << 16)  // Bus number
      | ((uint32_t)slot << 11) // Device number (slot)
      | ((uint32_t)func << 8)  // Function number
      | (offset & 0xFC);       // Register offset (must be aligned to 4 bytes)

  // Write address to PCI config address port
  outl(0xCF8, address);

  // Write the byte at the correct offset (offset % 4) into the PCI config data
  // port
  outb(0xCFC + (offset & 3), value);
}
