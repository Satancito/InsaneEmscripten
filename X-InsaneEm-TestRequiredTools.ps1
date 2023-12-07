$ErrorActionPreference = "Stop"

Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoBlue "Test - InsaneEm - Dependency tools"
Write-Host

Write-InfoMagenta "== Python"
$command = Get-Command "python"
Write-Host "$($command.Source)"
& "$($command.Source)" --version
Write-Host

Write-InfoMagenta "== Git"
$command = Get-Command "git"
Write-Host "$($command.Source)"
& "$($command.Source)" --version
Write-Host

Write-InfoMagenta "== Make"
$command = Get-Command "make"
Write-Host "$($command.Source)"
& "$($command.Source)" --version
Write-Host

Write-InfoMagenta "== Java"
$command = Get-Command "java"
Write-Host "$($command.Source)"
& "$($command.Source)" --version
Write-Host

Write-InfoMagenta "== Node"
$command = Get-Command "node"
Write-Host "$($command.Source)"
& "$($command.Source)" --version
Write-Host

Write-InfoMagenta "== Deno"
$command = Get-Command "deno"
Write-Host "$($command.Source)"
& "$($command.Source)" --version
Write-Host

if ($IsWindows) {
    Write-InfoMagenta "== WSL"
    $command = Get-Command "wsl"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host
}