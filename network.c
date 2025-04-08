#include "network.h"
#include "paging.h"
#include "stdlib.h"
#include "vga.h"

// Intel 82540EM (E1000) specific register offsets
#define E1000_REG_CTRL 0x0000     // Control Register
#define E1000_REG_STATUS 0x0008   // Status Register
#define E1000_REG_EEPROM 0x0014   // EEPROM Read Register
#define E1000_REG_CTRL_EXT 0x0018 // Extended Control Register
#define E1000_REG_MDIC 0x0020     // MDI Control Register
#define E1000_REG_RCTL 0x0100     // Receive Control Register
#define E1000_REG_TCTL 0x0400     // Transmit Control Register
#define E1000_REG_RDBAL 0x2800    // RX Descriptor Base Address Low
#define E1000_REG_RDBAH 0x2804    // RX Descriptor Base Address High
#define E1000_REG_RDLEN 0x2808    // RX Descriptor Length
#define E1000_REG_RDH 0x2810      // RX Descriptor Head
#define E1000_REG_RDT 0x2818      // RX Descriptor Tail
#define E1000_REG_TDBAL 0x3800    // TX Descriptor Base Address Low
#define E1000_REG_TDBAH 0x3804    // TX Descriptor Base Address High
#define E1000_REG_TDLEN 0x3808    // TX Descriptor Length
#define E1000_REG_TDH 0x3810      // TX Descriptor Head
#define E1000_REG_TDT 0x3818      // TX Descriptor Tail
#define E1000_REG_ICR 0x00C0      // Interrupt Cause Read
// Control register bits
#define E1000_CTRL_FD 0x00000001      // Full duplex
#define E1000_CTRL_ASDE 0x00000020    // Auto-speed detection enable
#define E1000_CTRL_SLU 0x00000040     // Set link up
#define E1000_CTRL_ILOS 0x00000080    // Invert Loss-of-Signal
#define E1000_CTRL_RST 0x04000000     // Reset
#define E1000_CTRL_PHY_RST 0x80000000 // PHY Reset
// Receive Control Register bits
#define E1000_RCTL_EN 0x00000002         // Receiver Enable
#define E1000_RCTL_SBP 0x00000004        // Store Bad Packets
#define E1000_RCTL_UPE 0x00000008        // Unicast Promiscuous Mode
#define E1000_RCTL_MPE 0x00000010        // Multicast Promiscuous Mode
#define E1000_RCTL_LBM_NONE 0x00000000   // No loopback
#define E1000_RCTL_BSIZE_2048 0x00000000 // Buffer size 2048
#define E1000_RCTL_SECRC 0x04000000      // Strip Ethernet CRC

// Transmit Control Register bits
#define E1000_TCTL_EN 0x00000002   // Transmit Enable
#define E1000_TCTL_PSP 0x00000008  // Pad Short Packets
#define E1000_TCTL_CT 0x00000100   // Collision Threshold
#define E1000_TCTL_COLD 0x00040000 // Collision Distance

// Descriptor status bits
#define E1000_RXD_STAT_DD 0x01  // Descriptor Done
#define E1000_RXD_STAT_EOP 0x02 // End of Packet

// Number of receive descriptors
#define RX_DESC_COUNT 32
#define TX_DESC_COUNT 32
#define RX_BUFFER_SIZE 2048
struct rx_desc {
  volatile uint64_t addr;     // Buffer address
  volatile uint16_t length;   // Length of received data
  volatile uint16_t checksum; // Checksum
  volatile uint8_t status;    // Descriptor status
  volatile uint8_t errors;    // Descriptor errors
  volatile uint16_t special;  // Special field
} __attribute__((packed));

/*void memset(void *ptr, int val, int len) {*/
/*  char *p = (char *)ptr;*/
/*  for (int i = 0; i < len; i++) {*/
/*    p[i] = val;*/
/*  }*/
/*}*/
static char mac[6];
// Transmit descriptor structure
struct tx_desc {
  volatile uint64_t addr;    // Buffer address
  volatile uint16_t length;  // Data length
  volatile uint8_t cso;      // Checksum offset
  volatile uint8_t cmd;      // Command
  volatile uint8_t status;   // Status
  volatile uint8_t css;      // Checksum start
  volatile uint16_t special; // Special field
} __attribute__((packed));

