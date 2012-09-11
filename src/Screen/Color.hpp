/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#ifndef XCSOAR_SCREEN_COLOR_HPP
#define XCSOAR_SCREEN_COLOR_HPP

#ifdef ENABLE_OPENGL
#include "Screen/OpenGL/Color.hpp"
#elif defined(ENABLE_SDL)
#include "Screen/SDL/Color.hpp"
#else
#include "Screen/GDI/Color.hpp"
#endif

class ColorTheme;

extern ColorTheme *color_theme;

static inline constexpr uint8_t
LightColor(uint8_t c)
{
  return ((c ^ 0xff) >> 1) ^ 0xff;
}

/**
 * Returns a lighter version of the specified color, adequate for
 * SRCAND filtering.
 */
static inline constexpr Color
LightColor(Color c)
{
  return Color(LightColor(c.Red()), LightColor(c.Green()),
               LightColor(c.Blue()));
}

static inline constexpr uint8_t
DarkColor(uint8_t c)
{
  return (c >> 1);
}

/**
 * Returns a darker version of the specified color.
 */
static inline constexpr Color
DarkColor(Color c)
{
  return Color(DarkColor(c.Red()), DarkColor(c.Green()),
               DarkColor(c.Blue()));
}

Color Desaturate(Color c);

#define COLOR_GRAY_NOOK Color(238, 238, 238)
#define COLOR_LIGHT_GRAY_NOOK Color(239, 239, 239)
#define COLOR_DARK_GRAY_NOOK Color(237, 237, 237)
#define COLOR_RED_NOOK Color(238, 238, 238)
#define COLOR_GREEN_NOOK Color(237, 237, 237)
#define COLOR_BLUE_NOOK Color(236, 236, 236)
#define COLOR_YELLOW_NOOK Color(238, 238, 238)
#define COLOR_CYAN_NOOK Color(238, 238, 238)
#define COLOR_MAGENTA_NOOK Color(237, 237, 237)
#define COLOR_ORANGE_NOOK Color(236, 236, 236)
#define COLOR_BROWN_NOOK Color(236, 236, 236)

//#define COLOR_GRAY Color(238, 238, 238)
//#define COLOR_LIGHT_GRAY Color(239, 239, 239)
//#define COLOR_DARK_GRAY Color(237, 237, 237)
#define COLOR_GRAY Color(0x80, 0x80, 0x80)
#define COLOR_LIGHT_GRAY Color(0xc0, 0xc0, 0xc0)
#define COLOR_DARK_GRAY Color(0x4, 0x40, 0x40)
#define COLOR_RED Color(0xff, 0x00, 0x00)
#define COLOR_BLUE Color(0x00, 0x00, 0xff)
#define COLOR_ORANGE Color(0xff, 0xa2, 0x00)
#define COLOR_GREEN Color(0x00, 0xff, 0x00)
#define COLOR_YELLOW Color(0xff, 0xff, 0x00)
#define COLOR_CYAN Color(0x00, 0xff, 0xff)
#define COLOR_MAGENTA Color(0xff, 0x00, 0xff)
#define COLOR_BROWN Color(0xb7, 0x64, 0x1e)

#define COLOR_XCSOAR_LIGHT_TESTING Color(0xed, 0x90, 0x90)
#define COLOR_XCSOAR_TESTING Color(0xd0, 0x17, 0x17)
#define COLOR_XCSOAR_DARK_TESTING Color(0x5d, 0x0a, 0x0a)
#define COLOR_XCSOAR_LIGHT_NOOK Color(238, 238, 238)
#define COLOR_XCSOAR_NOOK Color(237, 237, 237)
#define COLOR_XCSOAR_DARK_NOOK Color(236, 236, 236)
#define COLOR_XCSOAR_LIGHT Color(0xaa, 0xc9, 0xe4)
#define COLOR_XCSOAR Color(0x3f, 0x76, 0xa8)
#define COLOR_XCSOAR_DARK Color(0x00, 0x31, 0x5e)

#define COLOR_BLACK Color(0x00, 0x00, 0x00)
#define COLOR_WHITE Color(0xFF, 0xFF, 0xFF)

class ColorTheme {
public:
  const Color xcsoar_light;
  const Color xcsoar;
  const Color xcsoar_dark;
  const Color gray;
  const Color light_gray;
  const Color dark_gray;
  const Color red;
  const Color green;
  const Color blue;
  const Color yellow;
  const Color cyan;
  const Color magenta;
  const Color orange;
  const Color brown;

  ColorTheme(bool has_color)
#ifdef TESTING
    :xcsoar_light(COLOR_XCSOAR_LIGHT_TESTING),
    xcsoar(COLOR_XCSOAR_TESTING),
    xcsoar_dark(COLOR_XCSOAR_DARK_TESTING),
#else
    :xcsoar_light(has_color ? COLOR_XCSOAR_LIGHT : COLOR_XCSOAR_LIGHT_NOOK),
    xcsoar(has_color ? COLOR_XCSOAR : COLOR_XCSOAR_NOOK),
    xcsoar_dark(has_color ? COLOR_XCSOAR_DARK : COLOR_XCSOAR_DARK_NOOK),
#endif
    gray(has_color ? COLOR_GRAY : COLOR_GRAY_NOOK),
    light_gray(has_color ? COLOR_LIGHT_GRAY : COLOR_LIGHT_GRAY_NOOK),
    dark_gray(has_color ? COLOR_DARK_GRAY : COLOR_DARK_GRAY_NOOK),
    red(has_color ? COLOR_RED : COLOR_RED_NOOK),
    green(has_color ? COLOR_GREEN : COLOR_GREEN_NOOK),
    blue(has_color ? COLOR_BLUE : COLOR_BLUE_NOOK),
    yellow(has_color ? COLOR_YELLOW : COLOR_YELLOW_NOOK),
    cyan(has_color ? COLOR_CYAN : COLOR_CYAN_NOOK),
    magenta(has_color ? COLOR_MAGENTA : COLOR_MAGENTA_NOOK),
    orange(has_color ? COLOR_ORANGE : COLOR_ORANGE_NOOK),
    brown(has_color ? COLOR_BROWN : COLOR_BROWN_NOOK) {
  }
};

#endif
