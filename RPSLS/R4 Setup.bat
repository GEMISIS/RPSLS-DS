@echo off
set /p d= Where to place:
ren DSi_Homebrew_Browser.nds _BOOT.NDS
copy *.nds %d%
ren _BOOT.NDS DSi_Homebrew_Browser.nds
EcHO Copied to location!
pause