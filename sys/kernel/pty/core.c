/*
 * Copyright (c) 2023 Ian Marco Moffett and the VegaOS team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of VegaOS nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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
        continue;
    }
    
    pty_putch(cursor_x, cursor_y, *str, color, 0x000000);
    ++str;
    cursor_x += PTY_FONT_WIDTH;
  }
}
