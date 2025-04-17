#include "rtl.h"
#include "paging.h"
#include "ports.h"
#include "reg.h"
#include "stdlib.h"
#include "vga.h"
#define MAC0 0x00
#define MAR0 0x08
#define RBSTART 0x30
#define CMD 0x37
#define IMR 0x3C
#define ISR 0x3E
#define CONFIG_1_REG 0x52
#define RCR 0x44             // Receive Configuration Register
#define TX_STATUS_OWN 0x2000 // OWN bit - set to 0 when transmission complete
#define TX_STATUS_TOK 0x8000 // Transmit OK status

// Define the transmit descriptor registers
#define TSAD0 0x20 // Transmit Start Address of Descriptor 0
#define TSAD1 0x24 // Transmit Start Address of Descriptor 1
#define TSAD2 0x28 // Transmit Start Address of Descriptor 2
#define TSAD3 0x2C // Transmit Start Address of Descriptor 3

// Define the transmit status registers
#define TSD0 0x10 // Transmit Status of Descriptor 0
#define TSD1 0x14 // Transmit Status of Descriptor 1
#define TSD2 0x18 // Transmit Status of Descriptor 2
#define TSD3 0x1C // Transmit Status of Descriptor 3

// Current transmit descriptor index
static uint8_t tx_cur = 0;

// Pointers for transmit buffers (4 buffers of 1536 bytes each)
static uint8_t *tx_buffer[4];
static uint8_t mac[6];
#define TX_CTRL_SIZE 0x000007FF  // Size of packet (0-1792)
#define TX_CTRL_OWN 0x00002000   // Set by driver, cleared when transmitted
#define TX_CTRL_TUN 0x00004000   // Transmit FIFO underrun
#define TX_CTRL_TOK 0x00008000   // Transmit OK
#define TX_CTRL_EARLY 0x00000080 // Early tx threshold - 8 bytes

// Maximum timeout for waiting (in iterations)
#define MAX_TRANSMIT_TIMEOUT 10000

int rtl8139_send_packet(uint16_t ioaddrt, const void *data, uint16_t length) {
  uint32_t tsd;
  int timeout = 0;

  // Check if length is valid
  if (length > 1536 || length < 14) {
    put_string("Invalid packet length\n");
    return -1;
  }

  // Wait until transmitter is available or timeout occurs
  do {
    tsd = inl(ioaddrt + TSD0 + (tx_cur * 4));
    if (!(tsd & TX_STATUS_OWN)) {
      break; // Transmitter is available
    }
    timeout++;

    // Simple delay - you might want to replace with a better delay mechanism
    for (volatile int i = 0; i < 1000; i++)
      ;

  } while (timeout < MAX_TRANSMIT_TIMEOUT);

  if (timeout >= MAX_TRANSMIT_TIMEOUT) {
    put_string("Transmitter busy - timeout occurred\n");
    return -1;
  }

  // Copy data to the current transmit buffer
  memcpy(tx_buffer[tx_cur], data, length);

  // Set the physical address of the buffer to the descriptor
  outl(ioaddrt + TSAD0 + (tx_cur * 4), (uint32_t)tx_buffer[tx_cur]);

  // Start transmission (set length and control bits)
  // Set EARLY threshold (8 bytes)
  outl(ioaddrt + TSD0 + (tx_cur * 4), (length & TX_CTRL_SIZE) | TX_CTRL_EARLY);

  // Move to the next descriptor
  tx_cur = (tx_cur + 1) % 4;

  put_string("Packet sent successfully\n");
  return 0;
}

