$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Test - Dependency Tools"
Write-Host

Write-InfoMagenta "git --version"
git --version; Test-LastExitCode
Write-Host

Write-InfoMagenta "make --version"
make --version; Test-LastExitCode
Write-Host

Write-InfoMagenta "java --version"
java --version; Test-LastExitCode
Write-Host

Write-InfoDarkGray "█ End - Test - Dependency Tools"
Write-Host