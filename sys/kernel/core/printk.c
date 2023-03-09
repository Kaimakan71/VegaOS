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

#include <sys/printk.h>
#include <pty/core.h>
#include <string.h>

#define as_str(chr) (char[2]){chr, 0}

static char* hex2str(uint64_t hex_num) 
{
    static char hex_string[80];
    char* ascii_numbers = "0123456789ABCDEF";
    uint8_t nibble;
    uint8_t i = 0, j, temp;
    uint8_t pad = 0;

    /* If passed in 0, print a 0 */
    if (hex_num == 0) 
    {
        memcpy(hex_string, "0\0", 2);
        i = 1;
    }

    if (hex_num < 0x10) pad = 1;    /* If one digit, will pad out to 2 later */

    while (hex_num > 0) 
    {
        /* Convert hex values to ascii string */
        nibble = (uint8_t)hex_num & 0x0F;  /* Get lowest 4 bits */
        nibble = ascii_numbers[nibble];    /* Hex to ascii */
        hex_string[i] = nibble;            /* Move ascii char into string */
        hex_num >>= 4;                     /* Shift right by 4 for next nibble */
        i++;
    }

    if (pad) hex_string[i++] = '0';  /* Pad out string with extra 0 */

    /* Add initial "0x" to front of hex string */
    hex_string[i++] = 'x';
    hex_string[i++] = '0';
    hex_string[i] = '\0';   /* Null terminate string */

    // Number is stored backwards in hex_string, reverse the string by swapping ends
    //   until they meet in the middle
    i--;    // Skip null byte
    for (j = 0; j < i; j++, i--) 
    {
        temp          = hex_string[j];
        hex_string[j] = hex_string[i];
        hex_string[i] = temp;
    }

    // Print hex string
    return hex_string;
}

static char* dec2str(size_t number) 
{
    static char dec_string[80];
    uint8_t i = 0, j, temp;
    uint8_t negative = 0;       /* Is number negative? */

    if (number == 0) 
    {
      dec_string[i++] = '0';  /* If passed in 0, print a 0 */
    }
    else if (number < 0)  
    {
        negative = 1;       /* Number is negative */
        number = -number;   /* Easier to work with positive values */
    }

    while (number > 0) 
    {
        dec_string[i] = (number % 10) + '0';
        number /= 10;
        i++;
    }

    if (negative)
    {
      dec_string[i++] = '-';
    }

    dec_string[i] = '\0';

    i--;
    for (j = 0; j < i; j++, i--) 
    {
        temp          = dec_string[j];
        dec_string[j] = dec_string[i];
        dec_string[i] = temp;
    }

    return dec_string;
}

static uint32_t get_color(char color_code)
{
  switch (color_code)
  {
    case KLOGCOLOR_DEFAULT:
      return PRINTK_DEFAULT_FG;
    case KLOGCOLOR_RED:
      return 0xFF0000;
    case KLOGCOLOR_WHITE:
      return 0xFFFFFF;
    case KLOGCOLOR_JADE:
      return 0x00A36C;
    case KLOGCOLOR_YELLOW:
      return 0xF8A611;
    case KLOGCOLOR_PURPLE:
      return 0xBF40BF;
    default:
      return PRINTK_DEFAULT_FG;
  }
}

static void handle_format(char fmt_char, va_list ap, uint32_t color)
{
  switch (fmt_char)
  {
    case 's':
      pty_putstr(va_arg(ap, const char*), color);
      break;
    case 'c':
      pty_putstr(as_str(va_arg(ap, int)), color);
      break;
    case 'x':
      pty_putstr(hex2str(va_arg(ap, uint64_t)), color);
      break;
    case 'd':
      pty_putstr(dec2str(va_arg(ap, uint64_t)), color);
      break;
  }
}

void vprintk(const char *fmt, va_list ap)
{
  uint32_t color = PRINTK_DEFAULT_FG;

  while (*fmt)
  {
    if (*fmt == '\001')
    {
      ++fmt;
      color = get_color(*fmt++);
      continue;
    }

    if (*fmt == '%')
    {
      ++fmt;
      handle_format(*fmt, ap, color);
    }
    else
    {
      pty_putstr(as_str(*fmt), color);
    }

    ++fmt;
  }
}

void printk(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vprintk(fmt, ap);
  va_end(ap);
}
