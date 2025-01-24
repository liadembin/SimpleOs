#include "ports.h"
#include "stdlib.h"
#include "vga.h"
#include <stdint.h>
#define NULL (char *)0x00;

#define KEY_AMOUNT 62

#define IDT_ENTRIES 256
typedef struct {
  uint32_t ds;
  // general purpose registers pushed by pusha
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  // pushed by isr procedure
  uint32_t int_no, err_code;
  // pushed by CPU automatically
  uint32_t eip, cs, eflags, useresp, ss;
} registers_t;
// not mine also this is maddness

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
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
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

#define SHOW_TIME 1
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

void handle_date(char *buffer, int length) {}
void handle_nice(char *buffer, int length) {}
void handle_dir(char *buffer, int length) {}
void handle_hostid(char *buffer, int length) {}
void handle_ptx(char *buffer, int length) {}
void handle_basename(char *buffer, int length) {}
void handle_printenv(char *buffer, int length) {}
void handle_base32(char *buffer, int length) {}
void handle_stat(char *buffer, int length) {}
void handle_chown(char *buffer, int length) {}
void handle_install(char *buffer, int length) {}
void handle_realpath(char *buffer, int length) {}
void handle_runcon(char *buffer, int length) {}
void handle_md5sum(char *buffer, int length) {}
void handle_truncate(char *buffer, int length) {}
void handle_unexpand(char *buffer, int length) {}
void handle_chcon(char *buffer, int length) {}
void handle_vdir(char *buffer, int length) {}
void handle_wc(char *buffer, int length) {}
void handle_nohup(char *buffer, int length) {}
void handle_false(char *buffer, int length) {}
void handle_expand(char *buffer, int length) {}
void handle_head(char *buffer, int length) {}
void handle_pwd(char *buffer, int length) {}
void handle_chmod(char *buffer, int length) {}
void handle_sha256sum(char *buffer, int length) {}
void handle_cut(char *buffer, int length) {}
void handle_sha224sum(char *buffer, int length) {}
void handle_fold(char *buffer, int length) {}
void handle_base64(char *buffer, int length) {}
void handle_uniq(char *buffer, int length) {}
void handle_uptime(char *buffer, int length) {}
void handle_nl(char *buffer, int length) {}
void handle_cp(char *buffer, int length) {}
void handle_expr(char *buffer, int length) {}
void handle_mktemp(char *buffer, int length) {}
void handle_tac(char *buffer, int length) {}
void handle_groups(char *buffer, int length) {}
void handle_pinky(char *buffer, int length) {}
void handle_comm(char *buffer, int length) {}
void handle_arch(char *buffer, int length) {}
void handle_split(char *buffer, int length) {}
void handle_unlink(char *buffer, int length) {}
void handle_stdbuf(char *buffer, int length) {}
void handle_test(char *buffer, int length) {}
void handle_sha1sum(char *buffer, int length) {}
void handle_sleep(char *buffer, int length) {}
void handle_users(char *buffer, int length) {}
void handle_sort(char *buffer, int length) {}
void handle_shuf(char *buffer, int length) {}
void handle_rmdir(char *buffer, int length) {}
void handle_mkdir(char *buffer, int length) {}
void handle_b2sum(char *buffer, int length) {}
void handle_readlink(char *buffer, int length) {}
void handle_fmt(char *buffer, int length) {}
void handle_sha512sum(char *buffer, int length) {}
void handle_touch(char *buffer, int length) {}
void handle_link(char *buffer, int length) {}
void handle_whoami(char *buffer, int length) {}
void handle_numfmt(char *buffer, int length) {}
void handle_ln(char *buffer, int length) {}
void handle_logname(char *buffer, int length) {}
void handle_dircolors(char *buffer, int length) {}
void handle_who(char *buffer, int length) {}
void handle_seq(char *buffer, int length) {}
void handle_true(char *buffer, int length) {}
void handle_dirname(char *buffer, int length) {}
void handle_sum(char *buffer, int length) {}
void handle_mkfifo(char *buffer, int length) {}
void handle_chroot(char *buffer, int length) {}
void handle_paste(char *buffer, int length) {}
void handle_pr(char *buffer, int length) {}
void handle_od(char *buffer, int length) {}
void handle_id(char *buffer, int length) {}
void handle_tsort(char *buffer, int length) {}
void handle_printf(char *buffer, int length) {}
void handle_cat(char *buffer, int length) {}
void handle_nproc(char *buffer, int length) {}
void handle_chgrp(char *buffer, int length) {}
void handle_mknod(char *buffer, int length) {}
void handle_yes(char *buffer, int length) {}
void handle_tty(char *buffer, int length) {}
void handle_ls(char *buffer, int length) {}
void handle_sha384sum(char *buffer, int length) {}
void handle_timeout(char *buffer, int length) {}
void handle_mv(char *buffer, int length) {}
void handle_pathchk(char *buffer, int length) {}
void handle_sync(char *buffer, int length) {}
void handle_shred(char *buffer, int length) {}
void handle_join(char *buffer, int length) {}
void handle_stty(char *buffer, int length) {}
void handle_uname(char *buffer, int length) {}
void handle_du(char *buffer, int length) {}
// void handle(char* buffer,int length){}
void handle_tee(char *buffer, int length) {}
void handle_df(char *buffer, int length) {}
void handle_dd(char *buffer, int length) {}
void handle_echo(char *buffer, int length) {}
void handle_csplit(char *buffer, int length) {}
void handle_tail(char *buffer, int length) {}
void handle_env(char *buffer, int length) {}
void handle_factor(char *buffer, int length) {}
void handle_tr(char *buffer, int length) {}
void handle_rm(char *buffer, int length) {}
void handle_cksum(char *buffer, int length) {}
#define MAX_COMMAND 1024
void handle_command(char *command, int length) {
  put_string("\nHandling your shell command: \n");
  put_string(command);
  put_string("\n");
  char *gnu_core_utils[] = {
      "chcon",     "chgrp",     "chown",     "chmod",    "cp",      "dd",
      "df",        "dir",       "dircolors", "ls",       "install", "ln",
      "ls",        "mkdir",     "mkfifo",    "mknod",    "mktemp",  "mv",
      "realpath",  "rm",        "rmdir",     "shred",    "sync",    "touch",
      "truncate",  "vdir",      "b2sum",     "base32",   "base64",  "cat",
      "cksum",     "comm",      "csplit",    "cut",      "expand",  "fmt",
      "fold",      "head",      "join",      "md5sum",   "nl",      "numfmt",
      "od",        "paste",     "ptx",       "pr",       "sha1sum", "sha224sum",
      "sha256sum", "sha384sum", "sha512sum", "shuf",     "sort",    "split",
      "sum",       "tac",       "tail",      "tr",       "tsort",   "unexpand",
      "uniq",      "wc",        "arch",      "basename", "chroot",  "date",
      "dirname",   "du",        "echo",      "env",      "expr",    "factor",
      "false",     "groups",    "hostid",    "id",       "link",    "logname",
      "nice",      "nohup",     "nproc",     "pathchk",  "pinky",   "printenv",
      "printf",    "pwd",       "readlink",  "runcon",   "seq",     "sleep",
      "stat",      "stdbuf",    "stty",      "tee",      "test",    "timeout",
      "true",      "tty",       "uname",     "unlink",   "unlink",  "uptime",
      "users",     "who",       "whoami",    "yes",
  }; // not including [ because fuck that for now //   "["};

  int indx = get_str_index(gnu_core_utils, command,
                           sizeof(gnu_core_utils) / sizeof(gnu_core_utils[0]));
  if (indx == -1) {
    put_string("Unkown core util \n");
    return;
  }
  typedef void (*util_handler)(char *, int);
  util_handler arr[] = {
      handle_date,    handle_nice,      handle_dir,       handle_hostid,
      handle_ptx,     handle_basename,  handle_printenv,  handle_base32,
      handle_stat,    handle_chown,     handle_install,   handle_realpath,
      handle_runcon,  handle_md5sum,    handle_truncate,  handle_unexpand,
      handle_chcon,   handle_vdir,      handle_wc,        handle_nohup,
      handle_false,   handle_expand,    handle_head,      handle_pwd,
      handle_chmod,   handle_sha256sum, handle_cut,       handle_sha224sum,
      handle_fold,    handle_base64,    handle_uniq,      handle_uptime,
      handle_nl,      handle_cp,        handle_expr,      handle_mktemp,
      handle_tac,     handle_groups,    handle_pinky,     handle_comm,
      handle_arch,    handle_split,     handle_unlink,    handle_stdbuf,
      handle_test,    handle_sha1sum,   handle_sleep,     handle_users,
      handle_sort,    handle_shuf,      handle_rmdir,     handle_mkdir,
      handle_b2sum,   handle_readlink,  handle_fmt,       handle_sha512sum,
      handle_touch,   handle_link,      handle_whoami,    handle_numfmt,
      handle_ln,      handle_logname,   handle_dircolors, handle_who,
      handle_seq,     handle_true,      handle_dirname,   handle_sum,
      handle_mkfifo,  handle_chroot,    handle_paste,     handle_pr,
      handle_od,      handle_id,        handle_tsort,     handle_printf,
      handle_cat,     handle_nproc,     handle_chgrp,     handle_mknod,
      handle_yes,     handle_tty,       handle_ls,        handle_sha384sum,
      handle_timeout, handle_mv,        handle_pathchk,   handle_sync,
      handle_shred,   handle_join,      handle_stty,      handle_uname,
      handle_du,      handle_tee,       handle_df,        handle_dd,
      handle_echo,    handle_csplit,    handle_tail,      handle_env,
      handle_factor,  handle_tr,        handle_rm,        handle_cksum};
  arr[indx](command, length);

  put_string("\n >>>>");
}
void shell_handle(char *new_char) {
  static char command_buffer[MAX_COMMAND];
  static unsigned int ptr = 0;

  if (strlen(new_char) > 1) {
    // special char then
    if (strcmp(new_char, "BACKSPACE")) {
      if (ptr > 0) {

        ptr--;
      }
      Cursor_position pos = get_cursor_position();
      if (pos.x > 0) {
        pos.x--;
      }
      clear_pixel(pos);
      set_cursor_position(pos.x, pos.y);
      // TODO: handle deleting a new line char
    }
    if (strcmp(new_char, "ENTER")) {
      command_buffer[ptr++] = '\0';
      handle_command(command_buffer, ptr);
      ptr = 0;
    }

    return;
  }

  put_string(new_char);
  if (ptr >= MAX_COMMAND - 1) {
    put_string("Cant put more chars. ");
    return;
  }
  command_buffer[ptr++] = new_char[0];
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
void print_state(registers_t *r) {
  const int BUFF_SIZE = 15;
  char buffer[BUFF_SIZE];
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
}
// Modify the isr_handler
// function to include IRQ handling
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
    // if (r->int_no == 41) {
    //   put_string("We got a real clock not ossilator\n");
    // }
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
// ama just belive cluade translated correctly
// cause i cant be bothered
void _load_page_directory(unsigned int *page_directory) {
  __asm__ volatile("push %%ebp\n\t"
                   "mov %%esp, %%ebp\n\t"
                   "mov %0, %%eax\n\t"
                   "mov %%eax, %%cr3\n\t"
                   "mov %%ebp, %%esp\n\t"
                   "pop %%ebp"
                   :
                   : "r"(page_directory)
                   : "%eax");
}

void _enable_paging(void) {
  __asm__ volatile("push %%ebp\n\t"
                   "mov %%esp, %%ebp\n\t"
                   "mov %%cr0, %%eax\n\t"
                   "or $0x80000000, %%eax\n\t"
                   "mov %%eax, %%cr0\n\t"
                   "mov %%ebp, %%esp\n\t"
                   "pop %%ebp"
                   :
                   :
                   : "%eax");
}
#define PAGE_COUNT 1024 // pulled from my ass, table should be 4mb
// https://wiki.osdev.org/Paging#32-bit_Paging_(Protected_Mode)
// They are the paging directory (PD), and the paging table (PT).
// Both tables contain 1024 4-byte entries, making them 4 KiB each
// entry points to a page table
#define PDE_PRESENT_BIT                                                        \
  0b1 // Bit 0 // or 'Present'. If the bit is set, the page is actually in
      // physical memory
      // at the moment. For example, when a page is swapped out, it is not in
// physical memory and therefore not 'Present'. If a page is called, but not
// present, a page fault will occur, and the OS should handle it. (See
// below.)
#define PDE_READ_WRITE_BIT                                                     \
  0b10 // the 'Read/Write' permissions flag. If the bit is set, the page is
// read/write. Otherwise when it is not set, the page is read-only. The WP
// bit in CR0 determines if this is only applied to userland, always giving
// the kernel write access (the default) or both userland and the kernel
// (see Intel Manuals 3A 2-20).
#define PDE_USER_USERVISOR_BIT                                                 \
  0b100 //, the 'User/Supervisor' bit, controls access to the page based on
// privilege level. If the bit is set, then the page may be accessed by all;
// if the bit is not set, however, only the supervisor can access it. For a
// page directory entry, the user bit controls access to all the pages
// referenced by the page directory entry. Therefore if you wish to make a
// page a user page, you must set the user bit in the relevant page
// directory entry as well as the page table entry.
#define PDE_WRITE_THROUGH_BIT                                                  \
  0b1000 // controls Write-Through' abilities of the page. If the bit is set,
         // write-through caching is enabled. If not, then write-back is enabled
         // instead.
#define PDE_CACHE_DISABLE_BIT                                                  \
  0b10000 // is the 'Cache Disable' bit. If the bit is set, the page will not be
          // cached. Otherwise, it will be.
#define PDE_ACCESSED_BIT                                                       \
  0b100000 // is used to discover whether a PDE or PTE was read during virtual
           // address
// translation. If it has, then the bit is set, otherwise, it is not. Note
// that, this bit will not be cleared by the CPU, so that burden falls on
// the OS (if it needs this bit at all).
#define PDE_DIRTY_BIT                                                          \
  0b1000000 // we can use both the 6,8 bits so the 6th repesents dirty

#define PDE_IS_4MB_BIT                                                         \
  0b10000000 // staight foward, if set(it wont be) it changes meaning of some
             // stuff ie stuff like the DIRTY_BIT
#define PDE_AVAILABLE_BIT                                                      \
  0b10000000 // The remaining bits 9 through 11 (if PS=0, also bits 6 & 8) are
             // not used by the processor, and are free for the OS to store some
             // of its own accounting information. In addition, when P is not
             // set, the processor ignores the rest of the entry and you can use
             // all remaining 31 bits for extra information
#define PTE_PRESENT_BIT 0b1           // Bit 0
#define PTE_READ_WRITE_BIT 0b10       // Bit 1
#define PTE_USER_USERVISOR_BIT 0b100  // Bit 2
#define PTE_WRITE_THROUGH_BIT 0b1000  // Bit 3
#define PTE_CACHE_DISABLE_BIT 0b10000 // Bit 4
#define PTE_ACCESSED_BIT 0b100000     // Bit 5
#define PTE_DIRTY_BIT 0b1000000       // Bit 6
#define PTE_PAT_BIT 0b10000000        // Bit 7 (Page Attribute Table)
#define PTE_GLOBAL_BIT 0b100000000    // Bit 8
#define TABLE_COUNT 1024
static int32_t page_directory[TABLE_COUNT]
    __attribute__((aligned(4096))); // each value points to a page_table
static int32_t page_tables[TABLE_COUNT][1024] __attribute__((aligned(4096)));
void enable_paging() {
  // clang-format off
  //+--------------------------------------------------------------------------------+
  //|                             PAGE DIRECTORY int32 structure                      |
  //+--------------------------------------------------------------------------------+
  //| 31                        12 | 11  10  9  8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  //+-------------------------------+---------------+---+---+---+---+---+---+---+---+
  //| SECTOR (bits 31-12 of address)|  Free for use |4MB|AVL|ACC|CD |WT |U/S|R/W| P |
  //+-------------------------------+---------------+---+---+---+---+---+---+---+---+
  //| ????????????????????????????????| 0    0   0  0 | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 1 |
  //+--------------------------------------------------------------------------------+
  //
  // SECTOR:    Physical address of 4KB aligned page table or page frame (bits 31-12)
  // Free:      Bits 11-8 free for use
  // 4MB:       0 = 4KB page, 1 = 4MB page (if PSE is 1 in CR4)
  // AVL:       Available for system programmer's use
  // ACC:       Accessed bit
  // CD:        Cache disable
  // WT:        Write-through cache
  // U/S:       0 = Supervisor, 1 = User
  // R/W:       0 = Read-only, 1 = Read/Write
  // P:         Present bit
  // clang-format on
  uint32_t page_dir_base = 0b000000000000000000000000000001011;

  // clang-format off
  //+--------------------------------------------------------------------------------+
  //|                             PAGE TABLE ENTRY int32 structure                    |
  //+--------------------------------------------------------------------------------+
  //| 31                        12 | 11  10  9  8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
  //+-------------------------------+---------------+---+---+---+---+---+---+---+---+
  //| FRAME (bits 31-12 of address) |  Free for use |PAT|G |D |A |CD |WT |U/S|R/W| P |
  //+-------------------------------+---------------+---+---+---+---+---+---+---+---+
  //| ????????????????????????????????| 0  0   0  0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 1 |
  //+--------------------------------------------------------------------------------+
  //
  // FRAME:     Physical address of 4KB aligned page frame (bits 31-12)
  // Free:      Bits 11-9 free for use (bit 8 used for PAT on some architectures)
  // PAT:       Page Attribute Table (if supported)
  // G:         Global page (if set, page is global)
  // D:         Dirty (has been written to)
  // A:         Accessed (has been read from or written to)
  // CD:        Cache disable
  // WT:        Write-through cache
  // U/S:       0 = Supervisor, 1 = User
  // R/W:       0 = Read-only, 1 = Read/Write
  // P:         Present bit
  // clang-format on
  uint32_t page_table_base = 0b000000000000000000000000000001011;

  for (int i = 0; i < PAGE_COUNT; i++) {
    // each table has 1024 entries
    for (int j = 0; j < 1024; j++) {
      // Calculate the physical frame number (PFN)
    }
  }

  for (int i = 0; i < PAGE_COUNT; i++) {
    // Combine the page table address with the page directory base flags
    // againg the mask zeros the offset...
  }

  // Load page directory base register (PDBR)
  asm volatile("movl %0, %%cr3" : : "r"(page_directory));

  // Enable paging by setting the paging bit in CR0
  asm volatile("movl %%cr0, %%eax; orl $0x80000000, %%eax; movl %%eax, %%cr0"
               :
               :
               : "eax");
}
// no prot no flags
// all pages are read/write except
//  for now single procces(shell) so im not concerned
//  just curious regrading paging
void *mmap(void *addr, unsigned int length, int fd) {}
int munmap(void *addr, unsigned int length) {}
void main() {
  char *video_memory = (char *)0xb8000;
  char message[] = "Hello World\n";
  put_string(message);
  isr_install();
  __asm__ volatile("sti");
  put_string("Enabling paging\n");
  // enable_paging();
  put_string("Paging is enabled \n");
  put_string("Starting the Shell now, keyboard is loaded \n");
  put_string(" >>>>");
  // test_paging();
  // put_string(exception_messages[0]);
  // int a = 1 / 0;
  //  irq1();
  // while (1) {
  //   // for (char i = 'a'; i <= 'z'; i++) {
  //   //   print[0] = i;
  //   //   put_string(print);
  //   // }
  //   // for (int j = 0; j < 100; j++) {
  //   //   for (int i = 0; i < 2000000; i++) {
  //   //   }
  //   // }
  //   // clear_screen();
  // }
  *(video_memory + 100) = 'V';
  *(video_memory + 101) = 0x0f;
}
