#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <SDL3/SDL.h>
#include "t4k_common.h"
#include "globals.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    T4K_Tts_init();
    T4K_Tts_set_status(1); 

    printf("Starting TTS Test...\n");

    wchar_t word[] = L"cobra";
    wchar_t buffer[3000];
    
    wcscpy(buffer, word);
    int iter = wcslen(word);
    buffer[iter] = L'.'; iter++;
    buffer[iter] = L' '; iter++;
    
    if (1 < wcslen(word)) {
        for (int j = 0; j < wcslen(word); j++) {
            buffer[iter] = word[j]; iter++;
            buffer[iter] = L'.'; iter++;
            buffer[iter] = L' '; iter++;
        }
    }
    buffer[iter] = L'\0'; 

    printf("The game builds this string: '%ls'\n", buffer);
    printf("Calling T4K_Tts_say once...\n");
    
    T4K_Tts_say(50, 50, INTERRUPT, "%S", buffer);
    
    SDL_Delay(5000);
    
    printf("Test finished.\n");
    SDL_Quit();
    return 0;
}
