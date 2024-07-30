#include <stdint.h>

int strlen(const char *str) {
  int cnt = 0;
  while (str[cnt] != '\0')
    cnt++;
  return cnt;
}
// not mine, this is maddness look for diffrent way maybe?
// thought we need to read and write the raw ports so idk
// unsigned char port_byte_in(unsigned short port) {
//   unsigned char result;
//   __asm__("in %%dx, %%al" : "=a"(result) : "d"(port));
//   return result;
// }
//
// void outb(unsigned short port, unsigned char data) {
//   __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
// }
//
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define VIDEO_MEMORY 0xb8000

#define CURSOR_PORT_COMMAND 0x3D4
#define CURSOR_PORT_DATA 0x3D5
#define SCREEN_WIDTH 80
#define IDT_ENTRIES 256
typedef struct {
  int x;
  int y;
} Cursor_position;
typedef struct {
  uint32_t ds;
  // general purpose registers pushed by pusha
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  // pushed by isr procedure
  uint32_t int_no, err_code;
  // pushed by CPU automatically
  uint32_t eip, cs, eflags, useresp, ss;
} registers_t;
static inline unsigned char inb(unsigned short port) {
  unsigned char result;
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

static inline void outb(unsigned short port, unsigned char data) {
  __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

Cursor_position get_cursor_position() {
  unsigned short pos = 0;
  Cursor_position cursor;

  outb(CURSOR_PORT_COMMAND, 0x0F);
  pos |= inb(CURSOR_PORT_DATA);
  outb(CURSOR_PORT_COMMAND, 0x0E);
  pos |= ((unsigned short)inb(CURSOR_PORT_DATA)) << 8;

  cursor.x = pos % SCREEN_WIDTH;
  cursor.y = pos / SCREEN_WIDTH;

  return cursor;
}

void set_cursor_position(int x, int y) {
  unsigned short pos = y * SCREEN_WIDTH + x;
  outb(CURSOR_PORT_COMMAND, 0x0F);
  outb(CURSOR_PORT_DATA, (unsigned char)(pos & 0xFF));
  outb(CURSOR_PORT_COMMAND, 0x0E);
  outb(CURSOR_PORT_DATA, (unsigned char)((pos >> 8) & 0xFF));
}

void put_pixel(char val, char col, int x, int y) {
  char final_col = col;
  if (col == '\0')
    final_col = 0x0f;
  unsigned int offset = (y * SCREEN_WIDTH + x) * 2;
  char *video_memory = (char *)VIDEO_MEMORY;
  video_memory[offset] = val;
  video_memory[offset + 1] = final_col;
}
char get_pixel(int x, int y) {
  unsigned int offset = (y * SCREEN_WIDTH + x) * 2;
  char *video_memory = (char *)VIDEO_MEMORY;
  return video_memory[offset];
}
void memcpy(char *dest, const char *src, unsigned int count) {
  for (unsigned int i = 0; i < count; i++) {
    *(dest + i) = *(src + i);
  }
}
void clear_row(int row) {
  char *video_memory = (char *)VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    video_memory[(row * SCREEN_WIDTH + i) * 2] = ' '; // Space character
    video_memory[(row * SCREEN_WIDTH + i) * 2 + 1] =
        0x07; // Light gray on black
  }
}
void scroll_line() {
  char *video_memory = (char *)VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_HEIGHT - 1; i++) {
    memcpy(video_memory + i * SCREEN_WIDTH * 2,
           video_memory + (i + 1) * SCREEN_WIDTH * 2, SCREEN_WIDTH * 2);
  }
  clear_row(SCREEN_HEIGHT - 1);
}
void clear_screen() {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    clear_row(i); // might as well just write raw to buffer but idkk...
  }
}
void put_string(char *string) {
  // This is some shit ass code but its low level....
  Cursor_position cursor = get_cursor_position();
  for (int i = 0; string[i] != '\0'; i++) {
    if (string[i] == '\10') { // Backspace
      cursor.x = 0;
    } else if (string[i] == '\13') { // Vertical tab
      cursor.y++;
    } else {
      put_pixel(string[i], 0x0f, cursor.x, cursor.y);
      cursor.x++;
      if (cursor.x >= SCREEN_WIDTH) {
        cursor.x = 0;
        cursor.y++;
      }
    }

    if (cursor.y >= SCREEN_HEIGHT) {
      scroll_line();
      cursor.y = SCREEN_HEIGHT - 1;
    }

    set_cursor_position(cursor.x, cursor.y);
  }
}
// typedef uint64_t Idt_gate;
extern void isr0();

extern void isr1();

extern void isr2();

extern void isr3();

extern void isr4();

extern void isr5();

extern void isr6();

extern void isr7();

extern void isr8();

extern void isr9();

extern void isr10();

extern void isr11();

extern void isr12();

extern void isr13();

extern void isr14();

extern void isr15();

extern void isr16();

extern void isr17();

extern void isr18();

extern void isr19();

extern void isr20();

extern void isr21();

extern void isr22();

extern void isr23();

extern void isr24();

extern void isr25();

extern void isr26();

extern void isr27();

extern void isr28();

