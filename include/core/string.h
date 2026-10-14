#pragma once
#include <stdint.h>
#include <stddef.h>

char *strstr(const char *haystack, const char *needle);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
size_t strlen(const char *s);