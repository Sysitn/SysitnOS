#pragma once
#include <stdint.h>

void pmm_init();
uint64_t pmm_alloc_page(void);
void pmm_free_page(uint64_t phys);
uint64_t pmm_count_free_pages(void);