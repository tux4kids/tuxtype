#!/usr/bin/env bash
# Auto-generated GitHub issue creation script for TuxType SDL3 migration
# Usage: chmod +x create_github_issues.sh && ./create_github_issues.sh
# Requires: gh CLI authenticated (gh auth login)

REPO="Midhun-M-git/tuxtype"
LABEL="sdl3-migration"

gh label create "$LABEL" --repo "$REPO" --color "0075ca" --description "SDL3 migration" 2>/dev/null || true

echo 'Issue 1/45: Update all SDL header includes to SDL3 namespace paths...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update all SDL header includes to SDL3 namespace paths' \
  --body 'SDL3 headers live under SDL3/, SDL3_image/, SDL3_mixer/, SDL3_ttf/ subdirectories. Every flat include of SDL.h, SDL_image.h, SDL_mixer.h, SDL_ttf.h must be updated to use the namespaced path.

**Affected Files:** `globals.h`, `setup.c`, `titlescreen.h`, `SDL_extras.h`, `SDL_extras.c`, `editor.c`, `loaders.c`

**Acceptance Criteria:** All SDL headers use `<SDL3/SDL.h>` style paths; project configures and compiles without file-not-found errors.'

echo 'Issue 2/45: Replace SDL_GetVideoInfo() / SDL_VideoInfo with SDL_Get...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_GetVideoInfo() / SDL_VideoInfo with SDL_GetDesktopDisplayMode()' \
  --body 'SDL_VideoInfo and SDL_GetVideoInfo() are completely removed in SDL3. Native resolution and hardware info must be queried via SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay(), &mode).

**Affected Files:** `setup.c`

**Acceptance Criteria:** initialize_screen() compiles without SDL_VideoInfo references; fullscreen resolution detected correctly at runtime.'

echo 'Issue 3/45: Remove SDL_HWSURFACE / SDL_SWSURFACE / SDL_HWPALETTE su...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Remove SDL_HWSURFACE / SDL_SWSURFACE / SDL_HWPALETTE surface-mode flags' \
  --body 'SDL3 has no surface-based video mode. The surface_mode variable and all SDL_HWSURFACE, SDL_SWSURFACE, SDL_HWPALETTE flag logic must be deleted.

**Affected Files:** `setup.c`, `SDL_extras.c`

**Acceptance Criteria:** No hardware/software surface flag references remain; code compiles cleanly.'

echo 'Issue 4/45: Replace SDL_SetVideoMode() with SDL_CreateWindow() + SD...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_SetVideoMode() with SDL_CreateWindow() + SDL_CreateRenderer()' \
  --body 'SDL_SetVideoMode() is removed in SDL3. Both call-sites (setup.c and SDL_extras.c for resolution switch) must be replaced with SDL_CreateWindow() and SDL_CreateRenderer().

**Affected Files:** `setup.c`, `SDL_extras.c`

**Acceptance Criteria:** A valid window and renderer are created at startup and on resolution switch; game renders correctly.'

echo 'Issue 5/45: Remove global SDL_Surface* screen — replace with window...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Remove global SDL_Surface* screen — replace with window/renderer accessors' \
  --body 'The global screen pointer is used as the render target everywhere. With the SDL3 renderer pipeline it must be removed. All code using screen for blitting/filling must use T4K_GetRenderer() or the SDL3 renderer API.

