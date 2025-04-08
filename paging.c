#include "paging.h"
#include <stdint.h>

// Page directory and tables - statically allocated and aligned to 4KB
// boundaries
static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));
static uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));

void init_paging() {
  // Clear all tables to ensure all pages are initially not present
  for (int i = 0; i < 1024; i++) {
    // Mark all pages as not present
    page_directory[i] = 0x00000002;    // Not present, but writable
    first_page_table[i] = 0x00000002;  // Not present, but writable
    kernel_page_table[i] = 0x00000002; // Not present, but writable
  }

  // Identity map the first 1MB (256 pages of 4KB each)
  for (uint32_t i = 0; i < 256; i++) {
    // Present, read/write, supervisor
    first_page_table[i] = (i * 0x1000) | (PTE_PRESENT_BIT | PTE_READ_WRITE_BIT);
  }

  // Map the kernel to higher half (at 0xC0000000 virtual address)
  // Kernel is loaded at physical 0x1000 and occupies up to 54 sectors (27KB)
  // Let's map a bit more to be safe, say 1MB starting from 0x1000
  for (uint32_t i = 0; i < 256; i++) {
    // Map physical addresses starting at 0x1000 to virtual addresses starting
    // at 0xC0001000
    kernel_page_table[i] =
        ((0x1000 + i * 0x1000)) | (PTE_PRESENT_BIT | PTE_READ_WRITE_BIT);
  }

  // Set up page directory entries
  // Entry 0: Identity mapping for the first 1MB
  page_directory[0] =
      ((uint32_t)first_page_table) | (PDE_PRESENT_BIT | PDE_READ_WRITE_BIT);

  // Entry 768 (0xC0000000 >> 22): Higher half kernel mapping
  page_directory[768] =
      ((uint32_t)kernel_page_table) | (PDE_PRESENT_BIT | PDE_READ_WRITE_BIT);

  // Load page directory address into CR3 and enable paging in CR0
  load_page_directory(page_directory);
  enable_paging();
}

void load_page_directory(uint32_t *page_dir) {
  __asm__ volatile("mov %0, %%cr3" : : "r"(page_dir));
}

void enable_paging() {
  uint32_t cr0;
  __asm__ volatile("mov %%cr0, %0\n\t"
                   "or $0x80000000, %0\n\t"
                   "mov %0, %%cr0"
                   : "=r"(cr0)
                   :
                   : "memory");
}

void *map_page(uint32_t physical_addr, uint32_t virtual_addr, uint32_t flags) {
  uint32_t pd_index = virtual_addr >> 22;
  uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;

  // Check if page directory entry is present
  if (!(page_directory[pd_index] & PDE_PRESENT_BIT)) {
    // Allocate a new page table
    uint32_t *new_page_table = kmalloc_aligned(4096, 4096);
    if (!new_page_table) {
      return NULL; // Out of memory
    }

    // Initialize new page table
    for (int i = 0; i < 1024; i++) {
      new_page_table[i] = 0x00000002; // Not present, but writable
    }

    // Add the new page table to the page directory
    page_directory[pd_index] =
        ((uint32_t)new_page_table) | (PDE_PRESENT_BIT | PDE_READ_WRITE_BIT);
  }

  // Get the page table
  uint32_t *page_table = (uint32_t *)(page_directory[pd_index] & 0xFFFFF000);

  // Map the physical address to the virtual address
  page_table[pt_index] = (physical_addr & 0xFFFFF000) | flags;

  // Invalidate TLB for this page
  __asm__ volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");

  return (void *)virtual_addr;
}

void unmap_page(uint32_t virtual_addr) {
  uint32_t pd_index = virtual_addr >> 22;
  uint32_t pt_index = (virtual_addr >> 12) & 0x3FF;

  // Make sure the page directory entry is present
  if (page_directory[pd_index] & PDE_PRESENT_BIT) {
    uint32_t *page_table = (uint32_t *)(page_directory[pd_index] & 0xFFFFF000);

    // Unmap by marking not present
    page_table[pt_index] = 0x00000002; // Not present, writable

    // Invalidate TLB entry
    __asm__ volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
  }
}

