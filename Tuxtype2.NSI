; Tuxtype2.nsi
;
; This script is based on Tuxtype1.nsi, but adds uninstall support
; and (optionally) start menu shortcuts.
;
; It will install Tuxtype2.exe into a directory that the user selects,
;

; The name of the installer
Name "Tuxtype2"

; The file to write
OutFile "Tuxtype2-setup.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Tux4kids\Tuxtype2
; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM SOFTWARE\NSIS_Tuxtype2 "Install_Dir"

; The text to prompt the user to enter a directory
ComponentText "This will install Tuxtype2 on your computer. Select which optional things you want installed."
; The text to prompt the user to enter a directory
DirText "Choose a directory to install in to:"

; The stuff to install
Section "Tuxtype2 (required)"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; Put file there
  File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\autorun.inf"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\jpeg.dll"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\libpng1.dll"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\SDL.dll"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\SDL_image.dll"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\SDL_mixer.dll"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\SDL_ttf.dll"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\tuxtype2.exe"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype2.ico"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\zlib.dll"
SetOutPath $INSTDIR\data
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\keyboard.lst"
SetOutPath $INSTDIR\data\fonts
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\fonts\letters.ttf"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\fonts\menu.ttf"
SetOutPath $INSTDIR\data\images
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\down.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\fishy0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\fishy1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\fishy2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\fishy3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hidden.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas1_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas1_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas1_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas1_4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas2_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas2_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas2_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas2_4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas3_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas3_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas3_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\kcas3_4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\left.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\main_bkg.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_6.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_7.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_8.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\num_9.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\pract.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\right.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\splat0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\standby.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\title1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\up.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas1_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas1_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas1_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas1_4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas2_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas2_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas2_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas2_4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas3_1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas3_2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas3_3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\wcas3_4.png"
SetOutPath $INSTDIR\data\images\backgrounds
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\backgrounds\0.jpg"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\backgrounds\1.jpg"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\backgrounds\2.jpg"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\backgrounds\3.jpg"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\backgrounds\4.jpg"
SetOutPath $INSTDIR\data\images\cities
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\cdead-blue.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\cdead-green.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\cdead-orange.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\cdead-red.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\city-blue.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\city-green.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\city-orange.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\city-red.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-blue-1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-blue-2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-blue-3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-blue-4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-blue-5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-green-1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-green-2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-green-3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-green-4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-green-5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-orange-1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-orange-2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-orange-3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-orange-4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-orange-5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-red-1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-red-2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-red-3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-red-4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\csplode-red-5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\shield0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\shield1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\shield2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\shield3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\cities\shields.png"
SetOutPath $INSTDIR\data\images\comets
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\comets\comet1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\comets\comet2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\comets\comet3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\comets\cometex1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\comets\cometex2.png"
SetOutPath $INSTDIR\data\images\hands
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\6.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\7.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\8.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\9.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\hands\hands.png"
SetOutPath $INSTDIR\data\images\menu
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade10.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade6.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade7.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade8.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascade9.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cascaded.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\comet0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\comet1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\comet2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\comet3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\comet4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\comet5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\cometd.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keyboard0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keyboard1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keyboard2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keyboardd.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas6.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas7.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas8.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycas9.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\keycasd.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\lesson0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\lesson1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\lesson2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\lesson3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\lessond.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main6.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\main7.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\maind.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit5.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit6.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit7.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quit8.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\quitd.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\reg0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\reg1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\reg2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\sel0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\sel1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\menu\sel2.png"
SetOutPath $INSTDIR\data\images\status
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\gameover.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\numbers.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\score.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\tux_helmet1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\tux_helmet2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\tux_helmet3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\status\wave.png"
SetOutPath $INSTDIR\data\images\tux
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\console.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\gulp0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\gulp1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\gulp2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\gulp3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\run0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\run1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\stand0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\stand1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-console1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-console2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-console3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-console4.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-drat.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-egypt0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-egypt1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-fist0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-fist1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-kiss1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-kiss2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-relax1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-relax2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-sit.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-yay1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-yay2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-yes1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-yes2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\tux-yipe.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\walk0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\walk1.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\walk2.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\walk3.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\win0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\images\tux\yipe0.png"
SetOutPath $INSTDIR\data\sounds
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\README_SOUNDS.TXT"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\alarm.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\bite.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\buzz.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\click.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\excuseme.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\explosion.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\funkbrk.it"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\game.mod"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\game2.mod"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\game3.mod"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\kmus1.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\kmus2.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\kmus3.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\kmus4.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\laser.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\lose.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\pause.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\pop.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\run.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\shieldsdown.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\splat.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\tock.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\tuxi.ogg"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\win.wav"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\sounds\winfinal.wav"
SetOutPath $INSTDIR\data\themes
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\lang.po"
SetOutPath $INSTDIR\data\themes\armenian
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\armenian\lang.po"
SetOutPath $INSTDIR\data\themes\armenian\fonts
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\armenian\fonts\letters.ttf"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\armenian\fonts\menu.ttf"
SetOutPath $INSTDIR\data\themes\armenian\images
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\armenian\images\splat0.png"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\armenian\images\title1.png"
SetOutPath $INSTDIR\data\themes\dansk
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\dansk\keyboard.lst"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\dansk\lang.po"
SetOutPath $INSTDIR\data\themes\dansk\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\dansk\words\words1.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\dansk\words\words2.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\dansk\words\words3.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\dansk\words\words4.txt"
SetOutPath $INSTDIR\data\themes\espanol
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\espanol\lang.po"
SetOutPath $INSTDIR\data\themes\espanol\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\espanol\words\words1.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\espanol\words\words2.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\espanol\words\words3.txt"
SetOutPath $INSTDIR\data\themes\french
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\french\lang.po"
SetOutPath $INSTDIR\data\themes\french\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\french\words\words1.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\french\words\words2.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\french\words\words3.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\french\words\words4.txt"
SetOutPath $INSTDIR\data\themes\nynorsk
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\lang.po"
SetOutPath $INSTDIR\data\themes\nynorsk\images
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\images\splat0.png"
SetOutPath $INSTDIR\data\themes\nynorsk\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\words\3boks.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\words\4boks.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\words\5boks.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\words\6boks.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\words\dyrenamn.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\nynorsk\words\frukt.txt"
SetOutPath $INSTDIR\data\themes\spanish
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\spanish\lang.po"
SetOutPath $INSTDIR\data\themes\spanish\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\spanish\words\words1.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\spanish\words\words2.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\spanish\words\words3.txt"
SetOutPath $INSTDIR\data\themes\deutsch
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\lang.po"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\keyboard.lst"
SetOutPath $INSTDIR\data\themes\deutsch\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\words\kurz2-3.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\words\kurz2-3-mit.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\words\lang7-8.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\words\lang7-8-mit.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\words\mittel4-6.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\themes\deutsch\words\mittel4-6-mit.txt"
SetOutPath $INSTDIR\data\words
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\words\fingers.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\words\plants.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\words\words1.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\words\words2.txt"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\tuxtype\data\words\words3.txt"
SetOutPath $INSTDIR\docs\en
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\COPYING"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\ChangeLog"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\INSTALL"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\README"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\README.Win32"
File "C:\msys\1.0\home\lab\tuxtype2-1.5.1\TODO"

