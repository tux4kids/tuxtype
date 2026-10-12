# TuxType (SDL3 Edition)

**TuxType** is an educational typing tutor game starring Tux, the Linux Penguin. This repository has been migrated to **SDL3** and updated with accessibility features (Text-To-Speech navigation, Braille support, and localized audio feedback).

---

## Project Team & Organization

This SDL3 edition of TuxType is developed under **[Zendalona](https://zendalona.com/)**.

| Role | Name | Contact |
|---|---|---|
| **Founder, Zendalona** | Nalin Sathyan | [Nalin.x.Linux@gmail.com](mailto:Nalin.x.Linux@gmail.com) |
| **Mentor** | Nalin Sathyan | — |
| **Mentor** | Mukundhan Annamalai | — |
| **Mentor** | Deepak Aggarwal | [deepak.aggarwal9@gmail.com](mailto:deepak.aggarwal9@gmail.com) |
| **SDL3 Migration & Lead Developer** | Midhun M | [mmidhun781@gmail.com](mailto:mmidhun781@gmail.com) |
| **SDL3 Migration Co-Developer** | Bholu Kumar D | [bholukumar3352@gmail.com](mailto:bholukumar3352@gmail.com) |

### Key Contributions by Midhun M & Bholu Kumar D
- **SDL 1.2 → SDL3 migration**: Complete port of the entire codebase from SDL 1.2 to SDL3
- **F5 — TTS Toggle**: Implementation of runtime Text-To-Speech toggle (F5 key)
- **F9 — Braille Toggle**: Implementation of runtime Braille mode toggle (F9 key)
- **Windows build**: Native Windows MSIX package and CMake build system
- **macOS build**: macOS app bundle and build system
- **Ubuntu build**: Linux CMake build system and CI workflow

> Mentored by **Nalin Sathyan** (Founder, Zendalona), **Mukundhan Annamalai**, and **Deepak Aggarwal**, under the **Zendalona** organization.

---

## Prerequisites

Before compiling TuxType, make sure `t4kcommon` (SDL3 version) and development libraries are installed on your system.

```bash
# Core Dependencies (Debian / Ubuntu / Linux Mint)
sudo apt-get update
sudo apt-get install -y build-essential autoconf automake libtool pkg-config libxml2-dev librsvg2-dev libpng-dev libespeak-ng-dev
```

### Install t4kcommon First
```bash
git clone https://github.com/Midhun-M-git/t4kcommon.git
cd t4kcommon
mkdir -p build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
sudo ldconfig
```

---

## Build & Installation Guide (Linux / Autotools)

### Step 1: Clone the Repository
```bash
git clone https://github.com/Midhun-M-git/tuxtype.git
cd tuxtype
```

### Step 2: Build with Autotools
```bash
# Generate build configuration files
./autogen.sh

# Configure TuxType
./configure

# Build executable
make -j$(nproc)
```

### Step 3: Install
```bash
sudo make install
```

---

## Build & Installation Guide (Windows / CMake)

TuxType has been modernized to support native Windows builds using **CMake** and **vcpkg** (for SDL3 dependency management).

### Step 1: Build `t4kcommon` First
```powershell
git clone https://github.com/Midhun-M-git/t4kcommon.git
cd t4kcommon
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

### Step 2: Build `tuxtype`
```powershell
git clone https://github.com/Midhun-M-git/tuxtype.git
cd tuxtype
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```
*(Note: CMake will automatically find and compile the localized `.po` translation files into binary `.mo` formats during the build).*

---

## Running TuxType

To run TuxType directly from the build directory on Linux:

```bash
cd tuxtype/src
LD_LIBRARY_PATH=/usr/local/lib ./tuxtype
```

Or if installed system-wide:
```bash
tuxtype
```

---

## Key Shortcuts & Accessibility Controls

| Key | Description |
|---|---|
| **F5** | Toggle Text-To-Speech (TTS) On / Off |
| **F9** | Toggle Braille Output Mode On / Off |
| **ESC** | Pause game or Return to Previous Menu |
| **Space** | Continue / Skip dialogs |

---

## Features & Game Modes
- **Fish Cascade**: Help Tux catch falling fish by typing the letters.
- **Comet Zap**: Protect cities from incoming comets using laser typing defense.
- **Practice & Lessons**: Practice alphabet typing and finger positioning exercises.
- **Accessibility Integration**: Real-time TTS screen reading, multi-language voice support, and localized gettext translations.

### Recent Updates & Bug Fixes
- **Asynchronous Window Resizing**: Support for modern OS window managers via SDL3 event listening for `SDL_EVENT_WINDOW_RESIZED`, fixing `F10` fullscreen rendering bugs and clipping issues.
- **Memory Optimization**: Plugged severe memory leaks in the Wordlist Editor and dynamic text surfaces.
- **Advanced Text Rendering**: Upgraded the `T4K_sdl.c` text engine to dynamically calculate spacing and bounding boxes for complex multi-line scripts (like Malayalam).
- **Automated Translation Building**: CMake automatically compiles `.po` translation files and packages them.
