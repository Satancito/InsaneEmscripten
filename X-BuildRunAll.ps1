[CmdletBinding()]
param (
    [string]
    $ModuleExportName = "Insane",

    [switch]
    $NoMinifyJsFiles 
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    & "./X-BuildLib.ps1"
    Push-Location "Dist/Insane*BitCode"
    & "./X-BuildModule.ps1" -ModuleExportName $ModuleExportName -NoMinifyJsFiles:$NoMinifyJsFiles
    & "./X-Run.ps1"
}
finally {
    Pop-Location
}