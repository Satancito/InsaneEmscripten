[CmdletBinding()]
param (
    [switch]
    $NoMinifyJsFiles 

)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    & "$PSScriptRoot/X-InsaneEm-BuildModule.ps1" -NoMinifyJsFiles:$NoMinifyJsFiles
    & "$PSScriptRoot/X-InsaneEm-RunModule.ps1"
}
finally {
    Pop-Location
}