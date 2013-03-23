@echo off
set /p d= Where to place:
ren DSi_Homebrew_Browser.nds boot.nds
copy *.nds %d%
ren boot.nds DSi_Homebrew_Browser.nds
EcHO Copied to location!
pause