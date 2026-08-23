#pragma once
#include <stdbool.h>
#include <stddef.h>

void uart_init();
bool uart_available(void);
void uart_write(const char *str, size_t len);