$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶️▶️▶️ Running: $PSCommandPath"

$json = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name
$ModuleVersion = $productInfo.Version

Write-Host
Write-InfoDarkGreen "████ Run - Module: ""$ModuleExportName.js"", Version: $ModuleVersion"
Write-Host
& "$PSScriptRoot/X-InsaneEmscripten-SetEmscriptenEnvVars.ps1"
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

& $env:EMSCRIPTEN_EMRUN "$PSScriptRoot/index.html" --browser $browser --browser_args $browserArgs --browser_info
Write-InfoDarkGreen "█ End run - Module - Test client"