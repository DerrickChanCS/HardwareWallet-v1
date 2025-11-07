#pragma once
#include <stdint.h>

uint8_t hex2int(char c);
void hexstr2a(uint8_t *dest, const char *src, int len);
int a2hexstr(char *dest, const uint8_t *src, int len);