**Affected Files:** `globals.h`, `setup.c`, `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `practice.c`, `laser.c`, `alphabet.c`, `snow.c`, `pixels.c`

**Acceptance Criteria:** SDL_Surface* screen global removed; all files compile using renderer accessors.'

echo 'Issue 6/45: Replace SDL_WM_SetCaption() with SDL_SetWindowTitle()...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_WM_SetCaption() with SDL_SetWindowTitle()' \
  --body 'SDL_WM_SetCaption(title, icon_title) removed in SDL3. Replace with SDL_SetWindowTitle(window, '\''Tux Typing'\'').

**Affected Files:** `setup.c`

**Acceptance Criteria:** Window title set correctly; no compilation errors.'

echo 'Issue 7/45: Replace SDL_SetColorKey()+SDL_WM_SetIcon() icon setup w...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_SetColorKey()+SDL_WM_SetIcon() icon setup with SDL_SetWindowIcon()' \
  --body 'SDL_WM_SetIcon(icon, NULL) removed. SDL3 uses SDL_SetWindowIcon(window, icon) which reads alpha from the surface directly. The preceding SDL_SetColorKey call for the icon is also unnecessary.

**Affected Files:** `setup.c`

**Acceptance Criteria:** seticon() compiles with SDL3; window icon is correctly displayed.'

echo 'Issue 8/45: Replace SDL_WM_GrabInput() with SDL_SetWindowMouseGrab(...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_WM_GrabInput() with SDL_SetWindowMouseGrab()' \
  --body 'SDL_WM_GrabInput(SDL_GRAB_OFF/ON) and SDL_GRAB_* constants removed in SDL3. Use SDL_SetWindowMouseGrab(window, SDL_FALSE/TRUE).

**Affected Files:** `titlescreen.c`

**Acceptance Criteria:** Both grab call-sites compile and grab/release mouse input correctly.'

echo 'Issue 9/45: Replace SDL_FULLSCREEN surface flag with SDL_SetWindowF...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_FULLSCREEN surface flag with SDL_SetWindowFullscreen()' \
  --body 'Fullscreen is no longer a surface flag. Use SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) to enter fullscreen.

**Affected Files:** `setup.c`

**Acceptance Criteria:** Fullscreen mode uses SDL3 window API; no SDL_FULLSCREEN flag references remain.'

echo 'Issue 10/45: Replace screen->flags & SDL_FULLSCREEN checks with SDL_...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace screen->flags & SDL_FULLSCREEN checks with SDL_GetWindowFlags()' \
  --body 'Reading fullscreen state from a surface flags field is invalid in SDL3. Use SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN.

**Affected Files:** `titlescreen.c`, `SDL_extras.c`

**Acceptance Criteria:** All fullscreen-state checks compile and return correct results.'

echo 'Issue 11/45: Replace SDL_Flip(screen) with SDL_RenderPresent(rendere...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_Flip(screen) with SDL_RenderPresent(renderer)' \
  --body 'SDL_Flip() removed in SDL3. Both occurrences in titlescreen.c must be replaced with SDL_RenderPresent(renderer).

**Affected Files:** `titlescreen.c`

**Acceptance Criteria:** No SDL_Flip references remain; frames present correctly.'

echo 'Issue 12/45: Remove SDL_UpdateRect() calls — superseded by SDL_Rende...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Remove SDL_UpdateRect() calls — superseded by SDL_RenderPresent()' \
  --body 'SDL_UpdateRect(screen, x, y, w, h) removed in SDL3. The renderer redraws the full frame on SDL_RenderPresent(); all partial update calls must be removed.

**Affected Files:** `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `practice.c`, `snow.c`

**Acceptance Criteria:** No SDL_UpdateRect calls remain; display updates correctly.'

echo 'Issue 13/45: Remove SDL_UpdateRects() calls — superseded by SDL_Rend...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Remove SDL_UpdateRects() calls — superseded by SDL_RenderPresent()' \
  --body 'SDL_UpdateRects() removed in SDL3. Calls in titlescreen.c and snow.c must be removed.

**Affected Files:** `titlescreen.c`, `snow.c`

**Acceptance Criteria:** No SDL_UpdateRects calls remain; snow and titlescreen animations render correctly.'

echo 'Issue 14/45: Migrate SDL_BlitSurface() screen-blits to the SDL3 rend...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Migrate SDL_BlitSurface() screen-blits to the SDL3 renderer pipeline' \
  --body 'SDL_BlitSurface(src, srcrect, screen, dstrect) writes to screen which no longer exists. On-screen blits must convert surfaces to textures via SDL_CreateTextureFromSurface() and render with SDL_RenderTexture().

