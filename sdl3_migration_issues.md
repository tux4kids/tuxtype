# TuxType SDL3 Migration Issues

## Issue 1
**Issue Title:** Update all SDL header includes to SDL3 namespace paths
SDL3 headers live under SDL3/, SDL3_image/, SDL3_mixer/, SDL3_ttf/ subdirectories. Every flat include of SDL.h, SDL_image.h, SDL_mixer.h, SDL_ttf.h must be updated to use the namespaced path.

**Affected Files:** `globals.h`, `setup.c`, `titlescreen.h`, `SDL_extras.h`, `SDL_extras.c`, `editor.c`, `loaders.c`

**Acceptance Criteria:** All SDL headers use `<SDL3/SDL.h>` style paths; project configures and compiles without file-not-found errors.
---

## Issue 2
**Issue Title:** Replace SDL_GetVideoInfo() / SDL_VideoInfo with SDL_GetDesktopDisplayMode()
SDL_VideoInfo and SDL_GetVideoInfo() are completely removed in SDL3. Native resolution and hardware info must be queried via SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay(), &mode).

**Affected Files:** `setup.c`

**Acceptance Criteria:** initialize_screen() compiles without SDL_VideoInfo references; fullscreen resolution detected correctly at runtime.
---

## Issue 3
**Issue Title:** Remove SDL_HWSURFACE / SDL_SWSURFACE / SDL_HWPALETTE surface-mode flags
SDL3 has no surface-based video mode. The surface_mode variable and all SDL_HWSURFACE, SDL_SWSURFACE, SDL_HWPALETTE flag logic must be deleted.

**Affected Files:** `setup.c`, `SDL_extras.c`

**Acceptance Criteria:** No hardware/software surface flag references remain; code compiles cleanly.
---

## Issue 4
**Issue Title:** Replace SDL_SetVideoMode() with SDL_CreateWindow() + SDL_CreateRenderer()
SDL_SetVideoMode() is removed in SDL3. Both call-sites (setup.c and SDL_extras.c for resolution switch) must be replaced with SDL_CreateWindow() and SDL_CreateRenderer().

**Affected Files:** `setup.c`, `SDL_extras.c`

**Acceptance Criteria:** A valid window and renderer are created at startup and on resolution switch; game renders correctly.
---

## Issue 5
**Issue Title:** Remove global SDL_Surface* screen — replace with window/renderer accessors
The global screen pointer is used as the render target everywhere. With the SDL3 renderer pipeline it must be removed. All code using screen for blitting/filling must use T4K_GetRenderer() or the SDL3 renderer API.

**Affected Files:** `globals.h`, `setup.c`, `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `practice.c`, `laser.c`, `alphabet.c`, `snow.c`, `pixels.c`

**Acceptance Criteria:** SDL_Surface* screen global removed; all files compile using renderer accessors.
---

## Issue 6
**Issue Title:** Replace SDL_WM_SetCaption() with SDL_SetWindowTitle()
SDL_WM_SetCaption(title, icon_title) removed in SDL3. Replace with SDL_SetWindowTitle(window, 'Tux Typing').

**Affected Files:** `setup.c`

**Acceptance Criteria:** Window title set correctly; no compilation errors.
---

## Issue 7
**Issue Title:** Replace SDL_SetColorKey()+SDL_WM_SetIcon() icon setup with SDL_SetWindowIcon()
SDL_WM_SetIcon(icon, NULL) removed. SDL3 uses SDL_SetWindowIcon(window, icon) which reads alpha from the surface directly. The preceding SDL_SetColorKey call for the icon is also unnecessary.

**Affected Files:** `setup.c`

**Acceptance Criteria:** seticon() compiles with SDL3; window icon is correctly displayed.
---

## Issue 8
**Issue Title:** Replace SDL_WM_GrabInput() with SDL_SetWindowMouseGrab()
SDL_WM_GrabInput(SDL_GRAB_OFF/ON) and SDL_GRAB_* constants removed in SDL3. Use SDL_SetWindowMouseGrab(window, SDL_FALSE/TRUE).

**Affected Files:** `titlescreen.c`

**Acceptance Criteria:** Both grab call-sites compile and grab/release mouse input correctly.
---

## Issue 9
**Issue Title:** Replace SDL_FULLSCREEN surface flag with SDL_SetWindowFullscreen()
Fullscreen is no longer a surface flag. Use SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) to enter fullscreen.

**Affected Files:** `setup.c`

**Acceptance Criteria:** Fullscreen mode uses SDL3 window API; no SDL_FULLSCREEN flag references remain.
---

## Issue 10
**Issue Title:** Replace screen->flags & SDL_FULLSCREEN checks with SDL_GetWindowFlags()
Reading fullscreen state from a surface flags field is invalid in SDL3. Use SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN.

**Affected Files:** `titlescreen.c`, `SDL_extras.c`

**Acceptance Criteria:** All fullscreen-state checks compile and return correct results.
---

## Issue 11
**Issue Title:** Replace SDL_Flip(screen) with SDL_RenderPresent(renderer)
SDL_Flip() removed in SDL3. Both occurrences in titlescreen.c must be replaced with SDL_RenderPresent(renderer).

**Affected Files:** `titlescreen.c`

**Acceptance Criteria:** No SDL_Flip references remain; frames present correctly.
---

## Issue 12
**Issue Title:** Remove SDL_UpdateRect() calls — superseded by SDL_RenderPresent()
SDL_UpdateRect(screen, x, y, w, h) removed in SDL3. The renderer redraws the full frame on SDL_RenderPresent(); all partial update calls must be removed.

**Affected Files:** `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `practice.c`, `snow.c`