// Driver data structure
typedef struct {
  pci_device pci;                     // PCI device info
  uint32_t mmio_base;                 // Memory-mapped I/O base address
  struct rx_desc *rx_descs;           // Receive descriptors
  struct tx_desc *tx_descs;           // Transmit descriptors
  uint8_t *rx_buffers[RX_DESC_COUNT]; // Receive buffers
  uint8_t *tx_buffers[TX_DESC_COUNT]; // Transmit buffers
  uint32_t rx_cur;                    // Current RX descriptor index
  uint32_t tx_cur;                    // Current TX descriptor index
} e1000_device;

uint32_t scan_mmio_base(pci_device *network) {
  // read bar 0

  uint32_t bar0 =
      pciConfigReadWord(network->bus, network->slot, network->func, 0x10);
  // BAR0 will either be a 64-bit or 32-bit MMIO address (checked by testing
  // bits 2:1 to see if it's 00b (32-bit) or 10b (64-bit)) that points to the
  // device's base register space. BAR0 should always be used to interface with
  // the device via MMIO as the BAR number never changes in different devices in
  // the series. There is also a BAR that will contain an I/O base address, this
  // can be detected by looking at each BAR and testing bit 1. Documentation
  // states this will be in either BAR2 or BAR4, but emulators may move it.
  //  if(bar0 == NULL){
  /*  put_string("BAR0 is NULL \n");*/
  /*  return 0;*/
  /*}*/
  if ((bar0 & 0b11) == 0b00) {
    put_string("BAR0 is 32 bit \n");
    uint32_t mmio_base = bar0 & ~0x3;
    char buffer[16];
    int_to_hex_string(mmio_base, buffer, sizeof(buffer));
    put_string("MMIO base: ");
    put_string(buffer);
    put_string("\n");
    return mmio_base;
  } else if ((bar0 & 0b11) == 0b10) {
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
    return NULL;
  }
}

static inline uint32_t e1000_read_reg(e1000_device *dev, uint32_t reg) {
  volatile uint32_t *addr = (volatile uint32_t *)(dev->mmio_base + reg);
  return *addr;
}

