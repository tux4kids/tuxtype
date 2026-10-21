/* SPDX-License-Identifier: GPL-3.0-or-later */

/* Exercise Unicode input through the SDL word-list editor and saved files.
 * The games expect uppercase words, including letters outside ASCII.
 * This test uses the dummy video driver and does not need a display.
 */

#include "globals.h"
#include "funcs.h"
#include "SDL_extras.h"
#include "editor.h"

#include <unistd.h>
#include <wctype.h>

SDL_Surface *screen;
SDL_Event event;

#define CHECK(condition) do { \
  if (!(condition)) { \
    fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, #condition); \
    exit(1); \
  } \
} while (0)

static void key(SDLKey sym, Uint16 unicode)
{
  SDL_Event input = {0};
  input.type = SDL_KEYDOWN;
  input.key.keysym.sym = sym;
  input.key.keysym.unicode = unicode;
  CHECK(SDL_PushEvent(&input) == 0);
}

static void type_word(const wchar_t *word)
{
  while (*word)
    key(SDLK_UNKNOWN, *word++);
}

static void check_word(const char *path, const char *initial,
                       const wchar_t *input, const char *expected)
{
  char line[MAX_WORD_SIZE + 1];
  FILE *file = fopen(path, "w");
  CHECK(file != NULL);
  CHECK(fprintf(file, "Swedish\n%s\n", initial) > 0);
  CHECK(fclose(file) == 0);

  type_word(input);
  key(SDLK_ESCAPE, 0);
  EditWordList("swedish.txt");

  file = fopen(path, "r");
  CHECK(file != NULL);
  CHECK(fgets(line, sizeof(line), file) != NULL); /* Title */
  CHECK(fgets(line, sizeof(line), file) != NULL);
  CHECK(fclose(file) == 0);
  line[strcspn(line, "\n")] = '\0';
  fprintf(stderr, "Saved word: %s (expected %s)\n", line, expected);
  CHECK(strcmp(line, expected) == 0);
  CHECK(GenerateWordList(path) == 1);
}

int main(void)
{
  const char *locales[] = {"C.UTF-8", "en_US.UTF-8", "sv_SE.UTF-8", ""};
  char directory[] = "/tmp/tuxtype-editor-XXXXXX";
  char words[FNLEN], path[FNLEN], title[MAX_WORD_SIZE + 1];
  FILE *file;
  unsigned int i;

  for (i = 0; i < sizeof(locales) / sizeof(locales[0]); i++)
    if (setlocale(LC_ALL, locales[i]) && towupper(L'\u00e5') == L'\u00c5')
      break;
  if (i == sizeof(locales) / sizeof(locales[0]))
  {
    fprintf(stderr, "A Unicode-capable locale is required for this test\n");
    return 77;
  }

  /* Fail instead of leaving make check waiting if an editor stops handling
   * the queued exit key. All files are isolated from the user's word lists. */
  alarm(30);
  CHECK(setenv("SDL_VIDEODRIVER", "dummy", 1) == 0);
  CHECK(setenv("SDL_AUDIODRIVER", "dummy", 1) == 0);
  CHECK(mkdtemp(directory) != NULL);
  snprintf(words, sizeof(words), "%s/words", directory);
  CHECK(mkdir(words, 0700) == 0);
  snprintf(path, sizeof(path), "%s/words/swedish.txt", directory);

  Opts_Initialize();
  settings.sys_sound = 0;
  settings.fullscreen = 0;
  settings.tts = 0;
  text_to_speech_status = 0;
  snprintf(settings.default_data_path, FNLEN, "%s", TEST_DATA_DIR);
  snprintf(settings.theme_data_path, FNLEN, "%s/themes/svenska", TEST_DATA_DIR);
  snprintf(settings.user_settings_path, FNLEN, "%s", directory);
  settings.use_english = 0;
  LibInit(SDL_INIT_VIDEO);
  GraphicsInit();
  LoadBothBkgds("title/menu_bkg.jpg");
  CHECK(LoadKeyboard());

  check_word(path, "", L"\u00e5\u00e4\u00f6", "\xc3\x85\xc3\x84\xc3\x96");
  check_word(path, "", L"\u00c5\u00c4\u00d6", "\xc3\x85\xc3\x84\xc3\x96");
  check_word(path, "\xc3\x85", L"\u00e4\u00f6", "\xc3\x85\xc3\x84\xc3\x96");
  check_word(path, "", L"tux", "TUX");
  CHECK(unlink(path) == 0);

  /* List names pass through a separate SDL input loop. */
  type_word(L"\u00e5\u00e4\u00f6");
  key(SDLK_RETURN, 0);
  CHECK(CreateNewWordList() == 1);
  snprintf(path, sizeof(path), "%s/words/\xc3\x85\xc3\x84\xc3\x96.txt", directory);
  file = fopen(path, "r");
  CHECK(file != NULL);
  CHECK(fgets(title, sizeof(title), file) != NULL);
  CHECK(fclose(file) == 0);
  CHECK(strcmp(title, "\xc3\x85\xc3\x84\xc3\x96") == 0);

  FreeBothBkgds();
  SDL_Quit();
  CHECK(unlink(path) == 0);
  CHECK(rmdir(words) == 0);
  CHECK(rmdir(directory) == 0);
  return 0;
}
