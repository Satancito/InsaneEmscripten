$ErrorActionPreference = "Continue"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Test - Dependency tools"
Write-Host

Write-InfoMagenta "Git"
Test-Command "git --version" -WriteOutput
Write-Host

Write-InfoMagenta "Make"
Test-Command "make --version" -WriteOutput
Write-Host

Write-InfoMagenta "Java"
Test-Command "java --version" -WriteOutput
Write-Host

Write-InfoDarkGray "█ End - Test - Dependency Tools"
Write-Host