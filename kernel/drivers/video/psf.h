
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

#ifndef PSF_H
#define PSF_H

#include <kernel/types.h>

#define PSF_FONT_MAGIC 0x864ab572

struct psf_font {
  u32 magic;         /* magic bytes to identify PSF */
  u32 version;       /* zero */
  u32 headersize;    /* offset of bitmaps in file, 32 */
  u32 flags;         /* 0 if there's no unicode table */
  u32 numglyph;      /* number of glyphs */
  u32 bytesperglyph; /* size of each glyph */
  u32 height;        /* height in pixels */
  u32 width;         /* width in pixels */
}__attribute__((packed));

#endif
