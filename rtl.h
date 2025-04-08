#ifndef RTL_H
#define RTL_H
#include "pci.h"
#include "reg.h"
void enable_rtl8139(pci_device device);
void rtl_handler(registers_t *reg);
#endif
