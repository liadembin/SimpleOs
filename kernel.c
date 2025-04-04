#include "ports.h"
#include "stdlib.h"
#include "vga.h"
#include "interrupts.h"
#include "paging.h"
#include "network.h"

#include <stdint.h>
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
void handle_put_string(char *buffer, int length) {}
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
      "chcon",     "chgrp",    "chown",     "chmod",     "cp",
      "dd",        "df",       "dir",       "dircolors", "ls",
      "install",   "ln",       "ls",        "mkdir",     "mkfifo",
      "mknod",     "mktemp",   "mv",        "realpath",  "rm",
      "rmdir",     "shred",    "sync",      "touch",     "truncate",
      "vdir",      "b2sum",    "base32",    "base64",    "cat",
      "cksum",     "comm",     "csplit",    "cut",       "expand",
      "fmt",       "fold",     "head",      "join",      "md5sum",
      "nl",        "numfmt",   "od",        "paste",     "ptx",
      "pr",        "sha1sum",  "sha224sum", "sha256sum", "sha384sum",
      "sha512sum", "shuf",     "sort",      "split",     "sum",
      "tac",       "tail",     "tr",        "tsort",     "unexpand",
      "uniq",      "wc",       "arch",      "basename",  "chroot",
      "date",      "dirname",  "du",        "echo",      "env",
      "expr",      "factor",   "false",     "groups",    "hostid",
      "id",        "link",     "logname",   "nice",      "nohup",
      "nproc",     "pathchk",  "pinky",     "printenv",  "put_string",
      "pwd",       "readlink", "runcon",    "seq",       "sleep",
      "stat",      "stdbuf",   "stty",      "tee",       "test",
      "timeout",   "true",     "tty",       "uname",     "unlink",
      "unlink",    "uptime",   "users",     "who",       "whoami",
      "yes",
  }; // not including [ because fuck that for now //   "["};

  int indx = get_str_index(gnu_core_utils, command,
                           sizeof(gnu_core_utils) / sizeof(gnu_core_utils[0]));
  if (indx == -1) {
    put_string("Unkown core util \n");
    put_string(" >>>>");
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
      handle_od,      handle_id,        handle_tsort,     handle_put_string,
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
      if (pos.x > 5) {
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


// #include "pci.h"
void read_bars(pci_device device) {
  uint32_t bar0 = pciConfigReadWord(device.bus, device.slot, device.func, 0x10);
  if ((bar0 & 0b11) == 0b00) {
    put_string("ITS 32 \n");
  } else if ((bar0 & 0b11) == 0b10) {
    put_string("ITS 64\n");
  } else {
    put_string("IDK WTF \n");
  }
  // uint32_t bar2 = pciConfigReadWord(bus, slot, func, 0x18);
  // There is also a BAR that will contain an I/O base address, this can be
  // detected by looking at each BAR and testing bit 1. Documentation states
  // this will be in either BAR2 or BAR4, but emulators may move it.
  // https://www.google.com/search?q=how+many+BAR+in+pci&oq=how+many+BAR+in+pci&gs_lcrp=EgZjaHJvbWUyBggAEEUYOTIGCAEQLhhA0gEIMzg3OGowajGoAgCwAgA&sourceid=chrome&ie=UTF-8
  for (uint8_t bar = 1; bar < 6; bar++) {
    uint32_t bar_value = pciConfigReadWord(device.bus, device.slot, device.func,
                                           0x10 + 0x4 * bar);
    if ((bar_value & 1) != 0) {
      put_string("Found a fitting BAR");
      char buffer[5];
      int_to_hex_string(bar, buffer, 5);
      put_string(buffer);
      put_string("\n");
      int_to_hex_string(bar_value, buffer, 5);
      put_string("Its value is: ");
      put_string(buffer);
      put_string("\n");
    }
  }
  return;
}
// uint16_t i8254x_read_eeprom(uint8_t addr) {
//   uint32_t tmp;
//   uint16_t data;
//
//   if ((le32_to_cpu(mmio_read_dword(dev_info.mmio.addr, I8254X_EECD)) &
//        I8254X_EECD_EE_PRES) == 0) {
//     kpanic("EEPROM present bit is not set for i8254x\n");
//   }
//
//   /* Tell the EEPROM to start reading */
//   if (dev_info.version == I82547GI_EI || dev_info.version == I82541EI_A0 ||
//       dev_info.version == I82541EI_B0 || dev_info.version == I82541ER_C0 ||
//       dev_info.version == I82541GI_B1 || dev_info.version == I82541PI_C0) {
//     /* Specification says that only 82541x devices and the
//      * 82547GI/EI do 2-bit shift */
//     tmp = ((uint32_t)addr & 0xfff) << 2;
//   } else {
//     tmp = ((uint32_t)addr & 0xff) << 8;
//   }
//   tmp |= I8254X_EERD_START;
//   mmio_write_dword(dev_info.mmio.addr, I8254X_EERD, cpu_to_le32(tmp));
//
//   /* Wait until the read is finished - then the DONE bit is cleared */
//   timeout((le32_to_cpu(mmio_read_dword(dev_info.mmio.addr, I8254X_EERD)) &
//            I8254X_EERD_DONE) == 0,
//           100);
//
//   /* Obtain the data */
//   data = (uint16_t)(le32_to_cpu(
//                         mmio_read_dword(dev_info.mmio.addr, I8254X_EERD)) >>
//                     16);
//
//   /* Tell EEPROM to stop reading */
//   tmp = le32_to_cpu(mmio_read_dword(dev_info.mmio.addr, I8254X_EERD));
//   tmp &= ~(uint32_t)I8254X_EERD_START;
//   mmio_write_dword(dev_info.mmio.addr, I8254X_EERD, cpu_to_le32(tmp));
//   return data;
// }
//
// #include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* ATA driver for 28-bit LBA addressing mode */

/* External functions we'll use */
// void put_string(const char *str);
// void int_to_hex_string(int value, char *buffer, int buffer_size);

/* IO port addresses for ATA buses */
#define PRIMARY_IO_BASE 0x1F0
#define PRIMARY_CONTROL_BASE 0x3F6
#define SECONDARY_IO_BASE 0x170
#define SECONDARY_CONTROL_BASE 0x376

/* Port offsets from IO base */
#define ATA_DATA 0x0         /* Read/Write PIO data bytes */
#define ATA_ERROR 0x1        /* Error register (read) */
#define ATA_FEATURES 0x1     /* Features register (write) */
#define ATA_SECTOR_COUNT 0x2 /* Number of sectors to read/write */
#define ATA_LBA_LO 0x3       /* LBA low bits (0:7) */
#define ATA_LBA_MID 0x4      /* LBA mid bits (8:15) */
#define ATA_LBA_HI 0x5       /* LBA high bits (16:23) */
#define ATA_DRIVE_HEAD 0x6   /* Drive/Head selection and LBA bits (24:27) */
#define ATA_STATUS 0x7       /* Status register (read) */
#define ATA_COMMAND 0x7      /* Command register (write) */

/* Port offsets from Control base */
#define ATA_ALT_STATUS 0x0     /* Alternate status register (read) */
#define ATA_DEVICE_CONTROL 0x0 /* Device control register (write) */
#define ATA_DRIVE_ADDR 0x1     /* Drive address register */

/* Status register bits */
#define ATA_STATUS_ERR 0x01 /* Error occurred */
#define ATA_STATUS_DRQ 0x08 /* Data Request ready */
#define ATA_STATUS_SRV 0x10 /* Service */
#define ATA_STATUS_DF 0x20  /* Drive Fault */
#define ATA_STATUS_RDY 0x40 /* Drive Ready */
#define ATA_STATUS_BSY 0x80 /* Busy */

/* Error register bits */
#define ATA_ERROR_AMNF 0x01  /* Address mark not found */
#define ATA_ERROR_TKZNF 0x02 /* Track zero not found */
#define ATA_ERROR_ABRT 0x04  /* Aborted command */
#define ATA_ERROR_MCR 0x08   /* Media change request */
#define ATA_ERROR_IDNF 0x10  /* ID not found */
#define ATA_ERROR_MC 0x20    /* Media changed */
#define ATA_ERROR_UNC 0x40   /* Uncorrectable data error */
#define ATA_ERROR_BBK 0x80   /* Bad block detected */

/* Drive/Head register bits */
#define ATA_HEAD_MASK 0x0F       /* Head number mask (bits 0-3) */
#define ATA_DRIVE_BIT 0x10       /* Drive number bit (bit 4) */
#define ATA_LBA_BIT 0x40         /* LBA mode bit (bit 6) */
#define ATA_ALWAYS_SET_BITS 0xA0 /* Bits 5 and 7 are always set */

/* Device control register bits */
#define ATA_DCR_NIEN 0x02 /* No Interrupt */
#define ATA_DCR_SRST 0x04 /* Software Reset */
#define ATA_DCR_HOB 0x80  /* High Order Byte */

/* ATA commands */
#define ATA_CMD_READ_PIO 0x20      /* Read sectors (PIO) */
#define ATA_CMD_READ_PIO_EXT 0x24  /* Read sectors (PIO Extended - LBA48) */
#define ATA_CMD_WRITE_PIO 0x30     /* Write sectors (PIO) */
#define ATA_CMD_WRITE_PIO_EXT 0x34 /* Write sectors (PIO Extended - LBA48) */
#define ATA_CMD_IDENTIFY 0xEC      /* Identify device */

/* Device types */
#define DEVICE_TYPE_UNKNOWN 0
#define DEVICE_TYPE_ATA 1
#define DEVICE_TYPE_ATAPI 2
#define DEVICE_TYPE_SATA 3

/* Signature values */
#define ATAPI_SIG_MID 0x14
#define ATAPI_SIG_HI 0xEB
#define SATA_SIG_MID 0x3C
#define SATA_SIG_HI 0xC3

/* Maximum number of sectors for LBA28 addressing */
#define MAX_LBA28_SECTORS 0x0FFFFFFF /* 268,435,455 sectors (128 GiB) */

/* Device information structure */
typedef struct {
  int type;               /* Device type (ATA, ATAPI, SATA, etc.) */
  bool lba28_supported;   /* LBA28 addressing supported */
  bool lba48_supported;   /* LBA48 addressing supported */
  uint32_t lba28_sectors; /* Number of LBA28 addressable sectors */
  uint64_t lba48_sectors; /* Number of LBA48 addressable sectors */
  int udma_supported;     /* Supported UDMA modes (bitmap) */
  int udma_active;        /* Active UDMA mode */
  bool cable_80_detected; /* 80-conductor cable detected (master only) */
} ata_device_info;

/* Disk partition structure */
typedef struct {
  uint32_t start_lba;    /* Absolute start LBA of partition */
  uint32_t sector_count; /* Number of sectors in partition */
} ata_partition;

/* Current ATA state */
static struct {
  uint16_t io_base;      /* Current IO base address */
  uint16_t control_base; /* Current control base address */
  int current_drive;     /* Current selected drive (0 = master, 1 = slave) */
  ata_device_info
      drives[2][2]; /* Device info for primary/secondary master/slave */
  ata_partition current_partition; /* Currently selected partition */
} ata_state = {
    .io_base = PRIMARY_IO_BASE,
    .control_base = PRIMARY_CONTROL_BASE,
    .current_drive = 0,
};

/* Private function prototypes */
static void _ata_outb(uint16_t port, uint8_t val);
static uint8_t _ata_inb(uint16_t port);
static void _ata_insw(uint16_t port, void *addr, int count);
static void _ata_outsw(uint16_t port, void *addr, int count);
static void _ata_400ns_delay(void);
static int _ata_poll_status(void);
static int _ata_select_drive(int bus, int drive);
static void _ata_soft_reset(void);
static void _ata_wait_not_busy(void);
static void _ata_wait_ready(void);
static int _ata_process_error(void);
static uint16_t *_ata_get_identify_data(int bus, int drive, uint16_t *buffer);

/* Public function prototypes */
int ata_init(void);
int ata_identify_drive(int bus, int drive, ata_device_info *info);
void ata_print_drive_info(ata_device_info *info);
int ata_select_partition(uint32_t start_lba, uint32_t sector_count);
int ata_read_sectors(uint32_t lba, int count, void *buffer);
int ata_write_sectors(uint32_t lba, int count, void *buffer);

/*
 * Private Functions
 */

/* Output byte to ATA port */
static void _ata_outb(uint16_t port, uint8_t val) {
  /* On x86, this would be implemented with inline assembly:
   * asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
   */

  /* For now, we'll assume this function is provided externally */
}

/* Input byte from ATA port */
static uint8_t _ata_inb(uint16_t port) {
  /* On x86, this would be implemented with inline assembly:
   * uint8_t val;
   * asm volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
   * return val;
   */

  /* For now, we'll assume this function is provided externally */
  return 0;
}

/* Input multiple words from ATA port */
static void _ata_insw(uint16_t port, void *addr, int count) {
  /* On x86, this would be implemented with inline assembly:
   * asm volatile("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
   */

  /* For now, we'll assume this function is provided externally */
}

/* Output multiple words to ATA port */
static void _ata_outsw(uint16_t port, void *addr, int count) {
  /* On x86, this would be implemented with inline assembly:
   * asm volatile("rep outsw" : "+S"(addr), "+c"(count) : "d"(port) : "memory");
   */

  /* For now, we'll assume this function is provided externally */
}

/* Delay for approximately 400ns by reading the alternate status register */
static void _ata_400ns_delay(void) {
  /* Reading the alternate status register takes about 100ns */
  _ata_inb(ata_state.control_base + ATA_ALT_STATUS);
  _ata_inb(ata_state.control_base + ATA_ALT_STATUS);
  _ata_inb(ata_state.control_base + ATA_ALT_STATUS);
  _ata_inb(ata_state.control_base + ATA_ALT_STATUS);
}

/* Poll the status register until an operation completes */
static int _ata_poll_status(void) {
  uint8_t status;

  /* Wait for BSY to clear */
  _ata_400ns_delay(); /* Initial delay to allow status to update */

  while ((_ata_inb(ata_state.io_base + ATA_STATUS) & ATA_STATUS_BSY))
    ;

  /* Check for error conditions */
  status = _ata_inb(ata_state.io_base + ATA_STATUS);
  if (status & ATA_STATUS_ERR) {
    return _ata_process_error();
  }

  /* Check for drive fault */
  if (status & ATA_STATUS_DF) {
    return -1; /* Drive fault error */
  }

  /* Ensure that data is ready to be transferred */
  if (!(status & ATA_STATUS_DRQ)) {
    return -2; /* Data not ready */
  }

  return 0; /* Success */
}

/* Select a drive on a particular bus */
static int _ata_select_drive(int bus, int drive) {
  uint16_t io_base, control_base;
  uint8_t drive_head;

  /* Set the correct base addresses */
  if (bus == 0) {
    io_base = PRIMARY_IO_BASE;
    control_base = PRIMARY_CONTROL_BASE;
  } else {
    io_base = SECONDARY_IO_BASE;
    control_base = SECONDARY_CONTROL_BASE;
  }

  /* Update the state */
  ata_state.io_base = io_base;
  ata_state.control_base = control_base;
  ata_state.current_drive = drive;

  /* Prepare drive/head value */
  drive_head = ATA_ALWAYS_SET_BITS | ATA_LBA_BIT;
  if (drive == 1) {
    drive_head |= ATA_DRIVE_BIT;
  }

  /* Select drive */
  _ata_outb(io_base + ATA_DRIVE_HEAD, drive_head);
  _ata_400ns_delay(); /* Wait for drive to be selected */

  return 0;
}

/* Perform a software reset on the current bus */
static void _ata_soft_reset(void) {
  /* Set SRST bit in device control register */
  _ata_outb(ata_state.control_base + ATA_DEVICE_CONTROL, ATA_DCR_SRST);

  /* Wait at least 5us */
  _ata_400ns_delay();
  _ata_400ns_delay();

  /* Clear SRST bit in device control register */
  _ata_outb(ata_state.control_base + ATA_DEVICE_CONTROL, 0);

  /* Wait for BSY to clear on the selected drive */
  _ata_wait_not_busy();
}

/* Wait for the BSY bit to clear */
static void _ata_wait_not_busy(void) {
  /* Timeout could be implemented with a counter here */
  while (_ata_inb(ata_state.io_base + ATA_STATUS) & ATA_STATUS_BSY)
    ;
}

/* Wait for the drive to be ready (BSY clear and RDY set) */
static void _ata_wait_ready(void) {
  uint8_t status;

  /* Timeout could be implemented with a counter here */
  do {
    status = _ata_inb(ata_state.io_base + ATA_STATUS);
  } while ((status & ATA_STATUS_BSY) || !(status & ATA_STATUS_RDY));
}

/* Process an error that occurred during an ATA operation */
static int _ata_process_error(void) {
  uint8_t error = _ata_inb(ata_state.io_base + ATA_ERROR);

  /* Return the specific error code for detailed error handling */
  return -(100 + error); /* Negative error code with error register value */
}

/* Get identify data from a drive */
static uint16_t *_ata_get_identify_data(int bus, int drive, uint16_t *buffer) {
  uint8_t status, lba_mid, lba_hi;

  /* Select the drive */
  _ata_select_drive(bus, drive);

  /* Set feature registers to 0 */
  _ata_outb(ata_state.io_base + ATA_SECTOR_COUNT, 0);
  _ata_outb(ata_state.io_base + ATA_LBA_LO, 0);
  _ata_outb(ata_state.io_base + ATA_LBA_MID, 0);
  _ata_outb(ata_state.io_base + ATA_LBA_HI, 0);

  /* Send IDENTIFY command */
  _ata_outb(ata_state.io_base + ATA_COMMAND, ATA_CMD_IDENTIFY);

  /* Check if drive exists */
  status = _ata_inb(ata_state.io_base + ATA_STATUS);
  if (status == 0) {
    return 0; /* Drive does not exist */
  }

  /* Wait for BSY to clear */
  _ata_wait_not_busy();

  /* Check for ATAPI or SATA signature */
  lba_mid = _ata_inb(ata_state.io_base + ATA_LBA_MID);
  lba_hi = _ata_inb(ata_state.io_base + ATA_LBA_HI);

  if (lba_mid != 0 || lba_hi != 0) {
    /* This is not an ATA device, or it aborted the command */
    return 0;
  }

  /* Wait for data ready or error */
  status = _ata_inb(ata_state.io_base + ATA_STATUS);

  /* Check for error */
  if (status & ATA_STATUS_ERR) {
    return 0;
  }

  /* Wait for data ready */
  while (!(status & ATA_STATUS_DRQ)) {
    status = _ata_inb(ata_state.io_base + ATA_STATUS);

    if (status & ATA_STATUS_ERR) {
      return 0;
    }
  }

  /* Read the identify data */
  _ata_insw(ata_state.io_base + ATA_DATA, buffer, 256);

  return buffer;
}

/*
 * Public Functions
 */

/* Initialize the ATA driver */
int ata_init(void) {
  int i, j;
  uint16_t identify_data[256];

  /* Initialize primary/secondary bus device control registers */
  _ata_outb(PRIMARY_CONTROL_BASE + ATA_DEVICE_CONTROL, 0);
  _ata_outb(SECONDARY_CONTROL_BASE + ATA_DEVICE_CONTROL, 0);

  /* Identify all drives */
  for (i = 0; i < 2; i++) {   /* Bus: 0 = primary, 1 = secondary */
    for (j = 0; j < 2; j++) { /* Drive: 0 = master, 1 = slave */
      ata_identify_drive(i, j, &ata_state.drives[i][j]);
    }
  }

  /* Select the primary master by default */
  _ata_select_drive(0, 0);

  /* Success */
  return 0;
}

/* Identify an ATA drive and get its information */
int ata_identify_drive(int bus, int drive, ata_device_info *info) {
  uint16_t identify_data[256];

  /* Initialize device info */
  info->type = DEVICE_TYPE_UNKNOWN;
  info->lba28_supported = false;
  info->lba48_supported = false;
  info->lba28_sectors = 0;
  info->lba48_sectors = 0;
  info->udma_supported = 0;
  info->udma_active = 0;
  info->cable_80_detected = false;

  /* Try to get identify data */
  if (_ata_get_identify_data(bus, drive, identify_data) == NULL) {
    return -1; /* Drive does not exist or is not an ATA drive */
  }

  /* We have an ATA device */
  info->type = DEVICE_TYPE_ATA;

  /* Check if LBA28 is supported and get sector count */
  if ((identify_data[60] != 0) || (identify_data[61] != 0)) {
    info->lba28_supported = true;
    info->lba28_sectors =
        ((uint32_t)identify_data[61] << 16) | identify_data[60];
  }

  /* Check if LBA48 is supported */
  if (identify_data[83] & (1 << 10)) {
    info->lba48_supported = true;

    /* Get LBA48 sector count */
    info->lba48_sectors = ((uint64_t)identify_data[103] << 48) |
                          ((uint64_t)identify_data[102] << 32) |
                          ((uint64_t)identify_data[101] << 16) |
                          identify_data[100];
  }

  /* Get UDMA modes */
  if (identify_data[88] != 0) {
    info->udma_supported = identify_data[88] & 0xFF;
    info->udma_active = (identify_data[88] >> 8) & 0xFF;
  }

  /* Check for 80-conductor cable (master only) */
  if (drive == 0 && (identify_data[93] & (1 << 11))) {
    info->cable_80_detected = true;
  }

  return 0; /* Success */
}

/* Print drive information */
void ata_print_drive_info(ata_device_info *info) {
  char buffer[16];

  /* Print device type */
  put_string("Type: ");
  int_to_hex_string(info->type, buffer, sizeof(buffer));
  put_string(buffer);
  put_string("\n");

  /* Only continue if it's an ATA device */
  if (info->type != DEVICE_TYPE_ATA) {
    return;
  }

  /* Print LBA support */
  put_string("LBA28: ");
  put_string(info->lba28_supported ? "Yes" : "No");
  put_string("\n");

  if (info->lba28_supported) {
    put_string("LBA28 Sectors: ");
    int_to_hex_string(info->lba28_sectors, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
  }

  put_string("LBA48: ");
  put_string(info->lba48_supported ? "Yes" : "No");
  put_string("\n");

  /* Print UDMA modes */
  put_string("UDMA Supported: ");
  int_to_hex_string(info->udma_supported, buffer, sizeof(buffer));
  put_string(buffer);
  put_string("\n");

  put_string("UDMA Active: ");
  int_to_hex_string(info->udma_active, buffer, sizeof(buffer));
  put_string(buffer);
  put_string("\n");

  /* Print cable detection */
  put_string("80-pin Cable: ");
  put_string(info->cable_80_detected ? "Yes" : "No");
  put_string("\n");
}

/* Select a partition for subsequent operations */
int ata_select_partition(uint32_t start_lba, uint32_t sector_count) {
  /* Validate the partition */
  if (start_lba + sector_count > ata_state.drives[0][0].lba28_sectors) {
    return -1; /* Partition exceeds drive capacity */
  }

  /* Store the partition information */
  ata_state.current_partition.start_lba = start_lba;
  ata_state.current_partition.sector_count = sector_count;

  return 0; /* Success */
}

/* Read sectors from the current partition */
int ata_read_sectors(uint32_t lba, int count, void *buffer) {
  uint32_t absolute_lba;
  int i;

  /* Validate parameters */
  if (count <= 0 || count > 256) {
    return -1; /* Invalid count (0 means 256 sectors, but we don't support that)
                */
  }

  if (lba + count > ata_state.current_partition.sector_count) {
    return -2; /* Read beyond partition boundary */
  }

  /* Convert relative LBA to absolute LBA */
  absolute_lba = ata_state.current_partition.start_lba + lba;

  /* Ensure we're within LBA28 range */
  if (absolute_lba + count > MAX_LBA28_SECTORS) {
    return -3; /* Beyond LBA28 limit */
  }

  /* Select the correct drive (we use primary master for simplicity) */
  _ata_select_drive(0, 0);

  /* Wait for drive to be ready */
  _ata_wait_ready();

  /* Set up registers for the read operation */
  _ata_outb(ata_state.io_base + ATA_SECTOR_COUNT, count);
  _ata_outb(ata_state.io_base + ATA_LBA_LO, absolute_lba & 0xFF);
  _ata_outb(ata_state.io_base + ATA_LBA_MID, (absolute_lba >> 8) & 0xFF);
  _ata_outb(ata_state.io_base + ATA_LBA_HI, (absolute_lba >> 16) & 0xFF);

  /* Set drive/head register with the highest 4 bits of LBA and drive select */
  _ata_outb(ata_state.io_base + ATA_DRIVE_HEAD,
            ATA_ALWAYS_SET_BITS | ATA_LBA_BIT | ((absolute_lba >> 24) & 0x0F));

  /* Send the read command */
  _ata_outb(ata_state.io_base + ATA_COMMAND, ATA_CMD_READ_PIO);

  /* Read all the sectors */
  for (i = 0; i < count; i++) {
    /* Wait for data to be ready */
    if (_ata_poll_status() != 0) {
      return -4; /* Error during read */
    }

    /* Read a sector (256 words = 512 bytes) */
    _ata_insw(ata_state.io_base + ATA_DATA, buffer, 256);

    /* Move buffer pointer */
    buffer = (uint8_t *)buffer + 512;
  }

  return 0; /* Success */
}

/* Write sectors to the current partition */
int ata_write_sectors(uint32_t lba, int count, void *buffer) {
  uint32_t absolute_lba;
  int i;

  /* Validate parameters */
  if (count <= 0 || count > 256) {
    return -1; /* Invalid count (0 means 256 sectors, but we don't support that)
                */
  }

  if (lba + count > ata_state.current_partition.sector_count) {
    return -2; /* Write beyond partition boundary */
  }

  /* Convert relative LBA to absolute LBA */
  absolute_lba = ata_state.current_partition.start_lba + lba;

  /* Ensure we're within LBA28 range */
  if (absolute_lba + count > MAX_LBA28_SECTORS) {
    return -3; /* Beyond LBA28 limit */
  }

  /* Select the correct drive (we use primary master for simplicity) */
  _ata_select_drive(0, 0);

  /* Wait for drive to be ready */
  _ata_wait_ready();

  /* Set up registers for the write operation */
  _ata_outb(ata_state.io_base + ATA_SECTOR_COUNT, count);
  _ata_outb(ata_state.io_base + ATA_LBA_LO, absolute_lba & 0xFF);
  _ata_outb(ata_state.io_base + ATA_LBA_MID, (absolute_lba >> 8) & 0xFF);
  _ata_outb(ata_state.io_base + ATA_LBA_HI, (absolute_lba >> 16) & 0xFF);

  /* Set drive/head register with the highest 4 bits of LBA and drive select */
  _ata_outb(ata_state.io_base + ATA_DRIVE_HEAD,
            ATA_ALWAYS_SET_BITS | ATA_LBA_BIT | ((absolute_lba >> 24) & 0x0F));

  /* Send the write command */
  _ata_outb(ata_state.io_base + ATA_COMMAND, ATA_CMD_WRITE_PIO);

  /* Write all the sectors */
  for (i = 0; i < count; i++) {
    /* Wait for drive to be ready to accept data */
    if (_ata_poll_status() != 0) {
      return -4; /* Error preparing for write */
    }

    /* Write a sector (256 words = 512 bytes) */
    _ata_outsw(ata_state.io_base + ATA_DATA, buffer, 256);

    /* Move buffer pointer */
    buffer = (uint8_t *)buffer + 512;

    /* Flush the write cache on the last sector */
    if (i == count - 1) {
      /* The drive will set BSY again after receiving the last word */
      _ata_wait_not_busy();
    }
  }

  return 0; /* Success */
}
/* Small test function for the ATA driver */
void test_ata_driver() {
  int result;
  char buffer[16];
  ata_device_info drive_info;

  /* Print test header */
  put_string("ATA Driver Test Starting\n");

  /* Initialize the ATA driver */
  put_string("Initializing ATA driver...\n");
  result = ata_init();
  if (result != 0) {
    put_string("ATA initialization failed with code: ");
    int_to_hex_string(result, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
    return;
  }

  /* Identify primary master drive */
  put_string("Identifying primary master drive...\n");
  result = ata_identify_drive(0, 0, &drive_info);
  if (result != 0) {
    put_string("Drive identification failed with code: ");
    int_to_hex_string(result, buffer, sizeof(buffer));
    put_string(buffer);
    put_string("\n");
    return;
  }

  /* Print drive information */
  put_string("Drive information:\n");
  ata_print_drive_info(&drive_info);

  /* Only proceed if it's an ATA device */
  if (drive_info.type == DEVICE_TYPE_ATA) {
    /* Test partition selection (use the first 100MB) */
    put_string("Selecting test partition (first 100MB)...\n");
    uint32_t sector_count = 204800; /* 100MB = 204800 sectors of 512 bytes */
    result = ata_select_partition(0, sector_count);
    if (result != 0) {
      put_string("Partition selection failed with code: ");
      int_to_hex_string(result, buffer, sizeof(buffer));
      put_string(buffer);
      put_string("\n");
      return;
    }

    /* Allocate buffer for sector data */
    uint8_t sector_buffer[512];

    /* Test reading the first sector */
    put_string("Reading first sector...\n");
    result = ata_read_sectors(0, 1, sector_buffer);
    if (result != 0) {
      put_string("Sector read failed with code: ");
      int_to_hex_string(result, buffer, sizeof(buffer));
      put_string(buffer);
      put_string("\n");
      return;
    }

    /* Display first few bytes of the sector */
    put_string("First 16 bytes of sector 0:\n");
    for (int i = 0; i < 16; i++) {
      int_to_hex_string(sector_buffer[i], buffer, sizeof(buffer));
      put_string(buffer);
      put_string(" ");

      /* Line break every 8 bytes */
      if (i == 7) {
        put_string("\n");
      }
    }
    put_string("\n");

    /* Test is complete */
    put_string("ATA driver test completed successfully!\n");
  } else {
    put_string("No ATA device found, test aborted.\n");
  }
}

void main() {
  char *video_memory = (char *)0xb8000;
  char message[] = "Hello World\n";
  put_string(message);
  isr_install();
  pci_device devices[16];
  pci_device *ptrs[16];
  __asm__ volatile("sti");
  // put_string("Enabling paging\n");
  // enable_new_paging();
  // enable_paging_asm();

  // enable_paging();
  // put_string("Paging is enabled \n");
  put_string("Initializing the pci array \n");
  int cnt = 0;
  for (int i = 0; i < 16; i++) {

    devices[i].vendor = 0xFFFF;
    ptrs[i] = &devices[i];
  }
  put_string("Running syscall");
  __asm__("movl $0x1234, %eax\n"
          "movl $0x4321, %ebx\n"
          "int $0x8");
  uint16_t identify_data[256];

  ata_device_info device_info;

  /*put_string("Identifying master drive...\n");*/
  /*test_ata_driver();*/
  // int result = identify_drive(0, identify_data, &device_info);
  // switch (result) {
  // case 0:
  //   put_string("Drive identified successfully\n");
  //   // Process identify_data here
  //   put_string("Type: ");
  //   char buffer[10];
  //   int_to_hex_string(device_info.type, buffer, 10);
  //   put_string(buffer);
  //   break;
  // case -1:
  //   put_string("Drive does not exist\n");
  //   break;
  // case -2:
  //   put_string("Not an ATA drive\n");
  //   break;
  // case -3:
  //   put_string("Error during identification\n");
  //   break;
  // }
  char buffer[16];
  put_string("Starting the Shell now, keyboard is loaded \n");
  put_string(" >>>>");
  init_paging();
  //0x10838 - 0x1001C
  init_heap((void*)0x10000,2048 * 16);
  put_string("Paging and memory maping enabled. \n");
  void* page = kmalloc(2048);
  for(int i = 0; i < 2048; i++) {
    *(char*)(page + i) = 'A';
  }
  int_to_hex_string((unsigned long)page, buffer, 16);
  put_string("Page value: ");
  put_string(buffer);
  void* page2 = kmalloc(2048);
  for(int i = 0; i < 2048; i++) {
    *(char*)(page2 + i) = 'B';
  }
  int_to_hex_string((unsigned long)page2, buffer, 16);
  put_string("Page value: ");
  put_string(buffer);
  put_string("\n Sanity check \n");
  for (int i = 0; i < 2048; i++) {
    if (*(char*)(page + i) != 'A') {
      put_string("Error in memory allocation \n");
      break;
    }
  }
  for (int i = 0; i < 2048; i++) {
    if (*(char*)(page2 + i) != 'B') {
      put_string("Error in memory allocation \n");
      break;
    }
  }
  put_string("Memory allocation is working \n");
    put_string("Now scanning PCI bus. \n");
  pci_device network;
  // scan_pci_bus();
  int device_amnt = pci_print_devices(ptrs);
  for (int i = 0; i < device_amnt; i++) {
    if (devices[i].device == NET_CARD_ID) {
      put_string("Found a network card ");
      int_to_hex_string((uint32_t)ptrs[i], buffer, 16);
      put_string(buffer);
      put_string("\n");
      network = devices[i];
    }
  }
  read_bars(network);
  enable_network_card(network);
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
  // *(video_memory + 100) = 'V';
  // *(video_memory + 101) = 0x0f;
}