**Affected Files:** `editor.c`, `theme.c`, `titlescreen.c`, `playgame.c`, `laser.c`, `alphabet.c`, `practice.c`

**Acceptance Criteria:** All on-screen blits use the texture/renderer path; game graphics render correctly.'

echo 'Issue 15/45: Migrate screen SDL_FillRect() calls to SDL_RenderFillRe...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Migrate screen SDL_FillRect() calls to SDL_RenderFillRect() / SDL_RenderClear()' \
  --body 'SDL_FillRect(screen, rect, color) invalid in SDL3 renderer pipeline. Use SDL_SetRenderDrawColor() + SDL_RenderFillRect() or SDL_RenderClear() for on-screen fills.

**Affected Files:** `titlescreen.c`, `snow.c`, `pixels.c`

**Acceptance Criteria:** All on-screen fill operations use the SDL3 renderer API.'

echo 'Issue 16/45: Update SDL_MapRGB() call signature for SDL3...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update SDL_MapRGB() call signature for SDL3' \
  --body 'SDL_MapRGB(surface->format, r, g, b) changed in SDL3. First arg is const SDL_PixelFormatDetails* from SDL_GetPixelFormatDetails(); second arg is const SDL_Palette* (NULL if none).

**Affected Files:** `titlescreen.c`, `snow.c`

**Acceptance Criteria:** All SDL_MapRGB call-sites compile; colours render correctly.'

echo 'Issue 17/45: Rewrite COL2RGB macro in titlescreen.h to remove screen...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Rewrite COL2RGB macro in titlescreen.h to remove screen->format dependency' \
  --body 'The macro COL2RGB(col) uses SDL_MapRGB(screen->format, ...) which is invalid in SDL3. Rewrite to use SDL_GetPixelFormatDetails() or pass the format explicitly.

**Affected Files:** `titlescreen.h`

**Acceptance Criteria:** COL2RGB macro compiles and produces correct colour values.'

echo 'Issue 18/45: Replace SDL_FreeSurface() with SDL_DestroySurface() thr...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_FreeSurface() with SDL_DestroySurface() throughout' \
  --body 'SDL_FreeSurface() renamed SDL_DestroySurface() in SDL3.

**Affected Files:** `editor.c`, `theme.c`, `titlescreen.c`, `SDL_extras.c`, `setup.c`, `loaders.c`

**Acceptance Criteria:** No SDL_FreeSurface references remain; no memory leaks introduced.'

echo 'Issue 19/45: Replace SDL_CreateRGBSurface(SDL_SWSURFACE,...) with SD...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_CreateRGBSurface(SDL_SWSURFACE,...) with SDL_CreateSurface() in SDL_extras.c' \
  --body 'SDL_CreateRGBSurface with SDL_SWSURFACE flag is removed. Use SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888) or SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA8888).

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** All surface-creation calls compile with SDL3; text rendering and blending produce correct images.'

echo 'Issue 20/45: Remove direct SDL_SRCCOLORKEY / SDL_SRCALPHA flag manip...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Remove direct SDL_SRCCOLORKEY / SDL_SRCALPHA flag manipulation on surface->flags in SDL_extras.c' \
  --body 'SDL_extras.c directly bitwise-ANDs and ORs SDL_SRCCOLORKEY and SDL_SRCALPHA on surface->flags, which is invalid in SDL3. Use SDL_GetSurfaceColorKey(), SDL_SetSurfaceColorKey(), SDL_GetSurfaceAlphaMod(), SDL_SetSurfaceAlphaMod() instead.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** All surface flag manipulations use SDL3 getter/setter API; no direct surface->flags bitops for color key/alpha.'

