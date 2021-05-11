$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶️▶️▶️ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGreen "████ Run Insane.js - Test client"
Write-Host
& "./X-SetEnvVars.ps1"
$browser = "chrome"
$edge = "$(Select-ValueByPlatform "C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe" "TBD" "/Applications/Microsoft Edge.app/Contents/MacOS/Microsoft Edge")"
if($IsWindows)
{
    if(Test-Path "$edge" -PathType Leaf)
    {
        $browser = "$edge"
    }
}
if($IsMacOS)
{
    Write-Host $edge
    if(Test-Path "$edge" -PathType Leaf)
    {
        $browser = "$edge"
    }
}
$browserArgs = "-new-window -guest"

Write-InfoYellow "Press Ctrl+C to exit!"

& $env:EMSCRIPTEN_EMRUN "index.html" --browser $browser --browser_args $browserArgs --browser_info
Write-InfoDarkGreen "█ End run Insane.js - Test client"