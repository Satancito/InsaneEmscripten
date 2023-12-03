$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
try {
    Push-Location "$PSScriptRoot"
    $null = Test-Command "git submodule init" -ThrowOnFailure
    $null = Test-Command "git submodule update --remote --recursive " -ThrowOnFailure
}
finally {
    Pop-Location
}