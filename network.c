#include "network.h"
#include "paging.h"
#include "vga.h"
#include "stdlib.h"

// Intel 82540EM (E1000) specific register offsets
#define E1000_REG_CTRL        0x0000  // Control Register
#define E1000_REG_STATUS      0x0008  // Status Register
#define E1000_REG_EEPROM      0x0014  // EEPROM Read Register
#define E1000_REG_CTRL_EXT    0x0018  // Extended Control Register
#define E1000_REG_MDIC        0x0020  // MDI Control Register
#define E1000_REG_RCTL        0x0100  // Receive Control Register
#define E1000_REG_TCTL        0x0400  // Transmit Control Register
#define E1000_REG_RDBAL       0x2800  // RX Descriptor Base Address Low
#define E1000_REG_RDBAH       0x2804  // RX Descriptor Base Address High
#define E1000_REG_RDLEN       0x2808  // RX Descriptor Length
#define E1000_REG_RDH         0x2810  // RX Descriptor Head
#define E1000_REG_RDT         0x2818  // RX Descriptor Tail
#define E1000_REG_TDBAL       0x3800  // TX Descriptor Base Address Low
#define E1000_REG_TDBAH       0x3804  // TX Descriptor Base Address High
#define E1000_REG_TDLEN       0x3808  // TX Descriptor Length
#define E1000_REG_TDH         0x3810  // TX Descriptor Head
#define E1000_REG_TDT         0x3818  // TX Descriptor Tail
#define E1000_REG_ICR         0x00C0  // Interrupt Cause Read
// Control register bits
#define E1000_CTRL_FD         0x00000001  // Full duplex
#define E1000_CTRL_ASDE       0x00000020  // Auto-speed detection enable
#define E1000_CTRL_SLU        0x00000040  // Set link up
#define E1000_CTRL_ILOS       0x00000080  // Invert Loss-of-Signal
#define E1000_CTRL_RST        0x04000000  // Reset
#define E1000_CTRL_PHY_RST    0x80000000  // PHY Reset
// Receive Control Register bits
#define E1000_RCTL_EN         0x00000002  // Receiver Enable
#define E1000_RCTL_SBP        0x00000004  // Store Bad Packets
#define E1000_RCTL_UPE        0x00000008  // Unicast Promiscuous Mode
#define E1000_RCTL_MPE        0x00000010  // Multicast Promiscuous Mode
#define E1000_RCTL_LBM_NONE   0x00000000  // No loopback
#define E1000_RCTL_BSIZE_2048 0x00000000  // Buffer size 2048
#define E1000_RCTL_SECRC      0x04000000  // Strip Ethernet CRC

// Transmit Control Register bits
#define E1000_TCTL_EN         0x00000002  // Transmit Enable
#define E1000_TCTL_PSP        0x00000008  // Pad Short Packets
#define E1000_TCTL_CT         0x00000100  // Collision Threshold
#define E1000_TCTL_COLD       0x00040000  // Collision Distance

// Descriptor status bits
#define E1000_RXD_STAT_DD     0x01  // Descriptor Done
#define E1000_RXD_STAT_EOP    0x02  // End of Packet

// Number of receive descriptors
#define RX_DESC_COUNT         32
#define TX_DESC_COUNT         32
#define RX_BUFFER_SIZE        2048
struct rx_desc {
    volatile uint64_t addr;       // Buffer address
    volatile uint16_t length;     // Length of received data
    volatile uint16_t checksum;   // Checksum
    volatile uint8_t  status;     // Descriptor status
    volatile uint8_t  errors;     // Descriptor errors
    volatile uint16_t special;    // Special field
} __attribute__((packed));

void memset(void* ptr,int val,int len){
  char* p = (char*)ptr;
  for(int i = 0; i < len; i++){
    p[i] = val;
  }
}
// Transmit descriptor structure
struct tx_desc {
    volatile uint64_t addr;       // Buffer address
    volatile uint16_t length;     // Data length
    volatile uint8_t  cso;        // Checksum offset
    volatile uint8_t  cmd;        // Command
    volatile uint8_t  status;     // Status
    volatile uint8_t  css;        // Checksum start
    volatile uint16_t special;    // Special field
} __attribute__((packed));

// Driver data structure
typedef struct {
    pci_device pci;               // PCI device info
    uint32_t mmio_base;           // Memory-mapped I/O base address
    struct rx_desc *rx_descs;     // Receive descriptors
    struct tx_desc *tx_descs;     // Transmit descriptors
    uint8_t *rx_buffers[RX_DESC_COUNT]; // Receive buffers
    uint8_t *tx_buffers[TX_DESC_COUNT]; // Transmit buffers
    uint32_t rx_cur;              // Current RX descriptor index
    uint32_t tx_cur;              // Current TX descriptor index
} e1000_device;

