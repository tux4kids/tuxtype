#
# CMakeLists.txt for TuxType (SDL3 Edition)
#
# Provides native Windows (MinGW/MSYS2) build support alongside the
# existing Autotools build for Debian/Linux.
# All Windows-specific blocks are guarded by if(MINGW) so the Linux
# build is completely unaffected.
#

cmake_minimum_required(VERSION 3.5)

set(PACKAGE_NAME "tuxtype")
set(PACKAGE_VERSION "3.0.0")
set(APP_NAME "TuxType")
project(${APP_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")

# -------------------------------------------------------------------
# Find required libraries via pkg-config (works on both Linux + MinGW)
# -------------------------------------------------------------------
find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL3       REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED IMPORTED_TARGET sdl3-image)
pkg_check_modules(SDL3_TTF   REQUIRED IMPORTED_TARGET sdl3-ttf)
pkg_check_modules(SDL3_MIXER REQUIRED IMPORTED_TARGET sdl3-mixer)
pkg_check_modules(SDL3_NET   IMPORTED_TARGET sdl3-net)
pkg_check_modules(LIBRSVG    REQUIRED librsvg-2.0)
pkg_check_modules(LIBXML2    REQUIRED libxml-2.0)
pkg_check_modules(ESPEAK_NG           espeak-ng)

find_package(LibXml2 REQUIRED)

# t4kcommon — prefer the locally-built version on Linux to avoid picking up a
# stale system-installed libt4k_common.so that is missing SDL3-era symbols.
#
# Strategy by platform:
#   Linux  : if ../t4kcommon/build/src exists, always use it (bypass pkg-config).
#             This prevents the old system package from being chosen.
#   macOS  : falls into the else branch; CMAKE_PREFIX_PATH already points to
#             stage/ (set by build-mac.sh), so pkg-config finds the right one.
#   MinGW  : no system t4k_common exists; pkg-config fails → fallback used as before.
set(_T4K_LOCAL_LIB_DIR "${CMAKE_SOURCE_DIR}/../t4kcommon/build/src")
set(_T4K_LOCAL_INC_DIR "${CMAKE_SOURCE_DIR}/../t4kcommon/src")

if(UNIX AND NOT APPLE AND EXISTS "${_T4K_LOCAL_LIB_DIR}")
    # Linux: always use the locally-built libt4k_common to avoid stale system package
    set(T4K_COMMON_INCLUDE_DIRS "${_T4K_LOCAL_INC_DIR}")
    set(T4K_COMMON_LIBRARY_DIRS "${_T4K_LOCAL_LIB_DIR}")
    set(T4K_COMMON_LIBRARIES    "t4k_common")
    message(STATUS "Linux: using local t4k_common from: ${_T4K_LOCAL_LIB_DIR}")
else()
    # macOS / Windows / fallback: let pkg-config (or CMAKE_PREFIX_PATH) find it
    pkg_check_modules(T4K_COMMON t4k_common)
    if(NOT T4K_COMMON_FOUND)
        set(T4K_COMMON_INCLUDE_DIRS "${_T4K_LOCAL_INC_DIR}")
        set(T4K_COMMON_LIBRARY_DIRS "${_T4K_LOCAL_LIB_DIR}")
        set(T4K_COMMON_LIBRARIES    "t4k_common")
        message(STATUS "t4k_common not found via pkg-config, using fallback: ${_T4K_LOCAL_LIB_DIR}")
    endif()
endif()

# -------------------------------------------------------------------
# Platform-specific data prefix (where the game looks for its files)
# -------------------------------------------------------------------
if(APPLE)
    set(DATA_PREFIX "data")
elseif(WIN32)
    # On Windows, data sits next to the .exe in the install directory
    set(DATA_PREFIX "data")
    if(MINGW)
        add_definitions(-DBUILD_MINGW32)
    endif()
elseif(UNIX)
    set(DATA_PREFIX "${CMAKE_INSTALL_PREFIX}/share/${PACKAGE_NAME}")
else()
    message(SEND_ERROR "Unknown platform!")
endif()

# -------------------------------------------------------------------
# espeak-ng (TTS) — optional
# -------------------------------------------------------------------
if(ESPEAK_NG_FOUND)
    set(WITH_ESPEAK 1)
    set(WITH_ESPEAK_NG 1)
else()
    pkg_check_modules(ESPEAK espeak)
    if(ESPEAK_FOUND)
        set(WITH_ESPEAK 1)
        set(WITH_ESPEAK_NG 0)
    endif()
endif()

# -------------------------------------------------------------------
# Process the src/ subdirectory
# -------------------------------------------------------------------
add_subdirectory(src)

# -------------------------------------------------------------------
# Compile translations (.po -> .mo)
# -------------------------------------------------------------------
find_program(MSGFMT_EXECUTABLE msgfmt)
if(MSGFMT_EXECUTABLE)
    file(GLOB PO_FILES "${CMAKE_SOURCE_DIR}/po/*.po")
    set(MO_FILES "")
    foreach(PO_FILE ${PO_FILES})
        get_filename_component(LANG ${PO_FILE} NAME_WE)
        
        # Avoid compiling the template or backup files if they accidentally match
        if(LANG STREQUAL "en@boldquot" OR LANG STREQUAL "en@quot")
            continue()
        endif()

        set(MO_DIR "${CMAKE_SOURCE_DIR}/data/locale/${LANG}/LC_MESSAGES")
        set(MO_FILE "${MO_DIR}/${PACKAGE_NAME}.mo")
        
        add_custom_command(
            OUTPUT "${MO_FILE}"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${MO_DIR}"
            COMMAND ${MSGFMT_EXECUTABLE} -c -v -o "${MO_FILE}" "${PO_FILE}"
            DEPENDS "${PO_FILE}"
            COMMENT "Compiling translation for ${LANG}"
        )
        list(APPEND MO_FILES "${MO_FILE}")
    endforeach()

    if(MO_FILES)
        add_custom_target(translations ALL DEPENDS ${MO_FILES})
    endif()
else()
    message(WARNING "msgfmt not found. Translations will not be compiled.")
endif()
