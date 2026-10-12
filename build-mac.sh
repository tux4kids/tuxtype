#!/bin/bash
set -euo pipefail

# ==============================================================================
# Build & Package TuxType for macOS (.app, .dmg, .pkg)
# ==============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

echo "==> 1. Verifying build dependencies..."
for cmd in cmake pkg-config dylibbundler; do
    if ! command -v "$cmd" &>/dev/null; then
        echo "Error: '$cmd' is required. Run: brew install $cmd" >&2
        exit 1
    fi
done

HOMEBREW_PREFIX="$(brew --prefix)"
GETTEXT_PREFIX="$(brew --prefix gettext)"

# Staging directory for t4kcommon
STAGE_DIR="${SCRIPT_DIR}/stage"
mkdir -p "${STAGE_DIR}"

echo "==> 2. Building and installing t4kcommon..."
rm -rf t4kcommon/build
mkdir -p t4kcommon/build
cd t4kcommon/build
cmake .. \
  -DCMAKE_INSTALL_PREFIX="${STAGE_DIR}" \
  -DCMAKE_PREFIX_PATH="${HOMEBREW_PREFIX};${GETTEXT_PREFIX}" \
  -DCMAKE_C_FLAGS="-I${HOMEBREW_PREFIX}/include -I${GETTEXT_PREFIX}/include" \
  -DCMAKE_SHARED_LINKER_FLAGS="-L${HOMEBREW_PREFIX}/lib -L${GETTEXT_PREFIX}/lib -lintl -liconv -lpng" \
  -DCMAKE_EXE_LINKER_FLAGS="-L${HOMEBREW_PREFIX}/lib -L${GETTEXT_PREFIX}/lib -lintl -liconv -lpng"
make -j"$(sysctl -n hw.ncpu)"
make install
cd "${SCRIPT_DIR}"

echo "==> 3. Building tuxtype..."
rm -rf tuxtype/build
mkdir -p tuxtype/build
cd tuxtype/build
cmake .. \
  -DCMAKE_INSTALL_PREFIX="${STAGE_DIR}" \
  -DCMAKE_PREFIX_PATH="${HOMEBREW_PREFIX};${GETTEXT_PREFIX};${STAGE_DIR}" \
  -DCMAKE_C_FLAGS="-I${HOMEBREW_PREFIX}/include -I${GETTEXT_PREFIX}/include -I${STAGE_DIR}/include -DHAVE_SETENV=1" \
  -DCMAKE_EXE_LINKER_FLAGS="-L${HOMEBREW_PREFIX}/lib -L${GETTEXT_PREFIX}/lib -L${STAGE_DIR}/lib -lintl -liconv -lpng"
make -j"$(sysctl -n hw.ncpu)"
make translations || true
cd "${SCRIPT_DIR}"

echo "==> 4. Assembling TuxType.app bundle..."
rm -rf TuxType.app
mkdir -p TuxType.app/Contents/MacOS
mkdir -p TuxType.app/Contents/Resources/data
mkdir -p TuxType.app/Contents/Frameworks

cp tuxtype/build/src/tuxtype TuxType.app/Contents/MacOS/
cp tuxtype/Info.plist TuxType.app/Contents/
cp tuxtype/entitlements.plist TuxType.app/Contents/
cp -R tuxtype/data/* TuxType.app/Contents/Resources/data/
cp tuxtype/data/images/icons/tuxtype.icns TuxType.app/Contents/Resources/
if [ -d "${HOMEBREW_PREFIX}/share/espeak-ng-data" ]; then
  echo "==> Copying espeak-ng-data for offline speech synthesis..."
  cp -RL "${HOMEBREW_PREFIX}/share/espeak-ng-data" TuxType.app/Contents/Resources/
fi

echo "==> 5. Bundling dynamic libraries with dylibbundler..."
dylibbundler -b \
  -x TuxType.app/Contents/MacOS/tuxtype \
  -d TuxType.app/Contents/Frameworks/ \
  -p @executable_path/../Frameworks/ \
  -s "${HOMEBREW_PREFIX}/lib" \
  -s "${GETTEXT_PREFIX}/lib" \
  -s "${STAGE_DIR}/lib" \
  -s t4kcommon/build/src \
  -of

echo "==> 6. Deduplicating LC_RPATH entries..."
python3 -c '
import os, subprocess, glob

def dedupe_rpaths(path):
    proc = subprocess.run(["otool", "-l", path], capture_output=True, text=True)
    rpaths = []
    lines = proc.stdout.splitlines()
    for i, line in enumerate(lines):
        if "cmd LC_RPATH" in line:
            for j in range(i+1, min(i+4, len(lines))):
                if "path " in lines[j]:
                    p = lines[j].split("path ")[1].split(" (offset")[0].strip()
                    rpaths.append(p)
                    break
    seen = set()
    duplicates = []
    for r in rpaths:
        if r in seen:
            duplicates.append(r)
        else:
            seen.add(r)
    if duplicates:
        for r in duplicates:
            subprocess.run(["install_name_tool", "-delete_rpath", r, path], check=True)
        subprocess.run(["codesign", "-f", "-s", "-", path], check=True)

files = [f for f in glob.glob("TuxType.app/Contents/Frameworks/*.dylib")] + ["TuxType.app/Contents/MacOS/tuxtype"]
for f in files:
    if os.path.isfile(f) and not os.path.islink(f):
        dedupe_rpaths(f)
'

codesign --deep --force --verify --verbose --sign - TuxType.app

echo "==> 7. Creating TuxType-macOS.dmg (for direct download)..."
rm -rf dmg_root TuxType-macOS.dmg
mkdir -p dmg_root
cp -R TuxType.app dmg_root/
ln -s /Applications dmg_root/Applications
hdiutil create -volname "TuxType" -srcfolder dmg_root -ov -format UDZO TuxType-macOS.dmg
rm -rf dmg_root

echo "==> 8. Creating TuxType-AppStore.pkg (for App Store)..."
pkgbuild --component TuxType.app --install-location /Applications TuxType-AppStore.pkg

echo "=============================================================================="
echo " Packaging complete!"
echo " - App Bundle: $(pwd)/TuxType.app"
echo " - Disk Image: $(pwd)/TuxType-macOS.dmg ($(du -sh TuxType-macOS.dmg | cut -f1))"
echo " - App Store:  $(pwd)/TuxType-AppStore.pkg ($(du -sh TuxType-AppStore.pkg | cut -f1))"
echo "=============================================================================="
