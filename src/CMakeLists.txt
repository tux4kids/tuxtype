#
# src/CMakeLists.txt for TuxType (SDL3 Edition)
#
# Compiles all TuxType source files into a single executable.
# Windows/MinGW-specific blocks are guarded by if(MINGW) so the
# Debian/Linux Autotools build is completely unaffected.
#

cmake_minimum_required(VERSION 3.5)

# Set variables consumed by config.h.cmake
set(HAVE_RSVG 1)
if(MINGW)
    set(BUILD_MINGW32 1)
else()
    set(BUILD_MINGW32 0)
endif()

# Generate config.h into the build root (build-win/config.h)
# SDL_extras.h uses #include "../config.h" (one level above src/)
# so we place it in CMAKE_BINARY_DIR which is build-win/
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake
               ${CMAKE_BINARY_DIR}/config.h)

# -------------------------------------------------------------------
# All C source files (from src/Makefile.am tuxtype_SOURCES)
# -------------------------------------------------------------------
set(TUXTYPE_SOURCES
    alphabet.c
    audio.c
    convert_utf.c
    editor.c
    globals.c
    input_methods.c
    laser.c
    loaders.c
    main.c
    menu.c
    mysetenv.c
    options.c
    pause.c
    pixels.c
    playgame.c
    practice.c
    scandir.c
    scripting.c
    SDL_extras.c
    setup.c
    snow.c
    theme.c
    titlescreen.c
    braille.c
)

# -------------------------------------------------------------------
# Executable name: TuxType on Windows, tuxtype on Linux/Mac
# -------------------------------------------------------------------
if(WIN32)
    set(EXE_NAME "TuxType")
else()
    set(EXE_NAME "tuxtype")
endif()

add_executable(${EXE_NAME} ${TUXTYPE_SOURCES})

# -------------------------------------------------------------------
# Standalone TTS Test Tool
# -------------------------------------------------------------------
add_executable(test_tts test_tts.c)

# -------------------------------------------------------------------
# Compile definitions (mirrors Makefile.am AM_CFLAGS / AM_CPPFLAGS)
# -------------------------------------------------------------------
target_compile_definitions(${EXE_NAME} PRIVATE
    DATA_PREFIX="${DATA_PREFIX}"
    VAR_PREFIX="${DATA_PREFIX}"
    CONF_PREFIX="${DATA_PREFIX}"
    IM_PREFIX="${DATA_PREFIX}/input_methods"
    IMDIR="${DATA_PREFIX}/input_methods/"
    LOCALEDIR="${DATA_PREFIX}/locale"
    VERSION="${PACKAGE_VERSION}"
    PACKAGE_STRING="${APP_NAME}-${PACKAGE_VERSION}"
    HAVE_LIBXML2=1
    HAVE_LIBT4K_COMMON=1
    HAVE_RSVG=1
    DEBUG
)

if(MINGW)
    target_compile_definitions(${EXE_NAME} PRIVATE HAVE_PUTENV=1)
endif()

if(WITH_ESPEAK)
    target_compile_definitions(${EXE_NAME} PRIVATE
        WITH_ESPEAK=1
        WITH_ESPEAK_NG=${WITH_ESPEAK_NG}
    )
    target_compile_definitions(test_tts PRIVATE
        WITH_ESPEAK=1
        WITH_ESPEAK_NG=${WITH_ESPEAK_NG}
    )
endif()

# -------------------------------------------------------------------
# Include directories
# -------------------------------------------------------------------
target_include_directories(${EXE_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}           # for generated config.h (globals.h uses it)
    ${CMAKE_BINARY_DIR}/src       # for any src-level generated headers
    ${CMAKE_CURRENT_SOURCE_DIR}   # for local headers
    ${SDL3_INCLUDE_DIRS}
    ${SDL3_IMAGE_INCLUDE_DIRS}
    ${SDL3_TTF_INCLUDE_DIRS}
    ${SDL3_MIXER_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIR}
    ${LIBRSVG_INCLUDE_DIRS}
    ${T4K_COMMON_INCLUDE_DIRS}
)

target_include_directories(test_tts PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL3_INCLUDE_DIRS}
    ${T4K_COMMON_INCLUDE_DIRS}
)

