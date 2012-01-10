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

#ifndef XCSOAR_IO_TEXT_WRITER_HPP
#define XCSOAR_IO_TEXT_WRITER_HPP

#include "FileHandle.hpp"
#include "Compiler.h"
#include "Util/ReusableArray.hpp"

#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#ifdef _UNICODE
#include <tchar.h>
#endif

/**
 * Writer for an UTF-8 text file with native line endings.  All "const
 * char *" arguments must be either valid UTF-8 or 7 bit ASCII.
 *
 * The end-of-line marker is platform specific, and must not be passed
 * to this class.  Use the methods newline(), writeln() or printfln()
 * to finish a line.
 */
class TextWriter {
private:
  FileHandle file;

#ifdef _UNICODE
  ReusableArray<TCHAR> format_buffer;
  ReusableArray<char> convert_buffer;
#endif

public:
  /**
   * Creates a new text file.  Truncates the old file if it exists,
   * unless the parameter "append" is true.
   */
  TextWriter(const char *path, bool append=false);

#ifdef _UNICODE
  TextWriter(const TCHAR *path, bool append=false);
#endif

  /**
   * Returns true if opening the file has failed.  This must be
   * checked before calling any other method.
   */
  bool error() const {
    return !file.IsOpen();
  }

  /**
   * Ensure that all pending writes have been passed to the operating
   * system.  This does not guarantee that these have been written to
   * the physical device; they might still reside in the filesystem
   * cache.
   */
  bool flush() {
    assert(file.IsOpen());
    return file.Flush();
  }

  /**
   * Write one character.
   */
  void write(char ch) {
    assert(file.IsOpen());
    assert(ch != '\r');
    assert(ch != '\n');

    file.Write((int)ch);
  }

  /**
   * Finish the current line.
   */
  bool newline() {
    assert(file.IsOpen());

#ifndef HAVE_POSIX
    file.Write((int)'\r');
#endif
    file.Write((int)'\n');
    return true;
  }

  /**
   * Write a chunk of text to the file.
   */
  bool write(const char *s, size_t length) {
    assert(file.IsOpen());
    assert(memchr(s, '\r', length) == NULL);
    assert(memchr(s, '\n', length) == NULL);

    return file.Write(s, sizeof(*s), length) == length;
  }

  /**
   * Write a string to the file.
   */
  bool write(const char *s) {
    assert(file.IsOpen());
    assert(strchr(s, '\r') == NULL);
    assert(strchr(s, '\n') == NULL);

    return file.Write(s) >= 0;
  }

  /**
   * Write a string to the file, and finish the current line.
   */
  bool writeln(const char *s) {
    return write(s) && newline();
  }

#ifdef _UNICODE
  bool write(const TCHAR *s, size_t length);

  bool write(const TCHAR *s);

  bool writeln(const TCHAR *s) {
    return write(s) && newline();
  }
#endif

  template<typename... Args>
  void printf(const char *fmt, Args&&... args) {
    assert(file.IsOpen());
    assert(strchr(fmt, '\r') == NULL);
    assert(strchr(fmt, '\n') == NULL);

    file.WriteFormatted(fmt, args...);
  }

  template<typename... Args>
  void printfln(const char *fmt, Args&&... args) {
    assert(file.IsOpen());
    assert(strchr(fmt, '\r') == NULL);
    assert(strchr(fmt, '\n') == NULL);

    file.WriteFormatted(fmt, args...);
    newline();
  }

#ifdef _UNICODE
  template<typename... Args>
  void printf(const TCHAR *fmt, Args&&... args) {
    assert(file.IsOpen());
    assert(_tcschr(fmt, _T('\r')) == NULL);
    assert(_tcschr(fmt, _T('\n')) == NULL);

    file.WriteFormatted(fmt, args...);
  }

  template<typename... Args>
  void printfln(const TCHAR *fmt, Args&&... args) {
    assert(file.IsOpen());
    assert(_tcschr(fmt, _T('\r')) == NULL);
    assert(_tcschr(fmt, _T('\n')) == NULL);

    file.WriteFormatted(fmt, args...);
    newline();
  }
#endif
};

#endif