SetOutPath $INSTDIR
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NSIS_Tuxtype2 "Install_Dir" "$INSTDIR"
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tuxtype2" "DisplayName" "NSIS Tuxtype2 (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tuxtype2" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteUninstaller "uninstall.exe"
SectionEnd

; optional section
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\Tuxtype2"
  CreateShortCut "$SMPROGRAMS\Tuxtype2\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Tuxtype2\Tuxtype2.lnk" "$INSTDIR\Tuxtype2.exe" "" "$INSTDIR\Tuxtype2.exe" 0
  CreateShortCut "$DESKTOP\Tuxtype2.lnk" "$INSTDIR\Tuxtype2.exe" "" "$INSTDIR\Tuxtype2.exe" 0
SectionEnd

; uninstall stuff

UninstallText "This will uninstall Tuxtype2. Hit next to continue."

; special uninstall section.
Section "Uninstall"
  ; remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Tuxtype2"
  DeleteRegKey HKLM SOFTWARE\NSIS_Tuxtype2
  ; remove files
  Delete $INSTDIR\tuxtype2\*.*
  ; MUST REMOVE UNINSTALLER, too
  Delete $INSTDIR\uninstall.exe
  ; remove shortcuts, if any.
  Delete "$SMPROGRAMS\Tuxtype2\*.*"
  Delete "$DESKTOP\Tuxtype2.lnk"
  ; remove directories used.
  RMDir /r "$SMPROGRAMS\Tuxtype2"
  RMDir /r "$PROGRAMFILES\Tuxtype2"
SectionEnd

; eof
