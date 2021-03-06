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

#include "VarioGlue.hpp"
#include "PCMPlayer.hpp"
#include "VarioSynthesiser.hpp"
#include "ComputerSettings.hpp"

#ifdef ANDROID
#include "SLES/Init.hpp"
#endif

#include <assert.h>

static const unsigned sample_rate = 44100;

#ifdef ANDROID
static bool have_sles;
#endif

static PCMPlayer *player;
static VarioSynthesiser *synthesiser;

void
AudioVarioGlue::Initialise()
{
  assert(player == NULL);
  assert(synthesiser == NULL);

#ifdef ANDROID
  have_sles = SLES::Initialise();
  if (!have_sles)
    return;
#endif

  player = new PCMPlayer();
  synthesiser = new VarioSynthesiser();
}

#if defined(__clang__) || GCC_VERSION >= 40700
/* no, VarioSynthesiser really doesn't need a virtual destructor */
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif

void
AudioVarioGlue::Deinitialise()
{
  delete player;
  delete synthesiser;
}

void
AudioVarioGlue::Configure(const SoundSettings &settings)
{
#ifdef ANDROID
  if (!have_sles)
    return;
#endif

  assert(player != NULL);
  assert(synthesiser != NULL);

  if (settings.sound_vario_enabled) {
    synthesiser->SetVolume(settings.sound_volume);
    player->Start(*synthesiser, sample_rate);
  } else
    player->Stop();
}

void
AudioVarioGlue::SetValue(fixed vario)
{
#ifdef ANDROID
  if (!have_sles)
    return;
#endif

  assert(player != NULL);
  assert(synthesiser != NULL);

  synthesiser->SetVario(sample_rate, vario);
}

void
AudioVarioGlue::NoValue()
{
#ifdef ANDROID
  if (!have_sles)
    return;
#endif

  assert(player != NULL);
  assert(synthesiser != NULL);

  synthesiser->SetSilence();
}
