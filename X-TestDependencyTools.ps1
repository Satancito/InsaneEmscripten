$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Test - Dependency Tools"
Write-Host

git --version; Test-LastExitCode
make --version; Test-LastExitCode
java --version; Test-LastExitCode

Write-InfoDarkGray "█ End - Test - Dependency Tools"
Write-Host