# -------------------------------------------------------------------
# On MinGW: auto-detect paths for librsvg, cairo, glib, libintl etc.
# using pkg-config + compiler-derived MSYS2 prefix (no hardcoding).
# -------------------------------------------------------------------
if(MINGW)
    pkg_check_modules(CAIRO REQUIRED cairo)
    pkg_check_modules(GLIB  REQUIRED glib-2.0)

    # Derive MSYS2 prefix from compiler location (works on any install path)
    if(DEFINED ENV{MSYS2_PREFIX})
        set(_msys2_prefix "$ENV{MSYS2_PREFIX}")
    else()
        get_filename_component(_msys2_prefix "${CMAKE_C_COMPILER}" DIRECTORY)
        get_filename_component(_msys2_prefix "${_msys2_prefix}" DIRECTORY)
    endif()

    find_library(LIBINTL_LIBRARIES  NAMES intl libintl
        HINTS "${_msys2_prefix}/lib" REQUIRED)
    find_library(LIBICONV_LIBRARIES NAMES iconv libiconv
        HINTS "${_msys2_prefix}/lib" REQUIRED)
    find_library(LIBPNG_LIBRARIES   NAMES png libpng png16 libpng16
        HINTS "${_msys2_prefix}/lib" REQUIRED)
    find_library(ZLIB_LIBRARIES     NAMES z zlib
        HINTS "${_msys2_prefix}/lib" REQUIRED)

    target_include_directories(${EXE_NAME} PRIVATE
        ${LIBRSVG_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
    )
    target_include_directories(test_tts PRIVATE
        ${LIBRSVG_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
    )

    link_directories(${LIBRSVG_LIBRARY_DIRS})
endif()

# -------------------------------------------------------------------
# Link libraries — common to all platforms
# -------------------------------------------------------------------
target_link_directories(${EXE_NAME} PRIVATE ${T4K_COMMON_LIBRARY_DIRS})
target_link_directories(test_tts PRIVATE ${T4K_COMMON_LIBRARY_DIRS})

target_link_libraries(${EXE_NAME}
    PkgConfig::SDL3
    PkgConfig::SDL3_IMAGE
    PkgConfig::SDL3_TTF
    PkgConfig::SDL3_MIXER
    ${LIBXML2_LIBRARIES}
    ${LIBRSVG_LIBRARIES}
    ${T4K_COMMON_LIBRARIES}
)

target_link_libraries(test_tts
    PkgConfig::SDL3
    PkgConfig::SDL3_MIXER
    ${LIBRSVG_LIBRARIES}
    ${T4K_COMMON_LIBRARIES}
)

# -------------------------------------------------------------------
# On MinGW: also link gettext, iconv, png, zlib, Windows system libs
# -------------------------------------------------------------------
if(MINGW)
    target_link_libraries(${EXE_NAME}
        ${LIBINTL_LIBRARIES}
        ${LIBICONV_LIBRARIES}
        ${LIBPNG_LIBRARIES}
        ${ZLIB_LIBRARIES}
        # Required Windows system libraries
        uuid ole32 wsock32 shlwapi
    )
    target_link_libraries(test_tts
        ${LIBINTL_LIBRARIES}
        ${LIBICONV_LIBRARIES}
        uuid ole32 wsock32 shlwapi
    )
    # -mwindows: suppresses the console window in the final .exe
    # set_target_properties(${EXE_NAME} PROPERTIES
    #     LINK_FLAGS "-mwindows"
    # )
endif()

# On Linux/Mac, link math library
if(UNIX)
    target_link_libraries(${EXE_NAME} m)
endif()

# On Linux: embed the local t4k_common library path into the binary's RPATH so
# the dynamic loader finds our freshly-built .so at runtime, not the stale
# system-installed one. Guarded by UNIX AND NOT APPLE so this block has
# zero effect on macOS (which uses dylibbundler) and Windows (no RPATH concept).
if(UNIX AND NOT APPLE)
    set_target_properties(${EXE_NAME} PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/../t4kcommon/build/src"
    )
endif()

# -------------------------------------------------------------------
# espeak-ng / espeak (TTS) — optional
# -------------------------------------------------------------------
if(WITH_ESPEAK)
    if(WITH_ESPEAK_NG)
        target_include_directories(${EXE_NAME} PRIVATE ${ESPEAK_NG_INCLUDE_DIRS})
        target_link_libraries(${EXE_NAME} ${ESPEAK_NG_LIBRARIES})
        
        target_include_directories(test_tts PRIVATE ${ESPEAK_NG_INCLUDE_DIRS})
        target_link_libraries(test_tts ${ESPEAK_NG_LIBRARIES})
    else()
        target_include_directories(${EXE_NAME} PRIVATE ${ESPEAK_INCLUDE_DIRS})
        target_link_libraries(${EXE_NAME} ${ESPEAK_LIBRARIES})
        
        target_include_directories(test_tts PRIVATE ${ESPEAK_INCLUDE_DIRS})
        target_link_libraries(test_tts ${ESPEAK_LIBRARIES})
    endif()
endif()

# -------------------------------------------------------------------
# SDL3_net (optional, for future network features)
# -------------------------------------------------------------------
if(SDL3_NET_FOUND)
    target_link_libraries(${EXE_NAME} ${SDL3_NET_LIBRARIES})
    target_include_directories(${EXE_NAME} PRIVATE ${SDL3_NET_INCLUDE_DIRS})
    target_compile_definitions(${EXE_NAME} PRIVATE HAVE_LIBSDL_NET=1)
endif()

# -------------------------------------------------------------------
# Install
# -------------------------------------------------------------------
install(TARGETS ${EXE_NAME} DESTINATION bin)

# -------------------------------------------------------------------
# Copy data folder on Windows so it can be run from the build directory
# -------------------------------------------------------------------
if(WIN32)
    add_custom_command(TARGET ${EXE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:${EXE_NAME}>/data"
        COMMENT "Copying data folder to output directory..."
    )
endif()
