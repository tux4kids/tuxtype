/*
   audio.c:

   Code for audio-related functions

   Copyright 2000, 2003, 2010.
   Authors: Jesse Andrews, David Bruce.
   
   Project email: <tux4kids-tuxtype-dev@lists.alioth.debian.org>
   Project website: http://tux4kids.alioth.debian.org

   audio.c is part of Tux Typing, a.k.a "tuxtype".

Tux Typing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Tux Typing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




#include "globals.h"
#include "funcs.h"

extern Mix_Chunk* sounds[];

static Mix_Music* defaultMusic = NULL; // holds music for audioMusicLoad/unload

/* Fixed-channel playback (see funcs.h): a small channel-number ->
   MIX_Track* table, since SDL3_mixer's tracks aren't numbered. */
#define NUM_FIXED_CHANNELS 16
static MIX_Track* channel_tracks[NUM_FIXED_CHANNELS] = { NULL };

static MIX_Track* GetChannelTrack(int channel)
{
  if (channel < 0 || channel >= NUM_FIXED_CHANNELS)
    return NULL;
  if (!channel_tracks[channel])
    channel_tracks[channel] = MIX_CreateTrack(T4K_GetMixer());
  return channel_tracks[channel];
}

void PlayChannel(int channel, Mix_Chunk* chunk, int loops)
{
  MIX_Track* track;
  SDL_PropertiesID props;

  if (!chunk || !settings.sys_sound)
    return;

  track = GetChannelTrack(channel);
  if (!track)
    return;

  MIX_SetTrackAudio(track, chunk);
  props = SDL_CreateProperties();
  SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
  MIX_PlayTrack(track, props);
  SDL_DestroyProperties(props);
}

bool ChannelPlaying(int channel)
{
  MIX_Track* track = GetChannelTrack(channel);
  return track && MIX_TrackPlaying(track);
}



// play sound once
void PlaySound(Mix_Chunk* snd)
{
  PlaySoundLoop(snd, 0);
}

void playsound(int snd)
{
#ifndef NOSOUND
	T4K_PlaySound(sounds[snd]);
#endif
}


// play sound with optional repeats, or -1 for infinite
void PlaySoundLoop(Mix_Chunk* snd, int loops)
{
  if(!snd)
    return;
  if (!settings.sys_sound)
    return;

  T4K_PlaySoundLoop(snd, loops);
}

// halt a channel or -1 for all
void audioHaltChannel(int channel)
{
    if (channel < 0)
    {
        int i;
        for (i = 0; i < NUM_FIXED_CHANNELS; i++)
            if (channel_tracks[i])
                MIX_StopTrack(channel_tracks[i], 0);
        T4K_AudioHaltChannel(channel);
        return;
    }

    {
        MIX_Track* track = GetChannelTrack(channel);
        if (track)
            MIX_StopTrack(track, 0);
    }
}

/* MusicLoad attempts to load and play the music file
 * Note: loops == -1 means forever
 */
void MusicLoad(const char* musicFilename, int loops)
{
  Mix_Music* tmp_music = NULL;

  if (!settings.sys_sound) return;
  if (!musicFilename) return;

  tmp_music = LoadMusic(musicFilename);

  if (tmp_music)
  {
    MusicUnload(); //Unload previous defaultMusic
    defaultMusic = tmp_music;
    T4K_AudioMusicPlay(defaultMusic, loops);
  }
}


/* MusicUnload attempts to unload any music data that was
 * loaded using the audioMusicLoad function
 */
void MusicUnload(void)
{
  if (!settings.sys_sound) return;

  if (defaultMusic)
  {
    MIX_DestroyAudio(defaultMusic);
    defaultMusic = NULL;
  }
}


/* audioMusicPlay attempts to play the passed music data.
 * if a music file was loaded using the audioMusicLoad
 * it will be stopped and unloaded
 * Note: loops == -1 means forever
 */
void MusicPlay(Mix_Music* musicData, int loops)
{
  if (!settings.sys_sound) return;
  if (!musicData) return;

  /* Stop previous music before playing new one: */
  MusicUnload();
  T4K_AudioMusicPlay(musicData, loops);
}
