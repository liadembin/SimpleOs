#ifndef VGA_H
#define VGA_H
#include "ports.h"
#include <stdint.h>

enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE = 1,
  VGA_COLOR_GREEN = 2,
  VGA_COLOR_CYAN = 3,
  VGA_COLOR_RED = 4,
  VGA_COLOR_MAGENTA = 5,
  VGA_COLOR_BROWN = 6,
  VGA_COLOR_LIGHT_GREY = 7,
  VGA_COLOR_DARK_GREY = 8,
  VGA_COLOR_LIGHT_BLUE = 9,
  VGA_COLOR_LIGHT_GREEN = 10,
  VGA_COLOR_LIGHT_CYAN = 11,
  VGA_COLOR_LIGHT_RED = 12,
  VGA_COLOR_LIGHT_MAGENTA = 13,
  VGA_COLOR_LIGHT_BROWN = 14,
  VGA_COLOR_WHITE = 15,
};

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define VIDEO_MEMORY 0xb8000
#define CURSOR_PORT_COMMAND 0x3D4
#define CURSOR_PORT_DATA 0x3D5

typedef struct {
  int x;
  int y;
} Cursor_position;

Cursor_position get_cursor_position();
void set_cursor_position(int x, int y);
void put_pixel(char val, char col, int x, int y);
char get_pixel(int x, int y);
void clear_row(int row);
void scroll_line();
void clear_pixel(Cursor_position cursor);
void clear_screen();
void put_string(char *string);
#endif
