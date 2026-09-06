# TuxType (SDL3 Edition)

**TuxType** is an educational typing tutor game starring Tux, the Linux Penguin. This repository has been migrated to **SDL3** and updated with accessibility features (Text-To-Speech navigation, Braille support, and localized audio feedback).

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

## Build & Installation Guide for TuxType

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

## Running TuxType

To run TuxType directly from the build directory:

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
