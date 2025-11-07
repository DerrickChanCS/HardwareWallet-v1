#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "ssd1306.h"

#define LINE_HEIGHT 16

void oled_init();
void oled_clear();
void oled_redraw();
void oled_invert(bool invert);
void oled_write(uint8_t x, uint8_t line, const char *text);

int a2hexstr(char *dest, const uint8_t *src, int len);