// Simple function to create and send a test packet
void rtl8139_send_test_packet(uint16_t ioaddrt) {
  // Create a simple broadcast packet (Ethernet header + some data)
  uint8_t packet[64] = {
      // Destination MAC (Broadcast FF:FF:FF:FF:FF:FF)
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,

      // Source MAC (made up for testing - 52:54:00:12:34:56)
      0x52, 0x54, 0x00, 0x12, 0x34, 0x56,

      // EtherType (0x0800 = IPv4)
      0x08, 0x00,

      // Payload (random data just to fill the packet)
      0x45, 0x00, 0x00, 0x30, 0x12, 0x34, 0x00, 0x00, 0x40, 0x11, 0x00, 0x00,
      0xC0, 0xA8, 0x01, 0x01, 0xC0, 0xA8, 0x01, 0xFF, 0x12, 0x34, 0x43, 0x21,
      0x00, 0x1C, 0x00, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x77,
      0x6F, 0x72, 0x6C, 0x64, 0x21, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};

  // Send the packet
  rtl8139_send_packet(ioaddrt, packet, sizeof(packet));
}
// Initialize transmit buffers using kmalloc
void rtl8139_init_tx(void) {
  // Allocate and initialize each transmit buffer
  for (int i = 0; i < 4; i++) {
    // Allocate 1536 bytes for each buffer
    tx_buffer[i] = (uint8_t *)kmalloc(1536);

    if (tx_buffer[i] == NULL) {
      put_string("Failed to allocate transmit buffer\n");
      // Handle allocation failure
      return;
    }

    // Clear the buffer
    memset(tx_buffer[i], 0, 1536);
  }

  // Reset the current transmit descriptor
  tx_cur = 0;
  put_string("Transmit buffers initialized\n");
}
static uint32_t ioaddr = 0;
// Update your enable_rtl8139 function to include transmit initialization
void rtl8139_send_arp_discover(uint16_t ioaddrt, uint8_t *src_mac,
                               uint32_t src_ip, uint32_t target_ip) {
  uint8_t packet[60] = {
      0}; // ARP packet is small, Ethernet requires at least 60 bytes
  int i = 0;

  // Ethernet Header
  // Destination MAC: Broadcast FF:FF:FF:FF:FF:FF
  for (int j = 0; j < 6; j++)
    packet[i++] = 0xFF;

  // Source MAC
  for (int j = 0; j < 6; j++)
    packet[i++] = src_mac[j];

  // EtherType: ARP = 0x0806
  packet[i++] = 0x08;
  packet[i++] = 0x06;

  // ARP Header
  packet[i++] = 0x00; // Hardware type: Ethernet (1)
  packet[i++] = 0x01;
  packet[i++] = 0x08; // Protocol type: IPv4 (0x0800)
  packet[i++] = 0x00;
  packet[i++] = 6;    // Hardware size (MAC length)
  packet[i++] = 4;    // Protocol size (IPv4 length)
  packet[i++] = 0x00; // Opcode: Request (1)
  packet[i++] = 0x01;

  // Sender MAC
  for (int j = 0; j < 6; j++)
    packet[i++] = src_mac[j];

  // Sender IP
  packet[i++] = (src_ip >> 24) & 0xFF;
  packet[i++] = (src_ip >> 16) & 0xFF;
  packet[i++] = (src_ip >> 8) & 0xFF;
  packet[i++] = src_ip & 0xFF;

  // Target MAC: 00:00:00:00:00:00
  for (int j = 0; j < 6; j++)
    packet[i++] = 0x00;

  // Target IP
  packet[i++] = (target_ip >> 24) & 0xFF;
  packet[i++] = (target_ip >> 16) & 0xFF;
  packet[i++] = (target_ip >> 8) & 0xFF;
  packet[i++] = target_ip & 0xFF;

  // Pad to minimum Ethernet size (60 bytes without CRC)
  while (i < 60)
    packet[i++] = 0x00;

  rtl8139_send_packet(ioaddrt, packet, i);
}

