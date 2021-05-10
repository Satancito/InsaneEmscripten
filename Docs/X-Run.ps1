$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGreen "████ Run Insane.js - Test client"
Write-Host
& "./X-SetEnvVars.ps1"
$browser = "chrome" #"$(Select-ValueByPlatform "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe" "TBD" "TBD")"
$browserArgs = "-new-window -guest"

Write-InfoYellow "Close the browser first and press Ctrl+C to exit!"

& $env:EMSCRIPTEN_EMRUN "index.html" --browser $browser --browser_args $browserArgs --browser_info
Write-InfoDarkGreen "█ End run Insane.js - Test client"