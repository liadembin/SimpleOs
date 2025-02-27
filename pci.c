#include "kernel.h"
#include "ports.h"
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
// uint16_t pciCheckVendor(uint8_t bus, uint8_t slot) {
//   uint16_t vendor, deviceId;
//   /* Try and read the first configuration register. with all 1's meaning
//   erorr
//    */
//
//   vendor = pciConfigReadWord(bus, slot, 0, 0);
//   if (vendor != 0xFFFF) {
//     deviceId = pciConfigReadWord(bus, slot, 0, 2);
//   }
//   return (vendor);
// }
#include "pci.h"
int pci_print_devices(pci_device *devices[16]) {
  int cnt = 0;
  for (uint32_t bus = 0; bus < 256; bus++) {
    for (uint32_t slot = 0; slot < 32; slot++) {
      for (uint32_t func = 0; func < 8; func++) {
        // brute force, should use recrs but only 2^15 not that bad
        uint16_t ven = getDeviceVendor(bus, slot, func);
        if (ven == 0xFFFF)
          continue;
        uint16_t id = getDeviceId(bus, slot, func);
        char buffer[16];
        int_to_hex_string(ven, buffer, 16);
        put_string("Vendor id : ");
        put_string(buffer);
        int_to_hex_string(id, buffer, 16);
        put_string(" Id: ");
        put_string(buffer);
        put_string(" function: ");
        int_to_hex_string(func, buffer, 16);
        put_string(buffer);
        put_string("\n");
        pci_device *dv = devices[cnt++];
        dv->vendor = ven;
        dv->func = func;
        dv->device = id;
        dv->bus = bus;
        dv->slot = slot;
      }
    }
  }
  return cnt;
}