uint32_t scan_mmio_base(pci_device* network){
  // read bar 0 
  
  uint32_t bar0 = pciConfigReadWord(network->bus, network->slot, network->func, 0x10);
  //BAR0 will either be a 64-bit or 32-bit MMIO address (checked by testing bits 2:1 to see if it's 00b (32-bit) or 10b (64-bit)) that points to the device's base register space. BAR0 should always be used to interface with the device via MMIO as the BAR number never changes in different devices in the series.
  //There is also a BAR that will contain an I/O base address, this can be detected by looking at each BAR and testing bit 1. Documentation states this will be in either BAR2 or BAR4, but emulators may move it.
  // if(bar0 == NULL){
  /*  put_string("BAR0 is NULL \n");*/
  /*  return 0;*/
  /*}*/
  if((bar0 & 0b11) == 0b00){
    put_string("BAR0 is 32 bit \n");
    uint32_t mmio_base = bar0 & ~0x3;
    char buffer[16];
    int_to_hex_string(mmio_base, buffer, sizeof(buffer));
    put_string("MMIO base: ");
    put_string(buffer);
    put_string("\n");
    return mmio_base;
  } else if((bar0 & 0b11) == 0b10){
    put_string("BAR0 is 64 bit \n");
    uint32_t mmio_base = bar0 & ~0x3;
    char buffer[16];
    int_to_hex_string(mmio_base, buffer, sizeof(buffer));
    put_string("MMIO base: ");
    put_string(buffer);
    put_string("\n");
    return mmio_base;
  } else {
    put_string("Unknown BAR type \n");
  }
}

static inline uint32_t e1000_read_reg(e1000_device *dev, uint32_t reg) {
    volatile uint32_t *addr = (volatile uint32_t*)(dev->mmio_base + reg);
    return *addr;
}

// Function to write to the MMIO region
static inline void e1000_write_reg(e1000_device *dev, uint32_t reg, uint32_t value) {
    volatile uint32_t *addr = (volatile uint32_t*)(dev->mmio_base + reg);
    *addr = value;
}
void wait_for_reset(e1000_device *dev) {
    // Wait for the device to reset
    for(int i = 0; i < 1000000; i++) {
        // Read the control register
      }
    uint32_t ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
    // if not reset call recursive
    put_string("Checking if reset. \n");
    if (ctrl & E1000_CTRL_RST) {
      e1000_write_reg(dev, E1000_REG_CTRL, ctrl | E1000_CTRL_RST);
      wait_for_reset(dev);
    }
    put_string("Reset already \n");

  /*uint32_t ctrl;*/
    /*do {*/
    /*    ctrl = e1000_read_reg(dev, E1000_REG_CTRL);*/
    /*} while (ctrl & E1000_CTRL_RST);*/
}
void e1000_delay(int ms) {
    // Simple delay loop
    for (volatile int i = 0; i < ms * 10000; i++);
}
void e1000_read_mac_address(e1000_device *dev) {
    uint32_t mac_low = e1000_read_reg(dev, E1000_REG_EEPROM);
    uint32_t mac_high = e1000_read_reg(dev, E1000_REG_CTRL_EXT);
    put_string("MAC address: ");
    char buffer[16];
    int_to_hex_string(mac_low, buffer, sizeof(buffer));
    put_string(buffer);
    put_string(":");
    int_to_hex_string(mac_high, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
}
void enable_network_card(pci_device network){
    put_string("Enabling network card \n");
    put_string("Vendor: ");
    char buffer[16];
    int_to_hex_string(network.vendor, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
    put_string("Device: ");
    int_to_hex_string(network.device, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
    /*put_string("Function: ");*/
    /*int_to_hex_string(network.func, buffer, sizeof(buffer));*/
    /*put_string(buffer);*/
    put_string("\n");
    put_string("Bus: ");
    int_to_hex_string(network.bus, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
    put_string("Slot: ");
    int_to_hex_string(network.slot, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
    e1000_device *dev = kmalloc(sizeof(e1000_device));
    memset(dev, 0, sizeof(e1000_device));
    dev->pci = network;
    dev->mmio_base = scan_mmio_base(&network);
    put_string("Reseting the device \n");
    uint32_t ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
    e1000_write_reg(dev, E1000_REG_CTRL, ctrl | E1000_CTRL_RST);
    
    // Wait a bit for reset to take effect
    e1000_delay(20);
    
    // Clear pending interrupts
    uint32_t icr = e1000_read_reg(dev, E1000_REG_ICR);
    
    // Check if reset bit cleared itself - if not, try PHY reset
    ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
    if (ctrl & E1000_CTRL_RST) {
        put_string("Primary reset failed, trying PHY reset\n");
        e1000_write_reg(dev, E1000_REG_CTRL, ctrl | E1000_CTRL_PHY_RST);
        e1000_delay(10);
    }
    
    // Final check and configuration
    ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
    if (ctrl & (E1000_CTRL_RST | E1000_CTRL_PHY_RST)) {
        put_string("Warning: Reset bits still set, proceeding anyway\n");
    } else {
        put_string("Device reset successful\n");
    }
    put_string("Device reset.\n");
    e1000_read_mac_address(dev);
}

void* read_packet(pci_device network, uint8_t* buffer, uint32_t size){
    put_string("Reading packet from network card \n");
    
}
// network card is 8254x
// https://wiki.osdev.org/Intel_8254x
