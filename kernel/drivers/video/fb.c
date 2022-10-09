/*
   Copyright 2022 Pigmy-penguin

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

           http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <drivers/video/fb.h>
#include <drivers/video/psf.h>
#include <drivers/video/tga.h>

#include <drivers/char/serial.h>
#include <kernel/string.h>

u32 *fb;
u16 width;
u16 height;
u16 pitch;
u16 a;

static int x_pos;
static int y_pos;

static int fg_color = DEFAULT_FG_COLOR;
static int bg_color = DEFAULT_BG_COLOR;

extern volatile unsigned char _binary_font_psf_start;
struct psf_font *font = (struct psf_font *)&_binary_font_psf_start;
static u16 *first_glyph;
static u32 font_w_bytes;

void putpixel(int x, int y, u32 color) { fb[y * (a) + x] = color; }

static void scroll(void) {
  int screensiz = pitch * 16 * (height / 16) / sizeof(u32);
  int rowsiz = pitch * 16 / sizeof(u32);
  y_pos--;
  for (int i = 0; i < screensiz - rowsiz; i++) {
    fb[i] = fb[i + rowsiz];
  }
}

void cls(void) {
  for (u32 i = 0; i < width * pitch / sizeof(u32); i++) {
    fb[i] = bg_color;
  }
  x_pos = 0;
  y_pos = 0;
}

static inline void set_color(u32 fg, u32 bg) {
  fg_color = fg;
  bg_color = bg;
}

static inline void draw_icon(int x, int y, int w, int h, u32 *img) {
  int j, l, i;

  for (l = j = 0; l < h; l++) {
    for (i = 0; i < w; i++, j++) {
      putpixel(x + i, y + l, img[j]);
    }
  }
}

void display_tga(struct tga_header *base) {
  if (base->magic1 != 0 || base->colormap != 0 || base->encoding != 2 ||
      base->cmaporig != 0 || base->cmapent != 0 || base->x != 0 ||
      base->bpp != 32 || base->pixeltype != 40)
    return;
  u32 *img = (u32 *)(sizeof(struct tga_header) + (u64)base);
  draw_icon(700, 50, base->w, base->h, img);
}

static u8 reverse(u8 b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

void fb_puts(const char *str) {
  while (*str)
    fb_putchar(*str++);
}

void fb_putchar(char c) {
  if (c == '\n')
    goto newline;
  switch (c) {
  case '\n':
    goto newline;
  case '^':
    set_color(0xf54263, DEFAULT_BG_COLOR);
    goto new_color;
  case '~':
    set_color(DEFAULT_FG_COLOR, DEFAULT_BG_COLOR);
    goto new_color;
  case '!':
    set_color(0x34eba8, DEFAULT_BG_COLOR);
    goto new_color;
  case '*':
    set_color(0xf8e1af, DEFAULT_BG_COLOR);
    goto new_color;
  case '$':
    set_color(DEFAULT_FG_COLOR, 0xf72202);
    goto new_color;
  }
  u32 x_pix = x_pos * font->width;
  u32 y_pix = y_pos * font->height;
  u8 *glyph = (u8 *)&_binary_font_psf_start + font->headersize +
              c * font->bytesperglyph;
  const u32 arr[] = {bg_color, fg_color};

  if (font_w_bytes == 2) {
    for (u32 height = 0; height < font->height; height++) {
      for (u32 width = 0; width < 9 /* = 8-1 */; width++) {
        putpixel(x_pix + width, y_pix + height,
                 arr[(reverse(glyph[0]) >> width) & 1]);
      }
      for (u32 width = 10; width < font->width; width++) {
        putpixel(x_pix + width, y_pix + height,
                 arr[(reverse(glyph[1]) >> width) & 1]);
      }
      glyph += font_w_bytes;
    }
  }

  else if (font_w_bytes == 1) {
    for (u32 height = 0; height < font->height; height++) {
      for (u32 width = 0; width < 8; width++) {
        putpixel(x_pix + width, y_pix + height,
                 arr[(reverse(glyph[0]) >> width) & 1]);
      }
    }
  }

  else
    return;

  x_pos++;
  if (x_pos >= width / 8) {
  newline:
    y_pos++;
    x_pos = 0;
    fb_putchar(' ');
  }
new_color:
  if (y_pos >= height / 16 - 1) {
    scroll();
  }
}

int init_fb(void *fb_addr, u16 fb_width, u16 fb_height, u16 fb_pitch) {
  fb = (u32 *)fb_addr;
  width = fb_width;
  height = fb_height;
  pitch = fb_pitch;
  a = pitch / sizeof(u32);

  x_pos = 0;
  y_pos = 0;

  first_glyph = (u16 *)&_binary_font_psf_start + font->headersize;
  if (font->magic != PSF_FONT_MAGIC)
    return 1;

  if (font->flags) {
    serial_print("Unicode is not supported. Please use another font!");
    return 1;
  }
  font_w_bytes = font->bytesperglyph / font->height;
  return 0;
}

void show_logo() {
  struct tga_header *logo = (struct tga_header *)&logo_tga;
  display_tga(logo);
}
