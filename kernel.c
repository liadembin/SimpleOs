#include "kernel.h"
// #include "interrupts.h"
// not mine also this is maddness

// typedef uint64_t Idt_gate;
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
#define PTE_PRESENT_BIT 0b1
#define PTE_READ_WRITE_BIT 0b10
#define PTE_USER_USERVISOR_BIT 0b100
#define PTE_WRITE_THROUGH_BIT 0b1000
#define PTE_CACHE_DISABLE_BIT 0b10000
#define PTE_ACCESSED_BIT 0b100000
#define PTE_DIRTY_BIT 0b1000000
#define PTE_PAT_BIT 0b10000000
#define PTE_GLOBAL_BIT 0b100000000
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
  uint32_t page_dir_base = 0b000000000000000000000000000001011;

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
}
// no prot no flags
// all pages are read/write except
//  for now single procces(shell) so im not concerned
//  just curious regrading paging
void *mmap(void *addr, unsigned int length, int fd) {}
int munmap(void *addr, unsigned int length) {}
// extern void enable_paging_asm();
void enable_new_paging() {
  // static page_table
  // static page_directory
  // ima hope claude translated ok cause i aint learning c inline syntax(ie
  // AT&T)
  __asm__ __volatile__("mov %0, %%eax\n\t"
                       "mov %%eax, %%cr3\n\t"
                       "mov %%cr0, %%eax\n\t"
                       "orl $0x80000001, %%eax\n\t"
                       "mov %%eax, %%cr0"
                       :
                       : "r"(page_directory)
                       : "%eax");
}
void main() {
  char *video_memory = (char *)0xb8000;
  char message[] = "Hello World\n";
  put_string(message);
  isr_install();
  __asm__ volatile("sti");
  put_string("Enabling paging\n");
  // enable_new_paging();
  // enable_paging_asm();

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
