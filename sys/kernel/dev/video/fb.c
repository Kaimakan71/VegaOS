#include <dev/video/fb.h>
#include <sys/limine.h>
#include <sys/pal.h>

#define FRAMEBUFFER framebuffer_req.response->framebuffers[0]

static volatile struct limine_framebuffer_request framebuffer_req = {
  .id = LIMINE_FRAMEBUFFER_REQUEST,
  .revision = 0
};

/*
 *  Does not continue if
 *  a framebuffer is not available.
 *
 *  TODO: Continue but just without
 *        using the framebuffer.
 */

static void fb_assert(void)
{
  if (framebuffer_req.response == NULL)
  {
    global_irq_disable();
    halt();
  }
}

static uint32_t get_index(uint32_t x, uint32_t y)
{
  fb_assert();
  return x + y * (FRAMEBUFFER->pitch/4);
}

int fb_putpix(uint32_t x, uint32_t y, uint32_t color)
{
  if (x >= fb_get_width() || y >= fb_get_height())
  {
    return 1;
  }
  
  uint32_t *fb = (uint32_t *)FRAMEBUFFER->address;
  fb[get_index(x, y)] = color;
  return 0;
}

void fb_clear(uint32_t color)
{
  for (uint32_t y = 0; y < fb_get_height(); ++y)
  {
    for (uint32_t x = 0; x < fb_get_width(); ++x)
    {
      fb_putpix(x, y, color);
    }
  }
}

uint32_t fb_get_height(void)
{
  return FRAMEBUFFER->height;
}

uint32_t fb_get_width(void)
{
  return FRAMEBUFFER->width;
}
