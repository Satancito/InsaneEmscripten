
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Reset this repository to latest code - git reset"
Write-Host

git fetch origin
git reset --hard origin/main

Write-InfoDarkGray "█ End - git reset"
Write-Host