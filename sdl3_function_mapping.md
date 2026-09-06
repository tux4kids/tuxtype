# TuxType SDL3 Migration — Function Mapping Table

> Scope: all `.c` / `.h` files under `tuxtype/src/`.  
> t4kcommon is already migrated to SDL3.

| # | File(s) | Old SDL1.2/SDL2 Function/Concept | New SDL3 Function/Concept | Implementation Notes |
|---|---------|----------------------------------|---------------------------|----------------------|
| 1 | `globals.h`, `setup.c`, `titlescreen.h`, `SDL_extras.h` | `#include "SDL.h"` / `#include "SDL_image.h"` / `#include "SDL_mixer.h"` / `#include "SDL_ttf.h"` | `#include <SDL3/SDL.h>` / `#include <SDL3_image/SDL_image.h>` / `#include <SDL3_mixer/SDL_mixer.h>` / `#include <SDL3_ttf/SDL_ttf.h>` | All flat SDL header paths must use the SDL3 namespaced subdirectory. Apply globally across all files. |
| 2 | `setup.c` | `SDL_GetVideoInfo()` / `const SDL_VideoInfo* video_info` | `SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay(), &mode)` | `SDL_VideoInfo` and `SDL_GetVideoInfo()` removed entirely in SDL3. Use `SDL_DisplayMode` struct. |
| 3 | `setup.c` | `video_info->hw_available` → `SDL_HWSURFACE` / else `SDL_SWSURFACE` | Removed — no surface hardware flags in SDL3 | `SDL_HWSURFACE`, `SDL_SWSURFACE`, `SDL_HWPALETTE` flags all gone. Delete `surface_mode` variable. |
| 4 | `setup.c`, `SDL_extras.c` | `SDL_SetVideoMode(w, h, BPP, SDL_FULLSCREEN \| surface_mode)` | `SDL_CreateWindow()` + `SDL_CreateRenderer()` | Both sites that call `SDL_SetVideoMode` (in `setup.c` and `SDL_extras.c`) must be replaced with the SDL3 window/renderer creation API. |
| 5 | `globals.h`, `setup.c`, many others | `SDL_Surface* screen` global render target | `SDL_Window*` + `SDL_Renderer*` via t4kcommon accessors `T4K_GetWindow()` / `T4K_GetRenderer()` | Remove the global `screen` pointer; all rendering goes through the renderer pipeline. |
| 6 | `setup.c` | `SDL_WM_SetCaption("Tux Typing", "TuxType")` | `SDL_SetWindowTitle(window, "Tux Typing")` | Icon title (second arg) removed in SDL3. |
| 7 | `setup.c` | `SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey)` then `SDL_WM_SetIcon(icon, NULL)` | `SDL_SetWindowIcon(window, icon)` (no color key needed) | `SDL_WM_SetIcon` removed. SDL3 reads alpha from the surface directly. |
| 8 | `titlescreen.c` | `SDL_WM_GrabInput(SDL_GRAB_OFF/ON)` | `SDL_SetWindowMouseGrab(window, SDL_FALSE/TRUE)` | `SDL_WM_GrabInput` and `SDL_GRAB_*` constants removed in SDL3. |
| 9 | `setup.c` | `SDL_FULLSCREEN` flag in `SDL_SetVideoMode` | `SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN)` | Fullscreen is a window property, not a surface flag. |
| 10 | `titlescreen.c`, `SDL_extras.c` | `screen->flags & SDL_FULLSCREEN` / `T4K_GetScreen()->flags & SDL_FULLSCREEN` | `SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN` | Surface flag no longer available; use window flag query. |
| 11 | `titlescreen.c` | `SDL_Flip(screen)` (2 occurrences) | `SDL_RenderPresent(renderer)` | `SDL_Flip` removed in SDL3. |
| 12 | `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `practice.c`, `snow.c` | `SDL_UpdateRect(screen, x, y, w, h)` | `SDL_RenderPresent(renderer)` (full-frame) | Partial update calls removed; renderer redraws entire frame. |
| 13 | `titlescreen.c`, `snow.c` | `SDL_UpdateRects(screen, n, rects)` | `SDL_RenderPresent(renderer)` | Removed entirely in SDL3. |
| 14 | `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `laser.c`, `alphabet.c`, `practice.c` | `SDL_BlitSurface(src, srcrect, screen, dstrect)` | `SDL_CreateTextureFromSurface()` + `SDL_RenderTexture()` for on-screen draws | Off-screen surface-to-surface blits remain valid; final display requires texture pipeline. |
| 15 | `titlescreen.c`, `snow.c`, `pixels.c` | `SDL_FillRect(surface, rect, color)` on screen | `SDL_SetRenderDrawColor()` + `SDL_RenderFillRect()` or `SDL_RenderClear()` | On-screen fills use renderer API; off-screen surface fills remain valid. |
| 16 | `titlescreen.c`, `snow.c` | `SDL_MapRGB(screen->format, r, g, b)` / `COL2RGB(col)` macro | `SDL_MapRGB(SDL_GetPixelFormatDetails(format), NULL, r, g, b)` | `SDL_MapRGB` signature changed; first arg is `SDL_PixelFormatDetails*` from `SDL_GetPixelFormatDetails()`. |
| 17 | `titlescreen.h` | `#define COL2RGB(col) SDL_MapRGB(screen->format, col->r, col->g, col->b)` | Rewrite macro: `SDL_MapRGB(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_ARGB8888), NULL, col->r, col->g, col->b)` | The macro depends on `screen->format` which no longer exists; must be reformulated. |
| 18 | `editor.c`, `theme.c`, `titlescreen.c`, `SDL_extras.c`, `setup.c`, `loaders.c` | `SDL_FreeSurface(surface)` | `SDL_DestroySurface(surface)` | Renamed in SDL3. |
| 19 | `SDL_extras.c` | `SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, bpp, Rm, Gm, Bm, Am)` (multiple calls) | `SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888)` | `SDL_SWSURFACE` flag and full mask signature removed in SDL3. |
| 20 | `SDL_extras.c` | `SDL_SRCCOLORKEY` / `SDL_SRCALPHA` read/write on `surface->flags` | These flags removed from `SDL_Surface.flags` in SDL3; use `SDL_GetSurfaceColorKey()` / `SDL_GetSurfaceAlphaMod()` | `SDL_extras.c` bitwise-ANDs and ORs these flags directly on `surface->flags` — invalid in SDL3. |
| 21 | `SDL_extras.c` | `SDL_SetColorKey(bg, SDL_SRCCOLORKEY \| SDL_RLEACCEL, color_key)` | `SDL_SetSurfaceColorKey(bg, SDL_TRUE, color_key)` | `SDL_SRCCOLORKEY` and `SDL_RLEACCEL` removed; use boolean API. |
| 22 | `SDL_extras.c`, `setup.c` | `SDL_SetColorKey(icon, SDL_SRCCOLORKEY, key)` | `SDL_SetSurfaceColorKey(icon, SDL_TRUE, key)` | Same as above for icon setup. |
| 23 | `SDL_extras.c` | `SDL_DisplayFormat(surface)` | `SDL_ConvertSurface(surface, SDL_GetWindowSurface_format())` or keep as-is with `SDL_PIXELFORMAT_ARGB8888` | `SDL_DisplayFormat` removed; convert explicitly to a target pixel format. |
| 24 | `SDL_extras.c` | `SDL_DisplayFormatAlpha(surface)` | `SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ARGB8888)` | `SDL_DisplayFormatAlpha` removed; use `SDL_ConvertSurface` with an alpha-capable format. |
| 25 | `SDL_extras.c` | `SDL_ConvertSurface(S1, fmt1, SDL_SWSURFACE)` | `SDL_ConvertSurface(S1, fmt1->format)` (SDL3 takes `SDL_PixelFormat` enum not `SDL_PixelFormatDetails*`) | Third arg changed; `SDL_SWSURFACE` flag removed. Verify new signature from SDL3 headers. |
| 26 | `SDL_extras.c` | `TTF_Init()` | `TTF_Init()` — retained; verify return value check (`TTF_Init() < 0` still valid) | SDL3_ttf retains `TTF_Init()`; confirm include path update to `<SDL3_ttf/SDL_ttf.h>`. |
| 27 | `SDL_extras.c` | `TTF_OpenFont(fn, size)` | `TTF_OpenFont(fn, size)` — retained | SDL3_ttf retains `TTF_OpenFont`; confirm header path. |
| 28 | `SDL_extras.c` | `TTF_RenderUTF8_Blended(font, text, color)` | `TTF_RenderUTF8_Blended(font, text, color)` — retained | Verify return type is still `SDL_Surface*` and that surface creation flags are SDL3-compatible. |
| 29 | `SDL_extras.c` | `TTF_CloseFont(font)` | `TTF_CloseFont(font)` — retained | Confirm header path update only. |
| 30 | `SDL_extras.c` | `TTF_Quit()` | `TTF_Quit()` — retained | Confirm header path update only. |
| 31 | `setup.c` | `Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048)` | `Mix_OpenAudio(0, NULL)` | SDL3_mixer 3.x changed signature: device ID (0=default) and `SDL_AudioSpec*` (NULL=defaults). |
| 32 | `titlescreen.c`, `playgame.c`, `practice.c`, `scripting.c`, `laser.c` | `SDL_KEYDOWN` / `SDL_KEYUP` event type | `SDL_EVENT_KEY_DOWN` / `SDL_EVENT_KEY_UP` | All SDL3 event type constants use `SDL_EVENT_` prefix. |
| 33 | `editor.c`, `titlescreen.c`, `theme.c`, `playgame.c` | `SDL_QUIT` event type | `SDL_EVENT_QUIT` | Renamed in SDL3. |
| 34 | `editor.c`, `titlescreen.c`, `theme.c`, `playgame.c`, `practice.c` | `SDL_MOUSEBUTTONDOWN` event type | `SDL_EVENT_MOUSE_BUTTON_DOWN` | Renamed in SDL3. |
| 35 | `playgame.c`, `practice.c`, `scripting.c` | `SDL_MOUSEMOTION` event type | `SDL_EVENT_MOUSE_MOTION` | Renamed in SDL3. |
| 36 | `playgame.c`, `practice.c` | `event.key.keysym.sym == SDLK_xxx` | `event.key.key == SDLK_xxx` | `SDL_Keysym` struct removed; key accessed as `event.key.key`. |
| 37 | `playgame.c` | `event.key.keysym.unicode` for text input | `SDL_EVENT_TEXT_INPUT` + `event.text.text` | `keysym.unicode` removed in SDL3; text input handled via dedicated `SDL_EVENT_TEXT_INPUT` event. |
| 38 | `titlescreen.c` | `SDL_ShowCursor(SDL_ENABLE)` / `SDL_ShowCursor(SDL_DISABLE)` / `SDL_ShowCursor(1)` | `SDL_ShowCursor()` / `SDL_HideCursor()` | `SDL_ShowCursor(int)` split into two functions in SDL3. |
| 39 | `scripting.c` | `SDL_ShowCursor(1)` (3 occurrences) | `SDL_ShowCursor()` | Same as above. |
| 40 | `titlescreen.c`, `playgame.c`, `practice.c` | `SDL_GetTicks()` returns `Uint32` | `SDL_GetTicks()` returns `Uint64` | Update all receiving variables to `Uint64`. |
| 41 | `playgame.c` | `Uint32 this_tick = SDL_GetTicks()` | `Uint64 this_tick = SDL_GetTicks()` | Same as above. |
| 42 | `titlescreen.c` | `SDL_GetMouseState((int*)(&cursor.x), (int*)(&cursor.y))` | `SDL_GetMouseState(&fx, &fy)` with `float fx, fy` | `SDL_GetMouseState` returns `float` coords in SDL3. |
| 43 | `pixels.c` | Direct `surface->pixels` / `surface->pitch` / `surface->format->BytesPerPixel` access | Must be guarded with `SDL_LockSurface()` / `SDL_UnlockSurface()` | SDL3 requires lock/unlock around direct pixel access. |
| 44 | `setup.c` | CMake `find_package(SDL)`, `find_package(SDL_image)`, `find_package(SDL_mixer)`, `find_package(SDL_ttf)` | `find_package(SDL3)`, `find_package(SDL3_image)`, `find_package(SDL3_mixer)`, `find_package(SDL3_ttf)` | All CMake find-package calls and link targets must be updated. |
| 45 | `CMakeLists.txt` | `${SDL_LIBRARY}`, `${SDLIMAGE_LIBRARY}`, `${SDLMIXER_LIBRARY}`, `${SDLTTF_LIBRARY}` | `SDL3::SDL3`, `SDL3_image::SDL3_image`, `SDL3_mixer::SDL3_mixer`, `SDL3_ttf::SDL3_ttf` | Use modern imported CMake targets. |

*Total distinct changes: **45***
