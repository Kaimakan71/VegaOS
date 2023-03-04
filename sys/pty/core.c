/* author: Ian Marco Moffett */

#include <pty/core.h>
#include <pty/font.h>
#include <dev/video/fb.h>

#define GLYPH_SIZE ((PTY_FONT_WIDTH + 7) / 8 * PTY_FONT_HEIGHT)

static uint32_t cursor_x = 0, cursor_y = 0;

static void pty_putch(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg)
{
  c -= 32;
  for (uint32_t cx = 0; cx < PTY_FONT_WIDTH; ++cx) 
  {
    for (uint32_t cy = 0; cy < PTY_FONT_HEIGHT; ++cy) 
    {
      uint16_t col = (DEFAULT_FONT_DATA[(uint64_t)c * PTY_FONT_WIDTH + cx] >> cy) & 1;
      fb_putpix(x + cx, y + cy, col ? fg : bg);
    }
  }
}

static void newline(void)
{
  cursor_y += PTY_FONT_HEIGHT;
  cursor_x = 0;
}

void pty_putstr(const char *str, uint32_t color)
{
  while (*str)
  {
    switch (*str)
    {
      case '\n':
        newline();
        ++str;
        break;
    }
    
    pty_putch(cursor_x, cursor_y, *str, color, 0x000000);
    ++str;
    cursor_x += PTY_FONT_WIDTH;
  }
}