echo 'Issue 21/45: Replace SDL_SetColorKey() SDL_SRCCOLORKEY|SDL_RLEACCEL ...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_SetColorKey() SDL_SRCCOLORKEY|SDL_RLEACCEL with SDL_SetSurfaceColorKey()' \
  --body 'SDL_SetColorKey(surface, SDL_SRCCOLORKEY|SDL_RLEACCEL, key) changed to SDL_SetSurfaceColorKey(surface, SDL_TRUE, key). SDL_SRCCOLORKEY and SDL_RLEACCEL removed.

**Affected Files:** `SDL_extras.c`, `setup.c`

**Acceptance Criteria:** All color-key calls compile; sprite transparency works correctly.'

echo 'Issue 22/45: Replace SDL_DisplayFormat() with SDL_ConvertSurface() u...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_DisplayFormat() with SDL_ConvertSurface() using explicit pixel format' \
  --body 'SDL_DisplayFormat() removed in SDL3. Replace with SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGB888) or the appropriate screen-compatible format.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Surface conversion produces correct pixel format; no SDL_DisplayFormat references remain.'

echo 'Issue 23/45: Replace SDL_DisplayFormatAlpha() with SDL_ConvertSurfac...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_DisplayFormatAlpha() with SDL_ConvertSurface() using ARGB format' \
  --body 'SDL_DisplayFormatAlpha() removed in SDL3. Replace with SDL_ConvertSurface(surface, SDL_PIXELFORMAT_ARGB8888).

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Alpha-capable surface conversion compiles and produces correct results.'

echo 'Issue 24/45: Update SDL_ConvertSurface() third argument — remove SDL...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update SDL_ConvertSurface() third argument — remove SDL_SWSURFACE flag' \
  --body 'SDL_ConvertSurface(S1, fmt1, SDL_SWSURFACE) third arg removed in SDL3. New signature is SDL_ConvertSurface(surface, SDL_PixelFormat).

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** SDL_ConvertSurface compiles with SDL3 signature; conversion result is correct.'

echo 'Issue 25/45: Verify TTF_Init() return-value check compatible with SD...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Verify TTF_Init() return-value check compatible with SDL3_ttf' \
  --body 'TTF_Init() is retained in SDL3_ttf but confirm the return-value check (TTF_Init() < 0) is still valid, and update include path to <SDL3_ttf/SDL_ttf.h>.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** SDL3_ttf initialises correctly; include path updated.'

echo 'Issue 26/45: Verify TTF_OpenFont() API unchanged in SDL3_ttf...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Verify TTF_OpenFont() API unchanged in SDL3_ttf' \
  --body 'TTF_OpenFont(filename, size) expected unchanged. Confirm after include path update.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Fonts load correctly; no compile warnings.'

echo 'Issue 27/45: Verify TTF_RenderUTF8_Blended() API unchanged in SDL3_t...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Verify TTF_RenderUTF8_Blended() API unchanged in SDL3_ttf' \
  --body 'TTF_RenderUTF8_Blended(font, text, color) expected unchanged. Confirm return type is SDL_Surface*.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Text renders to surface correctly under SDL3_ttf.'

echo 'Issue 28/45: Verify TTF_CloseFont() and TTF_Quit() unchanged in SDL3...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Verify TTF_CloseFont() and TTF_Quit() unchanged in SDL3_ttf' \
  --body 'Both functions expected unchanged. Confirm after include path update.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** Font resources freed and library quits without errors.'

echo 'Issue 29/45: Update Mix_OpenAudio() call signature for SDL3_mixer 3....'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update Mix_OpenAudio() call signature for SDL3_mixer 3.x' \
  --body 'Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048) changed in SDL3_mixer to Mix_OpenAudio(0, NULL).

**Affected Files:** `setup.c`

**Acceptance Criteria:** Audio initialises without errors; sound plays correctly.'

