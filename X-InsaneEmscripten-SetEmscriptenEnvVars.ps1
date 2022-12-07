
param (
    [Parameter()]
    [System.String]
    $InstallDir = [String]::Empty 
)
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Setting environment variables"
Write-Host

$path = [string]::IsNullOrWhiteSpace($InstallDir) ? 
        ([string]::IsNullOrWhiteSpace($env:EMSCRIPTEN_SDK) ? [string]::Empty : ($env:EMSCRIPTEN_SDK | Split-Path)) : $InstallDir

if (Test-Path "$path" -PathType Leaf) {
    $path = [string]::Empty
}

$defaultPath = "$(Get-UserHome)" 
if ([String ]::IsNullOrWhiteSpace($path)) {
    if (Test-Path $defaultPath -PathType Container) { 
        Write-InfoMagenta "Default Emscripten location is: " -NoNewLine
        Write-InfoWhite "`"$defaultPath`". " -NoNewLine
        Write-InfoMagenta "Press enter to accept this value or type another valid location."
    }
    do {
        Write-InfoGreen "Enter the path where the Emscripten SDK directory is located: " -NoNewLine
        $path = [System.Console]::ReadLine();
        $path = [String]::IsNullOrWhiteSpace($path) ? $defaultPath : $path
        if (Test-Path "$path" -PathType Leaf) {
            Write-InfoYellow "Invalid location, `"$path`" can be a file."
            $path = [string]::Empty
        }
    } until (![String]::IsNullOrWhiteSpace($path))
}

Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_SDK" -Value "$path/emsdk" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_ROOT" -Value "$env:EMSCRIPTEN_SDK/upstream/emscripten" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_COMPILER" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "em++.bat" "em++" "em++")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMRUN" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emrun.bat" "emrun" "emrun")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMMAKE" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emmake.bat" "emmake" "emmake")" -Verbose

Write-Host
Write-InfoDarkGray "███ End setting environment variables - Finished"