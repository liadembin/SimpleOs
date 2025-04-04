#ifndef PAGING_H
#define PAGING_H
#include "ports.h"
// #include "vga.h"
#include <stdint.h>
#include <stddef.h>
// #include "kernel.h"

/* Page Directory Entry (PDE) flags */
#define PDE_PRESENT_BIT        0x00000001 // Page is present in memory
#define PDE_READ_WRITE_BIT     0x00000002 // Page is writable (0 = read-only)
#define PDE_USER_USERVISOR_BIT 0x00000004 // Page access level (0 = supervisor only, 1 = user)
#define PDE_WRITE_THROUGH_BIT  0x00000008 // Page cache write-through enabled
#define PDE_CACHE_DISABLE_BIT  0x00000010 // Page cache is disabled
#define PDE_ACCESSED_BIT       0x00000020 // Page has been accessed
#define PDE_DIRTY_BIT          0x00000040 // Page has been written to (only valid for 4MB pages)
#define PDE_IS_4MB_BIT         0x00000080 // Page size is 4MB (0 = 4KB)
#define PDE_GLOBAL_BIT         0x00000100 // Page is global (not flushed from TLB)
#define PDE_AVAILABLE          0x00000E00 // Bits 9-11 available for OS use

/* Page Table Entry (PTE) flags */
#define PTE_PRESENT_BIT        0x00000001 // Page is present in memory
#define PTE_READ_WRITE_BIT     0x00000002 // Page is writable (0 = read-only)
#define PTE_USER_USERVISOR_BIT 0x00000004 // Page access level (0 = supervisor only, 1 = user)
#define PTE_WRITE_THROUGH_BIT  0x00000008 // Page cache write-through enabled
#define PTE_CACHE_DISABLE_BIT  0x00000010 // Page cache is disabled
#define PTE_ACCESSED_BIT       0x00000020 // Page has been accessed
#define PTE_DIRTY_BIT          0x00000040 // Page has been written to
#define PTE_PAT_BIT            0x00000080 // Page Attribute Table bit
#define PTE_GLOBAL_BIT         0x00000100 // Page is global (not flushed from TLB)
#define PTE_AVAILABLE          0x00000E00 // Bits 9-11 available for OS use

/* Memory constants */
#define PAGE_SIZE              4096       // Size of a page (4KB)
#define PAGE_TABLE_ENTRIES     1024       // Number of entries in a page table
#define KERNEL_VIRTUAL_BASE    0xC0000000 // Virtual base address of kernel (3GB)
#define KERNEL_PHYSICAL_BASE   0x1000     // Physical base address of kernel

/* Paging function declarations */
void init_paging();
void load_page_directory(uint32_t* page_dir);
void enable_paging();
void* map_page(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags);
void unmap_page(uint32_t virtual_addr);
void* kmalloc_aligned(uint32_t size, uint32_t alignment);

/* Memory management structures and functions */

// Double linked list node structure
typedef struct DoubleNode {
    void* value;
    void* metadata;
    struct DoubleNode* next;
    struct DoubleNode* prev;
} DoubleNode;

// Memory management function declarations
void init_heap(void* start, size_t size);
void* kmalloc(uint32_t size);
void* kmalloc_aligned(uint32_t size, uint32_t alignment);
void kfree(void* ptr);
static void merge_blocks(DoubleNode* node);
size_t get_free_memory();

#endif
