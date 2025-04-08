#ifndef PCI_H
#define PCI_H
#include "ports.h"
#include <stdint.h>

#define NET_CARD_ID 0x100E
#define RTL8139_ID 0x8139

typedef struct {
  uint32_t vendor;
  uint32_t device;
  uint32_t func;
  uint8_t bus;
  uint8_t slot;
  uint32_t baseaddr; // Base address register
  uint8_t irq_num;
} pci_device;

typedef struct {
  uint32_t vendor;
  uint32_t device;
  uint32_t func;
} pci_id;

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func,
                           uint8_t offset);
uint32_t pciConfigReadDWord(uint8_t bus, uint8_t slot, uint8_t func,
                            uint8_t offset);
uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function);
uint16_t getDeviceVendor(uint8_t bus, uint8_t slot, uint8_t func);
uint16_t getDeviceId(uint8_t bus, uint8_t slot, uint8_t func);
uint32_t getBaseAddress(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar);
int pci_print_devices(pci_device *devices[16]);
void pciConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset,
                        uint8_t value);
#endif