extern void isr29();

extern void isr30();

extern void isr31();

typedef struct {
  uint16_t offset_1;       // offset bits 0..15
  uint16_t selector;       // a code segment selector in GDT or LDT
  uint8_t zero;            // unused, set to 0
  uint8_t type_attributes; // gate type, dpl, and p fields
  uint16_t offset_2;       // offset bits 16..31
} __attribute__((packed)) InterruptDescriptor32;
InterruptDescriptor32 idt[256];
uint16_t lower16(uint32_t ptr) { return (uint16_t)(ptr & 0xFFFF); }
uint16_t high16(uint32_t ptr) { return (uint16_t)((ptr >> 16) & 0xFFFF); }
void add_idt_gate(int number_for_interupt, uint32_t ptr_to_handler) {
  idt[number_for_interupt].offset_1 = lower16(ptr_to_handler);
  idt[number_for_interupt].selector = 0x08;
  idt[number_for_interupt].zero = 0;
  // 0x8E = 1  00 0 1  110
  //        P DPL 0 D Type
  idt[number_for_interupt].type_attributes = 0x8E;
  idt[number_for_interupt].offset_2 = high16(ptr_to_handler);
}
typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idt_register_t;
idt_register_t idt_reg;
extern void irq0();
extern void irq1();
extern void irq7();
extern void irq8();
extern void irq15();
void keyboard_handler(registers_t *r);

void load_idt() {
  idt_reg.base = (uint32_t)&idt;
  idt_reg.limit = IDT_ENTRIES * sizeof(InterruptDescriptor32) - 1;
  asm volatile("lidt (%0)" : : "r"(&idt_reg));
}
void isr_install() {
  add_idt_gate(0, (uint32_t)isr0);
  add_idt_gate(1, (uint32_t)isr1);
  add_idt_gate(2, (uint32_t)isr2);
  add_idt_gate(3, (uint32_t)isr3);
  add_idt_gate(4, (uint32_t)isr4);
  add_idt_gate(5, (uint32_t)isr5);
  add_idt_gate(6, (uint32_t)isr6);
  add_idt_gate(7, (uint32_t)isr7);
  add_idt_gate(8, (uint32_t)isr8);
  add_idt_gate(9, (uint32_t)isr9);
  add_idt_gate(10, (uint32_t)isr10);
  add_idt_gate(11, (uint32_t)isr11);
  add_idt_gate(12, (uint32_t)isr12);
  add_idt_gate(13, (uint32_t)isr13);
  add_idt_gate(14, (uint32_t)isr14);
  add_idt_gate(15, (uint32_t)isr15);
  add_idt_gate(16, (uint32_t)isr16);
  add_idt_gate(17, (uint32_t)isr17);
  add_idt_gate(18, (uint32_t)isr18);
  add_idt_gate(19, (uint32_t)isr19);
  add_idt_gate(20, (uint32_t)isr20);
  add_idt_gate(21, (uint32_t)isr21);
  add_idt_gate(22, (uint32_t)isr22);
  add_idt_gate(23, (uint32_t)isr23);
  add_idt_gate(24, (uint32_t)isr24);
  add_idt_gate(25, (uint32_t)isr25);
  add_idt_gate(26, (uint32_t)isr26);
  add_idt_gate(27, (uint32_t)isr27);
  add_idt_gate(28, (uint32_t)isr28);
  add_idt_gate(29, (uint32_t)isr29);
  add_idt_gate(30, (uint32_t)isr30);
  add_idt_gate(31, (uint32_t)isr31);

  // Remap the PIC
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  // Install the IRQs
  // handle keyboard
  add_idt_gate(32, (uint32_t)irq0);
  add_idt_gate(33, (uint32_t)irq1);
  // add_idt_gate(34, (uint32_t)irq2);
  // add_idt_gate(35, (uint32_t)irq3);
  // add_idt_gate(36, (uint32_t)irq4);
  // add_idt_gate(37, (uint32_t)irq5);
  // add_idt_gate(38, (uint32_t)irq6);
  add_idt_gate(39, (uint32_t)irq7);
  add_idt_gate(40, (uint32_t)irq8);
  // add_idt_gate(41, (uint32_t)irq9);
  // add_idt_gate(42, (uint32_t)irq10);
  // add_idt_gate(43, (uint32_t)irq11);
  // add_idt_gate(44, (uint32_t)irq12);
  // add_idt_gate(45, (uint32_t)irq13);
  // add_idt_gate(46, (uint32_t)irq14);
  add_idt_gate(47, (uint32_t)irq15);
  //
  load_idt(); // Load with ASM
}

char *exception_messages[0x1F + 1] = {"Division By Zero",
                                      "Debug",
                                      "Non Maskable Interrupt",
                                      "Breakpoint",
                                      "Into Detected Overflow",
                                      "Out of Bounds",
                                      "Invalid Opcode",
                                      "No Coprocessor",

                                      "Double Fault",
                                      "Coprocessor Segment Overrun",
                                      "Bad TSS",
                                      "Segment Not Present",
                                      "Stack Fault",
                                      "General Protection Fault",
                                      "Page Fault",
                                      "Unknown Interrupt",

                                      "Coprocessor Fault",
                                      "Alignment Check",
                                      "Machine Check",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",

                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved",
                                      "Reserved"};
