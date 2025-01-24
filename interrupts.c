#include "interrupts.h"
#include "ports.h"
#include "stdlib.h"
#include "vga.h"
#include <stdint.h>
InterruptDescriptor32 idt[256];
idt_register_t idt_reg;
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

void keyboard_handler(registers_t *r);

void load_idt() {
  idt_reg.base = (uint32_t)&idt;
  idt_reg.limit = IDT_ENTRIES * sizeof(InterruptDescriptor32) - 1;
  asm volatile("lidt (%0)" : : "r"(&idt_reg));
}
// typedef void (*interupt_handler)();
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
  add_idt_gate(34, (uint32_t)irq2);
  add_idt_gate(35, (uint32_t)irq3);
  add_idt_gate(36, (uint32_t)irq4);
  add_idt_gate(37, (uint32_t)irq5);
  add_idt_gate(38, (uint32_t)irq6);
  add_idt_gate(39, (uint32_t)irq7);
  add_idt_gate(40, (uint32_t)irq8);
  add_idt_gate(41, (uint32_t)irq9);
  add_idt_gate(42, (uint32_t)irq10);
  add_idt_gate(43, (uint32_t)irq11);
  add_idt_gate(44, (uint32_t)irq12);
  add_idt_gate(45, (uint32_t)irq13);
  add_idt_gate(46, (uint32_t)irq14);
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

void handle_clock_pulse() {
  static float time = 0;
  // each pulse should add 54.9 ms
  // if a full minute was passed we priotn
  const int interval = (1000 * 60);
  // put_string("Interval \n");
  int prevM = time / interval;
  time += 54.9;
  int currM = time / interval;
  if (currM > prevM && SHOW_TIME) {
    put_string("Another minute\n");
  } else if (prevM > currM) {
    put_string("OMG! time travel. (jk its overflow)\n");
  }
}
void keyboard_handler(registers_t *r) {
  uint8_t scancode = inb(0x60);
  // char s[16];
  // int_to_hex_string(scancode, s, 16);
  //  put_string(s);
  //  put_string("\10\13");

  const char *letter_from_code[KEY_AMOUNT] = {
      "ERROR",     "ESCAPE", "1",        "2",  "3",  "4",      "5",
      "6",         "7",      "8",        "9",  "0",  "-",      "=",
      "BACKSPACE", "TAB",    "q",        "w",  "e",  "r",      "t",
      "y",         "u",      "i",        "o",  "p",  "[",      "]",
      "ENTER",     "LCTRL",  "a",        "s",  "d",  "f",      "g",
      "h",         "j",      "k",        "l",  ";",  "'",      "`",
      "LSHIFT",    "\\",     "z",        "x",  "c",  "v",      "b",
      "n",         "m",      ",",        ".",  "/",  "RSHIFT", "KEYPAD *",
      "LALT",      " ",      "CAPSLOCK", "F1", "F2", "F3",     "F4",
      "F5"};
  // TODO: add arrow handling later
  //  put_string("Key: ");
  if (scancode < KEY_AMOUNT) {
    /*     put_string(" (pressed)"); */
    shell_handle(letter_from_code[scancode]);
  } else if (scancode >= 0x80 && scancode < 0x80 + KEY_AMOUNT) {
    // put_string(letter_from_code[scancode - 0x80]);
    // put_string(" (released)");
  } else {
    put_string("Unknown");
  }
  // put_string("\10\13");
}
void isr_handler(registers_t *r) {
  // Increased buffer size for larger hex values
  // put_string("Interrupt occurred:\10\13");
  //
  // print_state(r)

  if (r->int_no >= 32 && r->int_no < 48) {
    // Handle IRQs
    // put_string("Not an exception \10\13");
    char buff[3];
    int_to_hex_string(r->int_no, buff, 3);
    // put_string(buff);
    /*
      IRQ 0 – system timer (cannot be changed)
      IRQ 1 – keyboard on PS/2 port (cannot be changed)
      IRQ 2 – cascaded signals from IRQs 8–15 (any devices configured to use
      IRQ 2 will actually be using IRQ 9) IRQ 3 – serial port controller for
      serial port 2 (shared with serial port 4, if present)
      IRQ 4 – serial port
      controller for serial port 1 (shared with serial port 3, if present) IRQ
      5 – parallel port 3 or sound card IRQ 6 – floppy disk controller IRQ 7 –
      parallel port 1 (shared with parallel port 2, if present). It is used for
      printers or for any parallel port if a printer is not present. It can
      also be potentially be shared with a secondary sound card with careful
      management of the port. Slave PIC IRQ 8 – real-time clock (RTC) IRQ 9 –
      Advanced Configuration and Power Interface (ACPI) system control
      interrupt on Intel chipsets.[5] Other chipset manufacturers might use
      another interrupt for this purpose, or make it available for the use of
      peripherals (any devices configured to use IRQ 2 will actually be using
      IRQ 9) IRQ 10 – The Interrupt is left open for the use of peripherals
      (open interrupt/available, SCSI or NIC) IRQ 11 – The Interrupt is left
      open for the use of peripherals (open interrupt/available, SCSI or NIC)
      IRQ 12 – mouse on PS/2 port
      IRQ 13 – CPU co-processor or integrated floating point unit or
      inter-processor interrupt (use depends on OS) IRQ 14 – primary ATA
      channel (ATA interface usually serves hard disk drives and CD drives) IRQ
      15 – secondary ATA channel
             */
    // TODO: replace with a dispatcher array
    if (r->int_no == 33) {
      // put_string("Clock!\10\13");
      handle_clock_pulse();
    }
    if (r->int_no == 34) {
      // put_string("Keyboard!!!!!!!!\10\13");
      keyboard_handler(r);
    }

    if (r->int_no == 44) {
      put_string("We got a mouse baby \n");
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
  // put_string("Set interrupt flag \10\13");
  __asm__ volatile("sti");
}