echo 'Issue 30/45: Rename event type SDL_KEYDOWN / SDL_KEYUP to SDL_EVENT_...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Rename event type SDL_KEYDOWN / SDL_KEYUP to SDL_EVENT_KEY_DOWN / SDL_EVENT_KEY_UP' \
  --body 'All SDL3 event type constants use the SDL_EVENT_ prefix.

**Affected Files:** `titlescreen.c`, `playgame.c`, `practice.c`, `scripting.c`, `laser.c`

**Acceptance Criteria:** All keyboard event type checks use SDL3 names; input functions correctly.'

echo 'Issue 31/45: Rename event type SDL_QUIT to SDL_EVENT_QUIT...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Rename event type SDL_QUIT to SDL_EVENT_QUIT' \
  --body 'Quit event constant renamed SDL_EVENT_QUIT in SDL3.

**Affected Files:** `editor.c`, `titlescreen.c`, `theme.c`, `playgame.c`

**Acceptance Criteria:** Quit-event checks use SDL_EVENT_QUIT; window-close exits cleanly.'

echo 'Issue 32/45: Rename event type SDL_MOUSEBUTTONDOWN to SDL_EVENT_MOUS...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Rename event type SDL_MOUSEBUTTONDOWN to SDL_EVENT_MOUSE_BUTTON_DOWN' \
  --body 'Mouse button event type constant renamed in SDL3.

**Affected Files:** `editor.c`, `titlescreen.c`, `theme.c`, `playgame.c`, `practice.c`

**Acceptance Criteria:** Mouse-button event checks use new SDL3 name; clicks register correctly.'

echo 'Issue 33/45: Rename event type SDL_MOUSEMOTION to SDL_EVENT_MOUSE_MO...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Rename event type SDL_MOUSEMOTION to SDL_EVENT_MOUSE_MOTION' \
  --body 'Mouse motion event type constant renamed in SDL3.

**Affected Files:** `playgame.c`, `practice.c`, `scripting.c`

**Acceptance Criteria:** Mouse motion events correctly identified and processed.'

echo 'Issue 34/45: Update keyboard event field from event.key.keysym.sym t...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update keyboard event field from event.key.keysym.sym to event.key.key' \
  --body 'SDL_Keysym struct removed from SDL3. Access the key symbol as event.key.key (SDL_Keycode).

**Affected Files:** `playgame.c`, `practice.c`

**Acceptance Criteria:** All keysym field accesses compile; key comparisons work correctly.'

echo 'Issue 35/45: Replace event.key.keysym.unicode text input with SDL_EV...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace event.key.keysym.unicode text input with SDL_EVENT_TEXT_INPUT' \
  --body 'keysym.unicode removed in SDL3. Unicode text input is handled via the SDL_EVENT_TEXT_INPUT event and event.text.text field. Enable text input with SDL_StartTextInput().

**Affected Files:** `playgame.c`

**Acceptance Criteria:** All typed characters are received correctly via TEXT_INPUT events; unicode input works.'

echo 'Issue 36/45: Replace SDL_ShowCursor(SDL_ENABLE/DISABLE/1) with SDL_S...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace SDL_ShowCursor(SDL_ENABLE/DISABLE/1) with SDL_ShowCursor() / SDL_HideCursor()' \
  --body 'SDL_ShowCursor(int toggle) split into separate SDL_ShowCursor() and SDL_HideCursor() in SDL3. SDL_ENABLE/SDL_DISABLE constants also removed.

**Affected Files:** `titlescreen.c`, `scripting.c`

**Acceptance Criteria:** Cursor shown and hidden correctly at the appropriate points.'

echo 'Issue 37/45: Update SDL_GetTicks() return type from Uint32 to Uint64...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update SDL_GetTicks() return type from Uint32 to Uint64 throughout' \
  --body 'SDL_GetTicks() returns Uint64 in SDL3. All receiving Uint32 variables must become Uint64 to prevent truncation.

**Affected Files:** `titlescreen.c`, `playgame.c`, `practice.c`

**Acceptance Criteria:** All SDL_GetTicks() results stored in Uint64; timing logic correct.'