void int_to_hex_string(unsigned int number, char *buffer, int buff_size) {
  buffer[buff_size - 1] = '\0';
  int index = buff_size - 2;

  if (number == 0) {
    buffer[index--] = '0';
  } else {
    while (number > 0 && index >= 0) {
      int digit = number % 16;
      if (digit < 10) {
        buffer[index] = '0' + digit;
      } else {
        buffer[index] = 'A' + (digit - 10);
      }
      index--;
      number /= 16;
    }
  }

  if (index >= 0) {
    int shift = index + 1;
    for (int i = 0; i < buff_size - shift; i++) {
      buffer[i] = buffer[i + shift];
    }
  }
}

void keyboard_handler(registers_t *r) {
  uint8_t scancode = inb(0x60);
  // Process the scancode here
  // For now, let's just print it
  put_string("Keyboard scancode: ");
  char s[16];
  int_to_hex_string(scancode, s, 3);
  put_string(s);
  put_string("\10\13");
}
int strcmp(char *c1, char *c2) {
  int p = 0;
  while (c1[p] != '\0' && c2[p] != '\0') {
    if (c1[p] != c2[p++])
      return 0;
  }
  return c1[p] == c2[p] && c2[p] == '\0';
}
// Modify the isr_handler function to include IRQ handling
void isr_handler(registers_t *r) {
  const int BUFF_SIZE = 15;
  char buffer[BUFF_SIZE]; // Increased buffer size for larger hex values

  put_string("Interrupt occurred:\10\13");

  put_string("ds: 0x");
  int_to_hex_string(r->ds, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("edi: 0x");
  int_to_hex_string(r->edi, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", esi: 0x");
  int_to_hex_string(r->esi, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("ebp: 0x");
  int_to_hex_string(r->ebp, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", esp: 0x");
  int_to_hex_string(r->esp, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("ebx: 0x");
  int_to_hex_string(r->ebx, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", edx: 0x");
  int_to_hex_string(r->edx, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("ecx: 0x");
  int_to_hex_string(r->ecx, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", eax: 0x");
  int_to_hex_string(r->eax, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("int_no: 0x");
  int_to_hex_string(r->int_no, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", err_code: 0x");
  int_to_hex_string(r->err_code, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("eip: 0x");
  int_to_hex_string(r->eip, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", cs: 0x");
  int_to_hex_string(r->cs, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("eflags: 0x");
  int_to_hex_string(r->eflags, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  put_string("useresp: 0x");
  int_to_hex_string(r->useresp, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string(", ss: 0x");
  int_to_hex_string(r->ss, buffer, BUFF_SIZE);
  put_string(buffer);
  put_string("\10\13");

  if (r->int_no >= 32 && r->int_no < 48) {
    // Handle IRQs
    put_string("Not an exception \10\13");
    if (r->int_no == 33) {
      put_string("Clock!\10\13");
    }
    if (r->int_no == 34) {
      keyboard_handler(r);
    }
    // Send EOI (End of Interrupt) signal to PICs
    if (r->int_no >= 40) {
      outb(0xA0, 0x20); // Send EOI to slave PIC
    }
    outb(0x20, 0x20); // Send EOI to master PIC
  } else {
    // Handle exceptions
    put_string("Exception: ");
    char *err = exception_messages[r->err_code];
    // char bf[2];
    // for (int i = 0; i < 32; i++) {
    //   put_string("Exception_: ");
    //   put_string(exception_messages[i]);
    //   put_string("\10\13");
    // }
    // int_to_hex_string(strcmp(exception_messages[0], "S\0"), bf, 2);
    // put_string(bf);
    put_string(" ");
    put_string(err);
    put_string("\10\13");
    // while (1) {
    //   // Halt the system
    // }
  }
}

// taken from
// https://wiki.osdev.org/Interrupt_Descriptor_Table

void main() {
  char *video_memory = (char *)0xb8000;
  // char message[] = {'H', 'e', 'l', 'l', 'o', ' ',
  //   'W', 'o', 'r', 'l', 'd', '\0'
  // };
  // const char *message = "Hello World";
  char message[] = "Hello World";

  char mess2[] = "He\10\13Lllo\10a\10\13NewLine\10\13";
  put_string(mess2);
  put_string(message);
  put_string(message);
  put_string(message);
  char print[4] = "a\10\13";
  isr_install();
  char done[] = {'F', 'I', 'N', 'I', 'S', 'H', 'E', 'D', 0};
  put_string(done);
  put_string(exception_messages[0]);
  int a = 1 / 0;
  // irq1();
  while (1) {
    // for (char i = 'a'; i <= 'z'; i++) {
    //   print[0] = i;
    //   put_string(print);
    // }
    // for (int j = 0; j < 100; j++) {
    //   for (int i = 0; i < 2000000; i++) {
    //   }
    // }
    // clear_screen();
  }
  *(video_memory + 100) = 'V';
  *(video_memory + 101) = 0x0f;
}
