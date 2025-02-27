#include "vga.h"
#include "ports.h"
#include "stdlib.h"
Cursor_position get_cursor_position() {
  unsigned short pos = 0;
  Cursor_position cursor;

  outb(CURSOR_PORT_COMMAND, 0x0F);
  pos |= inb(CURSOR_PORT_DATA);
  outb(CURSOR_PORT_COMMAND, 0x0E);
  pos |= ((unsigned short)inb(CURSOR_PORT_DATA)) << 8;

  cursor.x = pos % SCREEN_WIDTH;
  cursor.y = pos / SCREEN_WIDTH;

  return cursor;
}

void set_cursor_position(int x, int y) {
  unsigned short pos = y * SCREEN_WIDTH + x;
  outb(CURSOR_PORT_COMMAND, 0x0F);
  outb(CURSOR_PORT_DATA, (unsigned char)(pos & 0xFF));
  outb(CURSOR_PORT_COMMAND, 0x0E);
  outb(CURSOR_PORT_DATA, (unsigned char)((pos >> 8) & 0xFF));
}

void put_pixel(char val, char col, int x, int y) {
  char final_col = col;
  if (col == '\0')
    final_col = 0x0f;
  unsigned int offset = (y * SCREEN_WIDTH + x) * 2;
  char *video_memory = (char *)VIDEO_MEMORY;
  video_memory[offset] = val;
  video_memory[offset + 1] = final_col;
}
char get_pixel(int x, int y) {
  unsigned int offset = (y * SCREEN_WIDTH + x) * 2;
  char *video_memory = (char *)VIDEO_MEMORY;
  return video_memory[offset];
}

void clear_row(int row) {
  char *video_memory = (char *)VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_WIDTH; i++) {
    video_memory[(row * SCREEN_WIDTH + i) * 2] = ' '; // Space character
    video_memory[(row * SCREEN_WIDTH + i) * 2 + 1] =
        0x07; // Light gray on black
  }
}
void scroll_line() {
  char *video_memory = (char *)VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_HEIGHT - 1; i++) {
    memcpy(video_memory + i * SCREEN_WIDTH * 2,
           video_memory + (i + 1) * SCREEN_WIDTH * 2, SCREEN_WIDTH * 2);
  }
  clear_row(SCREEN_HEIGHT - 1);
}
void clear_pixel(Cursor_position cursor) {
  // Cursor_position hold = get_cursor_position();
  // set_cursor_position(cursor.x, cursor.y);
  put_pixel(' ', 0x07, cursor.x, cursor.y);
}
void clear_screen() {
  for (int i = 0; i < SCREEN_HEIGHT; i++) {
    clear_row(i); // might as well just write raw to buffer but idkk...
  }
}

void put_string(char *string) {
  // This is some shit ass code but its low level....
  Cursor_position cursor = get_cursor_position();
  for (int i = 0; string[i] != '\0'; i++) {
    if (string[i] == '\n') {
      cursor.x = 0;
      cursor.y++;
    } else if (string[i] == '\10') { // Backspace
      cursor.x = 0;
    } else if (string[i] == '\13') { // Vertical tab
      cursor.y++;
    } else if (string[i] == ' ') {
      cursor.x++;
    } else {
      put_pixel(string[i], 0x0f, cursor.x, cursor.y);
      cursor.x++;
      if (cursor.x >= SCREEN_WIDTH) {
        cursor.x = 0;
        cursor.y++;
      }
    }

    if (cursor.y >= SCREEN_HEIGHT) {
      scroll_line();
      cursor.y = SCREEN_HEIGHT - 1;
    }

    set_cursor_position(cursor.x, cursor.y);
  }
}
