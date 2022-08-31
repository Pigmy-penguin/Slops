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

#include <drivers/graphics/fb.h>
#include <drivers/graphics/tga.h>

#include <drivers/char/serial.h>
#include <kernel/string.h>

u32 *fb;
u16 width;
u16 height;
u16 pitch;
u16 a;

static inline void draw_icon(int x, int y, int w, int h, u32 *img) 
{
   int j, l, i;

   for (l = j = 0; l < h; l++) {
      for (i = 0; i < w; i++, j++) {
         putpixel(x + i, y + l, img[j]);
      }
   }
}

void display_tga(struct tga_header *base)
{
   if (base->magic1 != 0 || base->colormap != 0 || base->encoding != 2
         || base->cmaporig != 0 || base->cmapent != 0 || base->x != 0
         || base->bpp != 32 || base->pixeltype != 40)
      return;
   u32 *img = (u32*)(sizeof(struct tga_header)+(u64)base);
   draw_icon(700, 50, base->w, base->h, img);
}

void init_fb(void *fb_addr, u16 fb_width, u16 fb_height, u16 fb_pitch)
{
   fb = (u32 *)fb_addr;
   width = fb_width;
   height = fb_height;
   pitch = fb_pitch;
   a = pitch/sizeof(u32);
}

void show_logo()
{
   struct tga_header *logo = (struct tga_header*)&logo_tga; 
   display_tga(logo);
}

void putpixel(int x, int y, u32 color)
{
   fb[y*(a)+x] = color;
}