typedef struct {
  size_t size;        // Size of the block
  uint8_t is_free;    // 1 if free, 0 if allocated
  uint32_t alignment; // Alignment of this block
} BlockMetadata;

// Start and end of the heap
static void *heap_start = NULL;
static void *heap_end = NULL;

// Free list head
static DoubleNode *free_list_head = NULL;

// Function to initialize the heap
void init_heap(void *start, size_t size) {
  if (heap_start != NULL) {
    return; // Already initialized
  }

  heap_start = start;
  heap_end = (void *)((uintptr_t)start + size);

  // Create initial block covering the entire heap
  DoubleNode *initial_node = (DoubleNode *)heap_start;
  BlockMetadata *metadata =
      (BlockMetadata *)((uintptr_t)heap_start + sizeof(DoubleNode));

  // Set up the initial block
  metadata->size = size - sizeof(DoubleNode) - sizeof(BlockMetadata);
  metadata->is_free = 1;
  metadata->alignment = sizeof(void *); // Default alignment

  // Set up the initial node
  initial_node->value = (void *)((uintptr_t)metadata + sizeof(BlockMetadata));
  initial_node->metadata = metadata;
  initial_node->next = NULL;
  initial_node->prev = NULL;

  // Initialize the free list
  free_list_head = initial_node;
  // return 1/0;
}

// Function to find a suitable free block
static DoubleNode *find_free_block(size_t size, uint32_t alignment) {
  DoubleNode *current = free_list_head;
  // cant seem to find a free block?
  while (current != NULL) {
    BlockMetadata *metadata = (BlockMetadata *)current->metadata;

    if (metadata->is_free) {
      // Calculate aligned address
      uintptr_t block_addr = (uintptr_t)current->value;
      uintptr_t aligned_addr = (block_addr + alignment - 1) & ~(alignment - 1);
      size_t adjustment = aligned_addr - block_addr;

      // Check if the block is big enough after alignment adjustment
      if (metadata->size >= size + adjustment) {
        return current;
      }
    }

    current = current->next;
  }

  return NULL;
}

// Function to split a block if needed
static void split_block(DoubleNode *node, size_t size, uint32_t alignment) {
  BlockMetadata *metadata = (BlockMetadata *)node->metadata;

  // Calculate aligned address
  uintptr_t block_addr = (uintptr_t)node->value;
  uintptr_t aligned_addr = (block_addr + alignment - 1) & ~(alignment - 1);
  size_t adjustment = aligned_addr - block_addr;

  // Get original block size
  size_t original_size = metadata->size;

  // Calculate remaining size after allocation
  size_t remaining_size = original_size - size - adjustment;

  // If remaining size is sufficient for a new block, split it
  if (remaining_size >
      sizeof(DoubleNode) + sizeof(BlockMetadata) + sizeof(void *)) {
    // Adjust current block's size
    metadata->size = size;
    metadata->is_free = 0;
    metadata->alignment = alignment;

    // Create a new block from the remaining space
    DoubleNode *new_node =
        (DoubleNode *)((uintptr_t)node->value + size + adjustment);
    BlockMetadata *new_metadata =
        (BlockMetadata *)((uintptr_t)new_node + sizeof(DoubleNode));

    // Set up new metadata
    new_metadata->size =
        remaining_size - sizeof(DoubleNode) - sizeof(BlockMetadata);
    new_metadata->is_free = 1;
    new_metadata->alignment = sizeof(void *);

    // Set up new node
    new_node->value = (void *)((uintptr_t)new_metadata + sizeof(BlockMetadata));
    new_node->metadata = new_metadata;

    // Insert into the doubly linked list
    new_node->next = node->next;
    new_node->prev = node;

    if (node->next != NULL) {
      node->next->prev = new_node;
    }

    node->next = new_node;
  } else {
    // If not enough space to split, just mark the entire block as used
    metadata->is_free = 0;
    metadata->alignment = alignment;
  }
}