**Acceptance Criteria:** No SDL_UpdateRect calls remain; display updates correctly.
---

## Issue 13
**Issue Title:** Remove SDL_UpdateRects() calls — superseded by SDL_RenderPresent()
SDL_UpdateRects() removed in SDL3. Calls in titlescreen.c and snow.c must be removed.

**Affected Files:** `titlescreen.c`, `snow.c`

**Acceptance Criteria:** No SDL_UpdateRects calls remain; snow and titlescreen animations render correctly.
---

## Issue 14
**Issue Title:** Migrate SDL_BlitSurface() screen-blits to the SDL3 renderer pipeline
SDL_BlitSurface(src, srcrect, screen, dstrect) writes to screen which no longer exists. On-screen blits must convert surfaces to textures via SDL_CreateTextureFromSurface() and render with SDL_RenderTexture().

**Affected Files:** `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `laser.c`, `alphabet.c`, `practice.c`

**Acceptance Criteria:** All on-screen blits use the texture/renderer path; game graphics render correctly.
---

## Issue 15
**Issue Title:** Migrate screen SDL_FillRect() calls to SDL_RenderFillRect() / SDL_RenderClear()
SDL_FillRect(screen, rect, color) invalid in SDL3 renderer pipeline. Use SDL_SetRenderDrawColor() + SDL_RenderFillRect() or SDL_RenderClear() for on-screen fills.

**Affected Files:** `titlescreen.c`, `snow.c`, `pixels.c`

**Acceptance Criteria:** All on-screen fill operations use the SDL3 renderer API.
---

## Issue 16
**Issue Title:** Update SDL_MapRGB() call signature for SDL3
SDL_MapRGB(surface->format, r, g, b) changed in SDL3. First arg is const SDL_PixelFormatDetails* from SDL_GetPixelFormatDetails(); second arg is const SDL_Palette* (NULL if none).

**Affected Files:** `titlescreen.c`, `snow.c`

**Acceptance Criteria:** All SDL_MapRGB call-sites compile; colours render correctly.
---

## Issue 17
**Issue Title:** Rewrite COL2RGB macro in titlescreen.h to remove screen->format dependency
The macro COL2RGB(col) uses SDL_MapRGB(screen->format, ...) which is invalid in SDL3. Rewrite to use SDL_GetPixelFormatDetails() or pass the format explicitly.

**Affected Files:** `titlescreen.h`

**Acceptance Criteria:** COL2RGB macro compiles and produces correct colour values.
---

## Issue 18
**Issue Title:** Replace SDL_FreeSurface() with SDL_DestroySurface() throughout
SDL_FreeSurface() renamed SDL_DestroySurface() in SDL3.

**Affected Files:** `editor.c`, `theme.c`, `titlescreen.c`, `SDL_extras.c`, `setup.c`, `loaders.c`

**Acceptance Criteria:** No SDL_FreeSurface references remain; no memory leaks introduced.
---

## Issue 19
**Issue Title:** Replace SDL_CreateRGBSurface(SDL_SWSURFACE,...) with SDL_CreateSurface() in SDL_extras.c
SDL_CreateRGBSurface with SDL_SWSURFACE flag is removed. Use SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888) or SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888).

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** All surface-creation calls compile with SDL3; text rendering and blending produce correct images.
---

## Issue 20
**Issue Title:** Remove direct SDL_SRCCOLORKEY / SDL_SRCALPHA flag manipulation on surface->flags in SDL_extras.c
SDL_extras.c directly bitwise-ANDs and ORs SDL_SRCCOLORKEY and SDL_SRCALPHA on surface->flags, which is invalid in SDL3. Use SDL_GetSurfaceColorKey(), SDL_SetSurfaceColorKey(), SDL_GetSurfaceAlphaMod(), SDL_SetSurfaceAlphaMod() instead.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** All surface flag manipulations use SDL3 getter/setter API; no direct surface->flags bitops for color key/alpha.
---

## Issue 21
**Issue Title:** Replace SDL_SetColorKey() SDL_SRCCOLORKEY|SDL_RLEACCEL with SDL_SetSurfaceColorKey()
SDL_SetColorKey(surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, key) changed to SDL_SetSurfaceColorKey(surface, SDL_TRUE, key). SDL_SRCCOLORKEY and SDL_RLEACCEL removed.

**Affected Files:** `SDL_extras.c`, `setup.c`

**Acceptance Criteria:** All color-key calls compile; sprite transparency works correctly.
---

## Issue 22
**Issue Title:** Replace SDL_DisplayFormat() with SDL_ConvertSurface() using explicit pixel format
SDL_DisplayFormat() removed in SDL3. Replace with SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGB888) or the appropriate screen-compatible format.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Surface conversion produces correct pixel format; no SDL_DisplayFormat references remain.
---

## Issue 23
**Issue Title:** Replace SDL_DisplayFormatAlpha() with SDL_ConvertSurface() using ARGB format
SDL_DisplayFormatAlpha() removed in SDL3. Replace with SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ARGB8888).

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Alpha-capable surface conversion compiles and produces correct results.
---

## Issue 24
**Issue Title:** Update SDL_ConvertSurface() third argument — remove SDL_SWSURFACE flag
SDL_ConvertSurface(S1, fmt1, SDL_SWSURFACE) third arg removed in SDL3. New signature is SDL_ConvertSurface(surface, SDL_PixelFormat).

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** SDL_ConvertSurface compiles with SDL3 signature; conversion result is correct.
---

## Issue 25
**Issue Title:** Verify TTF_Init() return-value check compatible with SDL3_ttf
TTF_Init() is retained in SDL3_ttf but confirm the return-value check (TTF_Init() < 0) is still valid, and update include path to <SDL3_ttf/SDL_ttf.h>.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** SDL3_ttf initialises correctly; include path updated.
---

## Issue 26
**Issue Title:** Verify TTF_OpenFont() API unchanged in SDL3_ttf
TTF_OpenFont(filename, size) expected unchanged. Confirm after include path update.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Fonts load correctly; no compile warnings.
---

## Issue 27
**Issue Title:** Verify TTF_RenderUTF8_Blended() API unchanged in SDL3_ttf
TTF_RenderUTF8_Blended(font, text, color) expected unchanged. Confirm return type is SDL_Surface*.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Text renders to surface correctly under SDL3_ttf.
---

## Issue 28
**Issue Title:** Verify TTF_CloseFont() and TTF_Quit() unchanged in SDL3_ttf
Both functions expected unchanged. Confirm after include path update.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Font resources freed and library quits without errors.
---

## Issue 29
**Issue Title:** Update Mix_OpenAudio() call signature for SDL3_mixer 3.x
Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048) changed in SDL3_mixer to Mix_OpenAudio(0, NULL).

**Affected Files:** `setup.c`

**Acceptance Criteria:** Audio initialises without errors; sound plays correctly.
---

## Issue 30
**Issue Title:** Rename event type SDL_KEYDOWN / SDL_KEYUP to SDL_EVENT_KEY_DOWN / SDL_EVENT_KEY_UP
All SDL3 event type constants use the SDL_EVENT_ prefix.

**Affected Files:** `titlescreen.c`, `playgame.c`, `practice.c`, `scripting.c`, `laser.c`

**Acceptance Criteria:** All keyboard event type checks use SDL3 names; input functions correctly.
---

## Issue 31
**Issue Title:** Rename event type SDL_QUIT to SDL_EVENT_QUIT
Quit event constant renamed SDL_EVENT_QUIT in SDL3.

**Affected Files:** `editor.c`, `titlescreen.c`, `theme.c`, `playgame.c`

**Acceptance Criteria:** Quit-event checks use SDL_EVENT_QUIT; window-close exits cleanly.
---

## Issue 32
**Issue Title:** Rename event type SDL_MOUSEBUTTONDOWN to SDL_EVENT_MOUSE_BUTTON_DOWN
Mouse button event type constant renamed in SDL3.

**Affected Files:** `editor.c`, `titlescreen.c`, `theme.c`, `playgame.c`, `practice.c`

**Acceptance Criteria:** Mouse-button event checks use new SDL3 name; clicks register correctly.
---

## Issue 33
**Issue Title:** Rename event type SDL_MOUSEMOTION to SDL_EVENT_MOUSE_MOTION
Mouse motion event type constant renamed in SDL3.

**Affected Files:** `playgame.c`, `practice.c`, `scripting.c`

**Acceptance Criteria:** Mouse motion events correctly identified and processed.
---

## Issue 34
**Issue Title:** Update keyboard event field from event.key.keysym.sym to event.key.key
SDL_Keysym struct removed from SDL3. Access the key symbol as event.key.key (SDL_Keycode).

**Affected Files:** `playgame.c`, `practice.c`

**Acceptance Criteria:** All keysym field accesses compile; key comparisons work correctly.
---

## Issue 35
**Issue Title:** Replace event.key.keysym.unicode text input with SDL_EVENT_TEXT_INPUT
keysym.unicode removed in SDL3. Unicode text input is handled via the SDL_EVENT_TEXT_INPUT event and event.text.text field. Enable text input with SDL_StartTextInput().

**Affected Files:** `playgame.c`

**Acceptance Criteria:** All typed characters are received correctly via TEXT_INPUT events; unicode input works.
---

## Issue 36
**Issue Title:** Replace SDL_ShowCursor(SDL_ENABLE/DISABLE/1) with SDL_ShowCursor() / SDL_HideCursor()
SDL_ShowCursor(int toggle) split into separate SDL_ShowCursor() and SDL_HideCursor() in SDL3. SDL_ENABLE/SDL_DISABLE constants also removed.

**Affected Files:** `titlescreen.c`, `scripting.c`

**Acceptance Criteria:** Cursor shown and hidden correctly at the appropriate points.
---

## Issue 37
**Issue Title:** Update SDL_GetTicks() return type from Uint32 to Uint64 throughout
SDL_GetTicks() returns Uint64 in SDL3. All receiving Uint32 variables must become Uint64 to prevent truncation.

**Affected Files:** `titlescreen.c`, `playgame.c`, `practice.c`

**Acceptance Criteria:** All SDL_GetTicks() results stored in Uint64; timing logic correct.
---

## Issue 38
**Issue Title:** Update Uint32 this_tick variable in playgame.c to Uint64
Uint32 this_tick = SDL_GetTicks() must become Uint64 to match SDL3 return type.

**Affected Files:** `playgame.c`

**Acceptance Criteria:** this_tick is Uint64; frame timing calculations remain correct.
---

## Issue 39
**Issue Title:** Update SDL_GetMouseState() to handle float coordinates in SDL3
SDL_GetMouseState() now returns float coordinates. Call-site casting to int* is invalid. Use float fx, fy locals.

**Affected Files:** `titlescreen.c`

**Acceptance Criteria:** Mouse coordinates read correctly; Easter Egg cursor position accurate.
---

## Issue 40
**Issue Title:** Guard direct pixel access in pixels.c with SDL_LockSurface() / SDL_UnlockSurface()
SDL3 requires SDL_LockSurface() before accessing surface->pixels, surface->pitch, or surface->format->BytesPerPixel.

**Affected Files:** `pixels.c`

**Acceptance Criteria:** All pixel operations lock the surface before access; no crashes or undefined behaviour.
---

## Issue 41
**Issue Title:** Update find_package(SDL) to find_package(SDL3) in CMakeLists.txt
CMake package name changed for SDL3.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** CMake finds SDL3 headers and libraries; configuration step succeeds.
---

## Issue 42
**Issue Title:** Update find_package(SDL_image/mixer/ttf) to SDL3 equivalents in CMakeLists.txt
CMake package names changed: SDL_image -> SDL3_image, SDL_mixer -> SDL3_mixer, SDL_ttf -> SDL3_ttf.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** All SDL3 satellite libraries found; project compiles against them.
---

## Issue 43
**Issue Title:** Replace raw CMake library variables with imported SDL3 targets in target_link_libraries
Replace ${SDL_LIBRARY}, ${SDLIMAGE_LIBRARY}, ${SDLMIXER_LIBRARY}, ${SDLTTF_LIBRARY} with SDL3::SDL3, SDL3_image::SDL3_image, SDL3_mixer::SDL3_mixer, SDL3_ttf::SDL3_ttf.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** Project links correctly against SDL3 using modern CMake imported target syntax.
---

## Issue 44
**Issue Title:** Audit and update CMake include_directories for SDL3 header paths
SDL3 CMake modules expose include dirs via imported targets; explicit include_directories() calls for SDL headers should be removed or updated.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** No hardcoded SDL include paths remain; headers resolved via imported targets.
---

## Issue 45
**Issue Title:** Verify SDL3 compatibility of SDL_extras.c surface blending / rotation helper functions
SDL_extras.c contains multiple helper functions that blit, rotate, and blend surfaces using SDL1.2 patterns (SDL_DisplayFormat, SDL_SRCALPHA flag ops, SDL_SWSURFACE). Each function must be audited and ported to SDL3 surface API.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** All SDL_extras helpers compile and produce visually correct output under SDL3; no deprecated API calls remain.
---

*Total issues: **45** (1:1 ratio with function mapping table)*
