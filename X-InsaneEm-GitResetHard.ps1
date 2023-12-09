
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Reset this repository to latest code - git reset"
Write-Host

Reset-GitRepository -Path "$PSScriptRoot"

Write-InfoDarkGray "█ End - git reset"
Write-Host