echo 'Issue 38/45: Update Uint32 this_tick variable in playgame.c to Uint6...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update Uint32 this_tick variable in playgame.c to Uint64' \
  --body 'Uint32 this_tick = SDL_GetTicks() must become Uint64 to match SDL3 return type.

**Affected Files:** `playgame.c`

**Acceptance Criteria:** this_tick is Uint64; frame timing calculations remain correct.'

echo 'Issue 39/45: Update SDL_GetMouseState() to handle float coordinates ...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update SDL_GetMouseState() to handle float coordinates in SDL3' \
  --body 'SDL_GetMouseState() now returns float coordinates. Call-site casting to int* is invalid. Use float fx, fy locals.

**Affected Files:** `titlescreen.c`

**Acceptance Criteria:** Mouse coordinates read correctly; Easter Egg cursor position accurate.'

echo 'Issue 40/45: Guard direct pixel access in pixels.c with SDL_LockSurf...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Guard direct pixel access in pixels.c with SDL_LockSurface() / SDL_UnlockSurface()' \
  --body 'SDL3 requires SDL_LockSurface() before accessing surface->pixels, surface->pitch, or surface->format->BytesPerPixel.

**Affected Files:** `pixels.c`

**Acceptance Criteria:** All pixel operations lock the surface before access; no crashes or undefined behaviour.'

echo 'Issue 41/45: Update find_package(SDL) to find_package(SDL3) in CMake...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update find_package(SDL) to find_package(SDL3) in CMakeLists.txt' \
  --body 'CMake package name changed for SDL3.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** CMake finds SDL3 headers and libraries; configuration step succeeds.'

echo 'Issue 42/45: Update find_package(SDL_image/mixer/ttf) to SDL3 equiva...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Update find_package(SDL_image/mixer/ttf) to SDL3 equivalents in CMakeLists.txt' \
  --body 'CMake package names changed: SDL_image -> SDL3_image, SDL_mixer -> SDL3_mixer, SDL_ttf -> SDL3_ttf.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** All SDL3 satellite libraries found; project compiles against them.'

echo 'Issue 43/45: Replace raw CMake library variables with imported SDL3 ...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Replace raw CMake library variables with imported SDL3 targets in target_link_libraries' \
  --body 'Replace ${SDL_LIBRARY}, ${SDLIMAGE_LIBRARY}, ${SDLMIXER_LIBRARY}, ${SDLTTF_LIBRARY} with SDL3::SDL3, SDL3_image::SDL3_image, SDL3_mixer::SDL3_mixer, SDL3_ttf::SDL3_ttf.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** Project links correctly against SDL3 using modern CMake imported target syntax.'

echo 'Issue 44/45: Audit and update CMake include_directories for SDL3 hea...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Audit and update CMake include_directories for SDL3 header paths' \
  --body 'SDL3 CMake modules expose include dirs via imported targets; explicit include_directories() calls for SDL headers should be removed or updated.

**Affected Files:** `CMakeLists.txt`, `src/CMakeLists.txt`

**Acceptance Criteria:** No hardcoded SDL include paths remain; headers resolved via imported targets.'

echo 'Issue 45/45: Verify SDL3 compatibility of SDL_extras.c surface blend...'
gh issue create --repo "$REPO" --label "$LABEL" \
  --title '[SDL3] Verify SDL3 compatibility of SDL_extras.c surface blending / rotation helper functions' \
  --body 'SDL_extras.c contains multiple helper functions that blit, rotate, and blend surfaces using SDL1.2 patterns (SDL_DisplayFormat, SDL_SRCALPHA flag ops, SDL_SWSURFACE). Each function must be audited and ported to SDL3 surface API.

**Affected Files:** `SDL_extras.c`

**Acceptance Criteria:** All SDL_extras helpers compile and produce visually correct output under SDL3; no deprecated API calls remain.'

echo "Done — 45 issues created in $REPO."