void enable_rtl8139(pci_device device) {
  put_string("Enabling RTL8139\n");

  char buffer[32] = {0};
  int_to_hex_string(device.baseaddr, buffer, 32);
  put_string("Base address: ");
  put_string(buffer);
  put_string("\n");

  ioaddr = device.baseaddr;
  pciConfigWriteByte(device.bus, device.slot, device.func, 0x3C, 10);
  device.irq_num = 10;
  put_string("Forced IRQ to 10\n");
  // Power on
  outb(ioaddr + CONFIG_1_REG, 0x00);

  // Software reset
  outb(ioaddr + CMD, 0x10);
  put_string("RTL8139 reset\n");
  while ((inb(ioaddr + CMD) & 0x10) != 0) {
    // Wait for reset to complete
  }
  put_string("Reset the NIC successfully\n");
  // Allocate receive buffer
  for (int i = 0; i < 6; i++) {
    mac[i] = inb(ioaddr + MAC0 + i);
  }
  put_string("MAC address: ");
  for (int i = 0; i < 6; i++) {
    char buffer[3];
    int_to_hex_string(mac[i], buffer, 3);
    put_string(buffer);
    put_string(":");
  }
  // while(1);
  void *received_buffer = kmalloc(8192 + 16);
  outl(ioaddr + RBSTART, (uintptr_t)received_buffer);
  put_string("Allocated recive buffer");
  // Initialize transmit buffers
  rtl8139_init_tx();
  put_string("Transmit buffers initialized\n");
  // Enable receive and transmit
  outl(ioaddr + RCR, 0x0F | (1 << 7));
  put_string("Receive and transmit enabled\n");
  // Set up interrupts

  outw(ioaddr + IMR, -1); // 0x0005);
  put_string("Interrupts enabled\n");
  // Start the receiver and transmitter
  outb(ioaddr + CMD, 0x0C);
  put_string("Receiver and transmitter started\n");
  int_to_hex_string(device.irq_num, buffer, 32);
  put_string("RTL8139 IRQ: ");
  put_string(buffer);
  put_string("\n");

  put_string("RTL8139 enabled\n");
  // rtl8139_send_test_packet(ioaddr);
  // Send a test packet
  // rtl8139_send_arp_discover(ioaddr, mac, 0, -1);
}
void read_packet(uint16_t addr, uint32_t size) {
  // Read the packet from the receive buffer
  uint8_t *buffer = (uint8_t *)kmalloc(size);
  if (!buffer) {
    put_string("Failed to allocate memory for packet buffer\n");
    return;
  }

  // Copy the packet data from the receive buffer
  for (uint32_t i = 0; i < size; i++) {
    buffer[i] = inb(addr + RBSTART + i);
  }

  put_string("Packet hexdump:\n");

  // Create a hexdump of the packet (first 128 bytes for readability)
  uint32_t dump_size = size > 128 ? 128 : size;
  for (uint32_t i = 0; i < dump_size; i++) {
    // Print offset at the beginning of each line
    if (i % 16 == 0) {
      char offset_str[8];
      int_to_hex_string(i, offset_str, 8);
      put_string("0x");
      put_string(offset_str);
      put_string(": ");
    }

    // Print byte in hex
    char hex_byte[3];
    int_to_hex_string(buffer[i], hex_byte, 3);
    put_string(hex_byte);
    put_string(" ");

    // Print ASCII representation at the end of each line
    if (i % 16 == 15 || i == dump_size - 1) {
      // Pad with spaces if the last line is incomplete
      for (int j = i % 16; j < 15; j++) {
        put_string("   ");
      }

      put_string(" | ");

      // Print ASCII characters (printable only)
      /*for (int j = i - (i % 16); j <= i; j++) {*/
      /*    if (buffer[j] >= 32 && buffer[j] <= 126) {*/
      /*        // Printable character*/
      /*        char c[2] = {buffer[j], 0};*/
      /*        put_string(c);*/
      /*    } else {*/
      /*        // Non-printable character*/
      /*        put_string(".");*/
      /*    }*/
      /*}*/

      put_string("\n");
    }
  }

  // Print packet size
  char size_str[16];
  int_to_hex_string(size, size_str, 16);
  put_string("Total packet size: ");
  put_string(size_str);
  put_string(" bytes\n");

  kfree(buffer);
}
void rtl_handler(registers_t *reg) {
  uint16_t isr = inw(ioaddr + ISR);
  put_string("RTL8139 interrupt occurred\n");
  outw(ioaddr + 0x3E, 0x05);
  if (isr & 0x01) {
    put_string("Received packet\n");

    // Get the current buffer position from the Command Block Index (CAPR)
    uint16_t current_offset = inw(ioaddr + 0x38); // CAPR register

    // Read 4 bytes at the current position to get packet header info
    uint16_t packet_status = inw(ioaddr + RBSTART + current_offset);
    uint16_t packet_length = inw(ioaddr + RBSTART + current_offset + 2);

    // Adjust the length to account for status and length fields
    packet_length -= 4;

    // Check if packet is valid
    if (packet_length > 0 && packet_length < 8192) {
      // Pass the correct address for the packet data
      read_packet(ioaddr + current_offset + 4, packet_length);

      // Update the CAPR register to mark this packet as processed
      // Add 4 for status and length, and 3 for alignment
      uint16_t new_offset = (current_offset + packet_length + 4 + 3) & ~3;
      outw(ioaddr + 0x38, new_offset - 16); // CAPR must be 16 bytes behind
    } else {
      put_string("Invalid packet length\n");
    }
  }

  if (isr & 0x04) {
    put_string("Transmit OK\n");
  }

  // Clear interrupt status
  outw(ioaddr + ISR, isr);
}
