$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
$browser = "$(Select-ValueByPlatform "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe" "TBD" "TBD")"
$emrun = "$(Select-ValueByPlatform "emrun.bat" "emrun" "emrun")" 
& $emrun "index.html" --browser $browser --browser_args "-new-window -guest"