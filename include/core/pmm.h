#pragma once
#include <stdint.h>
#include <boot/boot_info.h>

void pmm_init(const struct sysitn_memmap *memmap, uintptr_t hhdm_offset);
phys_addr_t pmm_alloc_page(void);
void pmm_free_page(phys_addr_t phys);
uint64_t pmm_count_free_pages(void);