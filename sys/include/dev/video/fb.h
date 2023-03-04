/* author: Ian Marco Moffett */

#ifndef _VIDEO_FB_H_
#define _VIDEO_FB_H_

#include <sys/types.h>


/*
 *  Places a pixel on the screen.
 *
 *  Returns 0 if success, otherwise 1 if
 *  out of bounds.
 */

int fb_putpix(uint32_t x, uint32_t y, uint32_t color);

/*
 *  Clears the screen to a specific
 *  color value.
 */

void fb_clear(uint32_t color);

/*
 *  Returns the height
 *  of the framebuffer.
 */

uint32_t fb_get_height(void);

/*
 *  Returns the width
 *  of the framebuffer.
 */

uint32_t fb_get_width(void);

#endif
