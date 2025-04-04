#include <stdint.h>
#define NET_CARD_ID 0x100E
typedef struct {
    uint32_t vendor;
    uint32_t device;
    uint32_t func;
    uint8_t bus;
    uint8_t slot;
} pci_device;
typedef struct {
    uint32_t vendor;
	uint32_t device;
	uint32_t func;
} pci_id;
#include <stdint.h>
uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func,
                           uint8_t offset);
uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function);

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function) ;
uint16_t getDeviceVendor(uint8_t bus, uint8_t slot, uint8_t func) ;
uint16_t getDeviceId(uint8_t bus, uint8_t slot, uint8_t func) ;

int pci_print_devices(pci_device *devices[16]) ;

