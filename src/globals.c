#include "globals.h"
//TTS Thread
SDL_Thread *tts_thread;

//TTS Word announcer Thread
SDL_Thread *tts_announcer_thread;


int  text_to_speech_status=0;

struct braille_dict braille_key_value_map[100];
