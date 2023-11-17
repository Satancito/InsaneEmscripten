$ErrorActionPreference = "Stop"

Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    Write-Host
    Write-InfoBlue "███ Test - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Python"
    $null = Test-Command "python --version" -ThrowOnFailure
    Write-Host

    Write-InfoMagenta "== Git"
    $null = Test-Command "git --version" -ThrowOnFailure
    Write-Host

    Write-InfoMagenta "== Make"
    $null = Test-Command "make --version" -ThrowOnFailure
    Write-Host

    Write-InfoMagenta "== Java"
    $null = Test-Command "java --version" -ThrowOnFailure
    Write-Host

    Write-InfoMagenta "== Node"
    $null = Test-Command "node --version" -ThrowOnFailure
    Write-Host

    Write-InfoMagenta "== Deno"
    $null = Test-Command "deno --version" -ThrowOnFailure
    Write-Host
}
finally {
    Write-InfoBlue "█ End - Test - Dependency Tools"
    Write-Host
}