// Function to write to the MMIO region
static inline void e1000_write_reg(e1000_device *dev, uint32_t reg,
                                   uint32_t value) {
  volatile uint32_t *addr = (volatile uint32_t *)(dev->mmio_base + reg);
  *addr = value;
}
void wait_for_reset(e1000_device *dev) {
  // Wait for the device to reset
  for (int i = 0; i < 1000000; i++) {
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
  for (volatile int i = 0; i < ms * 10000; i++)
    ;
}
void byte_to_hex_string(uint8_t byte, char *buffer) {
  const char hex_chars[] = "0123456789ABCDEF";
  buffer[0] = hex_chars[(byte >> 4) & 0x0F];
  buffer[1] = hex_chars[byte & 0x0F];
  buffer[2] = '\0'; // Null-terminate the string
}
char *e1000_read_mac_address(e1000_device *dev) {
  // RAL at offset 0x5400, RAH at offset 0x5404
  uint32_t mac_low = e1000_read_reg(dev, 0x5400);
  uint32_t mac_high = e1000_read_reg(dev, 0x5404);

  mac[0] = mac_low & 0xFF;
  mac[1] = (mac_low >> 8) & 0xFF;
  mac[2] = (mac_low >> 16) & 0xFF;
  mac[3] = (mac_low >> 24) & 0xFF;
  mac[4] = mac_high & 0xFF;
  mac[5] = (mac_high >> 8) & 0xFF;

  put_string("MAC address: ");
  for (int i = 0; i < 6; i++) {
    char buffer[3];
    byte_to_hex_string(mac[i], buffer);
    put_string(buffer);
    if (i < 5)
      put_string(":");
  }
  put_string("\n");
  return mac;
}
void e1000_init_rx_descriptors(e1000_device *dev) {
  // A descriptor ring is a circular buffer of descriptors that the network card
  // uses to communicate with the driver. Each descriptor contains information
  // about a buffer in memory where the card can place incoming packets.

  // Allocate memory for RX descriptors - this is a contiguous region of memory
  // that will hold our array of rx_desc structures
  dev->rx_descs = kmalloc(RX_DESC_COUNT * sizeof(struct rx_desc));
  memset(dev->rx_descs, 0, RX_DESC_COUNT * sizeof(struct rx_desc));

  put_string("RX descriptors allocated\n");

  // Allocate buffers for each descriptor
  // Each descriptor needs its own buffer to store received packet data
  for (int i = 0; i < RX_DESC_COUNT; i++) {
    // Allocate a buffer for this descriptor
    dev->rx_buffers[i] = kmalloc(RX_BUFFER_SIZE);

    // Point the descriptor to this buffer
    dev->rx_descs[i].addr = (uint64_t)dev->rx_buffers[i];

    // Clear status - will be set by the card when a packet is received
    dev->rx_descs[i].status = 0;
  }

  put_string("RX buffers allocated\n");

  // Setup descriptor registers - tell the card where our descriptor ring is
  // located The hardware needs to know the physical address of our descriptor
  // ring
  e1000_write_reg(dev, E1000_REG_RDBAL, (uint32_t)dev->rx_descs);
  e1000_write_reg(dev, E1000_REG_RDBAH, 0); // Assuming 32-bit addressing

  // Tell the card how big our descriptor ring is
  e1000_write_reg(dev, E1000_REG_RDLEN, RX_DESC_COUNT * sizeof(struct rx_desc));

  put_string("RX descriptor base and length set\n");

  // Initialize head and tail pointers
  // Head - where the card is currently processing
  // Tail - where the driver has processed up to
  e1000_write_reg(dev, E1000_REG_RDH, 0);
  e1000_write_reg(dev, E1000_REG_RDT, RX_DESC_COUNT - 1);

  // Initialize our own tracking variable
  dev->rx_cur = 0;

  put_string("RX head and tail initialized\n");

  // Enable receiver with appropriate settings
  // - EN: Enable the receiver
  // - SBP: Store Bad Packets (for debugging)
  // - UPE: Accept unicast packets addressed to us
  // - MPE: Accept multicast packets
  uint32_t rctl = e1000_read_reg(dev, E1000_REG_RCTL);
  rctl |= E1000_RCTL_EN | E1000_RCTL_SBP | E1000_RCTL_UPE | E1000_RCTL_MPE;
  rctl |= E1000_RCTL_BSIZE_2048; // Set buffer size
  e1000_write_reg(dev, E1000_REG_RCTL, rctl);

  put_string("RX enabled\n");
}

// Initialize transmit descriptor ring
void e1000_init_tx_descriptors(e1000_device *dev) {
  // The transmit descriptor ring works similarly to the receive ring
  // but is used for sending packets out rather than receiving them

  // Allocate memory for TX descriptors
  dev->tx_descs = kmalloc(TX_DESC_COUNT * sizeof(struct tx_desc));
  memset(dev->tx_descs, 0, TX_DESC_COUNT * sizeof(struct tx_desc));

  put_string("TX descriptors allocated\n");

  // Allocate buffers for each descriptor
  // These buffers will hold outgoing packet data
  for (int i = 0; i < TX_DESC_COUNT; i++) {
    dev->tx_buffers[i] = kmalloc(RX_BUFFER_SIZE); // Use same buffer size
    dev->tx_descs[i].addr = (uint64_t)dev->tx_buffers[i];
    dev->tx_descs[i].status = 0x1;
    // No cmd bits set - will be set when we need to transmit
  }

  put_string("TX buffers allocated\n");

  // Setup descriptor registers
  e1000_write_reg(dev, E1000_REG_TDBAL, (uint32_t)dev->tx_descs);
  e1000_write_reg(dev, E1000_REG_TDBAH, 0); // Assuming 32-bit addressing
  e1000_write_reg(dev, E1000_REG_TDLEN, TX_DESC_COUNT * sizeof(struct tx_desc));

  put_string("TX descriptor base and length set\n");

  // Initialize head and tail pointers
  e1000_write_reg(dev, E1000_REG_TDH, 0);
  e1000_write_reg(dev, E1000_REG_TDT, 0);

  // Initialize our own tracking variable
  dev->tx_cur = 0;

  put_string("TX head and tail initialized\n");

  // Setup transmit control register
  // - EN: Enable transmitter
  // - PSP: Pad Short Packets (to minimum Ethernet frame size)
  // - CT/COLD: Collision settings - important for half-duplex operation
  uint32_t tctl = e1000_read_reg(dev, E1000_REG_TCTL);
  tctl |= E1000_TCTL_EN | E1000_TCTL_PSP;
  tctl |= (0x10 << 4);  // CT - Collision Threshold
  tctl |= (0x40 << 12); // COLD - Collision Distance for half-duplex
  e1000_write_reg(dev, E1000_REG_TCTL, tctl);

  put_string("TX enabled\n");
}

// Function to check for and receive packets
int e1000_receive_packet(e1000_device *dev, uint8_t *buffer,
                         uint32_t max_size) {
  // Get the current descriptor we're examining
  uint32_t rx_cur = dev->rx_cur;
  struct rx_desc *desc = &dev->rx_descs[rx_cur];

  // Check if a packet is available (DD = Descriptor Done bit)
  if (!(desc->status & E1000_RXD_STAT_DD)) {
    return 0; // No packet available
  }

  // Check if this is the end of a packet (EOP = End of Packet bit)
  if (!(desc->status & E1000_RXD_STAT_EOP)) {
    put_string("Warning: Fragmented packet received\n");
    // In a full implementation, you'd handle packet fragments here
  }

  // Copy the packet data from the descriptor buffer to the caller's buffer
  uint16_t length = desc->length;
  if (length > max_size) {
    put_string("Warning: Packet truncated\n");
    length = max_size;
  }

  // Copy the packet data
  for (uint16_t i = 0; i < length; i++) {
    buffer[i] = dev->rx_buffers[rx_cur][i];
  }

  // Reset the descriptor for reuse
  desc->status = 0;

  // Update our position in the ring
  dev->rx_cur = (rx_cur + 1) % RX_DESC_COUNT;

  // Update the tail register to let the card know we processed this descriptor
  e1000_write_reg(dev, E1000_REG_RDT, rx_cur);

  // Return the length of the packet we received
  return length;
}

// Function to send a packet
int e1000_send_packet(e1000_device *dev, uint8_t *data, uint32_t length) {
  // Get the current descriptor to use for transmission
  uint32_t tx_cur = dev->tx_cur;
  struct tx_desc *desc = &dev->tx_descs[tx_cur];

  // Check if the descriptor is available (DD = Descriptor Done)
  if (!(desc->status & 0x1)) {
    // If we were doing this properly, we would wait or queue the packet
    put_string("TX descriptor not available\n");
    return -1;
  }

  // Copy the packet data to the descriptor's buffer
  for (uint32_t i = 0; i < length; i++) {
    dev->tx_buffers[tx_cur][i] = data[i];
  }

  // Setup the descriptor
  desc->length = length;
  desc->cmd = 0x08 | 0x01; // EOP (End of Packet) | RS (Report Status)
  desc->status = 0;

  // Move to the next descriptor
  dev->tx_cur = (tx_cur + 1) % TX_DESC_COUNT;

  // Tell the card there's a new packet to transmit
  e1000_write_reg(dev, E1000_REG_TDT, dev->tx_cur);

  // Return success
  return 0;
}
void enable_network_card(pci_device network) {
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
  uint32_t ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
  // Read back the control register
  ctrl = e1000_read_reg(dev, E1000_REG_CTRL);

  // Check if reset was successful
  if (ctrl & E1000_CTRL_RST) {
    put_string("Primary reset failed, trying PHY reset\n");
    e1000_write_reg(dev, E1000_REG_CTRL, E1000_CTRL_PHY_RST);
    e1000_delay(10);
  }

  // Final check
  ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
  if (ctrl & (E1000_CTRL_RST | E1000_CTRL_PHY_RST)) {
    put_string("Warning: Reset bits still set, proceeding anyway\n");
  } else {
    put_string("Device reset successful\n");
  }
  put_string("Device reset.\n");
  e1000_read_mac_address(dev);
  ctrl = e1000_read_reg(dev, E1000_REG_CTRL);
  put_string("READ CTRL\n");
  ctrl |= E1000_CTRL_SLU; // Set Link Up
  e1000_write_reg(dev, E1000_REG_CTRL, ctrl);
  put_string("Wrote SLU\n");
  e1000_init_rx_descriptors(dev);
  // Initialize descriptor rings

  e1000_init_tx_descriptors(dev);
  put_string("Network card fully initialized\n");
  return;
  // now starting DHCP
  // DHCP GOES BY DORA
  // Discover, Offer, Request, Acknowledge
  // DISCOVER - putting a broadcast packet on the network
  // First, let's create a DHCP discover packet
  char *pack = kmalloc(2048);

  memset(pack, 0, 2048);

  // --- Ethernet Header ---
  // Destination MAC (broadcast FF:FF:FF:FF:FF:FF)
  pack[0] = 0xFF;
  pack[1] = 0xFF;
  pack[2] = 0xFF;
  pack[3] = 0xFF;
  pack[4] = 0xFF;
  pack[5] = 0xFF;

  // Source MAC (use the device's MAC address)
  pack[6] = mac[0];
  pack[7] = mac[1];
  pack[8] = mac[2];
  pack[9] = mac[3];
  pack[10] = mac[4];
  pack[11] = mac[5];

  // Ethertype (0x0800 for IPv4)
  pack[12] = 0x08;
  pack[13] = 0x00;

  // --- IP Header (starting at offset 14) ---
  pack[14] = 0x45; // Version (4) and IHL (5)
  pack[15] = 0x00; // DSCP and ECN
  pack[16] = 0x01;
  pack[17] = 0x48; // Total length (328 bytes)
  pack[18] = 0x00;
  pack[19] = 0x00; // Identification
  pack[20] = 0x00;
  pack[21] = 0x00; // Flags and Fragment offset
  pack[22] = 0x80; // TTL (128)
  pack[23] = 0x11; // Protocol (17 for UDP)
  pack[24] = 0x00;
  pack[25] = 0x00; // Header checksum (to be calculated)
  pack[26] = 0x00;
  pack[27] = 0x00; // Source IP (0.0.0.0)
  pack[28] = 0x00;
  pack[29] = 0x00;
  pack[30] = 0xFF;
  pack[31] = 0xFF; // Destination IP (255.255.255.255)
  pack[32] = 0xFF;
  pack[33] = 0xFF;

  // --- UDP Header (starting at offset 34) ---
  pack[34] = 0x00;
  pack[35] = 0x44; // Source port (68)
  pack[36] = 0x00;
  pack[37] = 0x43; // Destination port (67)
  pack[38] = 0x01;
  pack[39] = 0x34; // Length (308 bytes)
  pack[40] = 0x00;
  pack[41] = 0x00; // Checksum (to be calculated)

  // --- DHCP Message (starting at offset 42) ---
  pack[42] = 0x01; // Op (1 = BOOTREQUEST)
  pack[43] = 0x01; // Hardware type (1 = Ethernet)
  pack[44] = 0x06; // Hardware address length (6 bytes)
  pack[45] = 0x00; // Hops

  // Transaction ID (random)
  pack[46] = 0x12;
  pack[47] = 0x34;
  pack[48] = 0x56;
  pack[49] = 0x78;

  // Seconds elapsed
  pack[50] = 0x00;
  pack[51] = 0x00;

  // Flags (broadcast)
  pack[52] = 0x80;
  pack[53] = 0x00;

  // Client IP address (0.0.0.0)
  pack[54] = 0x00;
  pack[55] = 0x00;
  pack[56] = 0x00;
  pack[57] = 0x00;

  // Your IP address (0.0.0.0)
  pack[58] = 0x00;
  pack[59] = 0x00;
  pack[60] = 0x00;
  pack[61] = 0x00;

  // Server IP address (0.0.0.0)
  pack[62] = 0x00;
  pack[63] = 0x00;
  pack[64] = 0x00;
  pack[65] = 0x00;

  // Gateway IP address (0.0.0.0)
  pack[66] = 0x00;
  pack[67] = 0x00;
  pack[68] = 0x00;
  pack[69] = 0x00;

  // Client hardware address (MAC)
  pack[70] = mac[0];
  pack[71] = mac[1];
  pack[72] = mac[2];
  pack[73] = mac[3];
  pack[74] = mac[4];
  pack[75] = mac[5];
  // Padding (10 bytes)
  // Already zeroed by memset

  // Server host name (64 bytes) - leave as zeros
  // Boot file name (128 bytes) - leave as zeros

  // Magic cookie (DHCP)
  pack[236] = 0x63;
  pack[237] = 0x82;
  pack[238] = 0x53;
  pack[239] = 0x63;

  // DHCP Message Type option
  pack[240] = 0x35; // Option 53: DHCP Message Type
  pack[241] = 0x01; // Length
  pack[242] = 0x01; // DHCP Discover

  // Client Identifier
  pack[243] = 0x3D; // Option 61: Client Identifier
  pack[244] = 0x07; // Length
  pack[245] = 0x01; // Hardware type (1 = Ethernet)
  pack[246] = mac[0];
  pack[247] = mac[1];
  pack[248] = mac[2];
  pack[249] = mac[3];
  pack[250] = mac[4];
  pack[251] = mac[5];

  // Parameter Request List
  pack[252] = 0x37; // Option 55: Parameter Request List
  pack[253] = 0x04; // Length
  pack[254] = 0x01; // Subnet Mask
  pack[255] = 0x03; // Router
  pack[256] = 0x06; // Domain Name Server
  pack[257] = 0x0F; // Domain Name

  // Host Name
  pack[258] = 0x0C; // Option 12: Host Name
  pack[259] = 0x09; // Length
  pack[260] = 'M';
  pack[261] = 'Y';
  pack[262] = 'O';
  pack[263] = 'S';
  pack[264] = 'K';
  pack[265] = 'E';
  pack[266] = 'R';
  pack[267] = 'N';
  pack[268] = 'L'; // "MYOSKERNL"

  // End option
  pack[269] = 0xFF; // Option 255: End

  // Calculate the packet size (270 bytes total)
  int packet_size = 270;

  // Now send the packet
  e1000_send_packet(dev, pack, packet_size);
  put_string("Sent the discover packet\n");
  // Wait for a response
  // This is a blocking call, in a real implementation we would use interrupts
  uint8_t response[2048];
  int response_size;
  for (int j = 0; j < 100; j++) {
    for (int i = 0; i < 100000000; i++) {
      // put_string("Waiting for a response \n");
    }
    response_size = e1000_receive_packet(dev, response, sizeof(response));
    if (response_size == 0)
      put_string("No response yet \n");
  }
  if (response_size > 0) {
    put_string("Received a response\n");
    // Process the response here
    // For now, just print the size of the response
    char buffer[16];
    int_to_hex_string(response_size, buffer, sizeof(buffer));
    put_string("Response size: ");
    put_string(buffer);
    put_string("\n");
  } else {
    put_string("No response received\n");
  }
}

/*void *read_packet(pci_device network, uint8_t *buffer, uint32_t size) {*/
/*  put_string("Reading packet from network card \n");*/
/*}*/
// network card is 8254x
// https://wiki.osdev.org/Intel_8254x