// Function to return aligned memory address
static void *align_address(void *addr, uint32_t alignment) {
  uintptr_t address = (uintptr_t)addr;
  uintptr_t aligned = (address + alignment - 1) & ~(alignment - 1);
  return (void *)aligned;
}
#include "vga.h"
// Allocate aligned memory
void *kmalloc_aligned(uint32_t size, uint32_t alignment) {
  if (size == 0 || alignment == 0) {
    put_string("Invalid size or alignment\n");
    return 1 / 0;
    return NULL;
  }

  // Make sure alignment is a power of 2
  if (alignment & (alignment - 1)) {
    // Find the next power of 2
    uint32_t power = 1;
    while (power < alignment) {
      power <<= 1;
    }
    alignment = power;
  }

  // Make sure alignment is at least the size of a pointer
  if (alignment < sizeof(void *)) {
    alignment = sizeof(void *);
  }

  // Find a suitable free block
  DoubleNode *block = find_free_block(size, alignment);
  if (block == NULL) {
    // return 1/0;
    return NULL; // Out of memory
  }

  // Split the block if needed
  split_block(block, size, alignment);

  // Calculate aligned address
  void *aligned_addr = align_address(block->value, alignment);

  return aligned_addr;
}

// Free allocated memory
void kfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  // Find the node containing this pointer
  DoubleNode *current = free_list_head;

  while (current != NULL) {
    uintptr_t block_start = (uintptr_t)current->value;
    BlockMetadata *metadata = (BlockMetadata *)current->metadata;
    uintptr_t block_end = block_start + metadata->size;

    if ((uintptr_t)ptr >= block_start && (uintptr_t)ptr < block_end) {
      // Found the block
      metadata->is_free = 1;

      // Try to merge with adjacent free blocks
      merge_blocks(current);
      return;
    }

    current = current->next;
  }
}

// Merge adjacent free blocks
static void merge_blocks(DoubleNode *node) {
  if (node == NULL) {
    return;
  }

  BlockMetadata *metadata = (BlockMetadata *)node->metadata;

  // Check if we can merge with the next block
  if (node->next != NULL) {
    BlockMetadata *next_metadata = (BlockMetadata *)node->next->metadata;

    if (next_metadata->is_free) {
      // Calculate combined size
      metadata->size +=
          sizeof(DoubleNode) + sizeof(BlockMetadata) + next_metadata->size;

      // Remove the next node from the list
      DoubleNode *next_next = node->next->next;
      node->next = next_next;

      if (next_next != NULL) {
        next_next->prev = node;
      }
    }
  }

  // Check if we can merge with the previous block
  if (node->prev != NULL) {
    BlockMetadata *prev_metadata = (BlockMetadata *)node->prev->metadata;

    if (prev_metadata->is_free) {
      // Calculate combined size
      prev_metadata->size +=
          sizeof(DoubleNode) + sizeof(BlockMetadata) + metadata->size;

      // Remove the current node from the list
      node->prev->next = node->next;

      if (node->next != NULL) {
        node->next->prev = node->prev;
      }
    }
  }
}

// Get total free memory
size_t get_free_memory() {
  size_t total_free = 0;
  DoubleNode *current = free_list_head;

  while (current != NULL) {
    BlockMetadata *metadata = (BlockMetadata *)current->metadata;

    if (metadata->is_free) {
      total_free += metadata->size;
    }

    current = current->next;
  }

  return total_free;
}

// Simple kmalloc without alignment requirements
void *kmalloc(uint32_t size) { return kmalloc_aligned(size, sizeof(void *)); }
