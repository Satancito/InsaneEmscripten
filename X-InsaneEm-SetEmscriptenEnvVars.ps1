
param (
    [Parameter()]
    [System.String]
    $InstallDir = [String]::Empty ,

    [Parameter()]
    [switch]
    $Force,

    [Parameter()]
    [switch]
    $Clean
)
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGray "███ Setting environment variables for Emscripten SDK"
Write-Host

if ($Clean.IsPresent) {
    Write-InfoMagenta "Cleaning environment variables."
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_SDK" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_ROOT" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_COMPILER" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMCC" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMRUN" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMMAKE" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMAR" -Value "" -Verbose
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMCONFIGURE" -Value "" -Verbose 
}

$path = [string]::IsNullOrWhiteSpace($InstallDir) ? 
        ([string]::IsNullOrWhiteSpace($env:EMSCRIPTEN_SDK) ? [string]::Empty : ($env:EMSCRIPTEN_SDK | Split-Path)) : $InstallDir

if (Test-Path "$path" -PathType Leaf) {
    $path = [string]::Empty
}

$defaultPath = "$(Get-UserHome)" 
if ([String ]::IsNullOrWhiteSpace($path)) {
    if ($Force.IsPresent) {
        $path = $defaultPath
        Write-PrettyKeyValue "Emscripten SDK directory will be located on:" "$path/emsdk"
    }
    else {
        if (Test-Path $defaultPath -PathType Container) { 
            Write-InfoMagenta "Default Emscripten location is: " -NoNewLine
            Write-InfoWhite "`"$defaultPath`". " -NoNewLine
            Write-InfoMagenta "Press enter to accept this value or type another valid location."
        }
        do {
            Write-InfoGreen "Enter the path where the Emscripten SDK directory will be located: " -NoNewLine
            $path = [System.Console]::ReadLine();
            $path = [String]::IsNullOrWhiteSpace($path) ? $defaultPath : $path
            if (Test-Path "$path" -PathType Leaf) {
                Write-InfoYellow "Invalid location, `"$path`" can be a file."
                $path = [string]::Empty
            }
        } until (![String]::IsNullOrWhiteSpace($path))
    }
}

Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_SDK" -Value "$path/emsdk" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_ROOT" -Value "$env:EMSCRIPTEN_SDK/upstream/emscripten" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_COMPILER" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "em++.bat" "em++" "em++")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMCC" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emcc.bat" "emcc" "emcc")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMRUN" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emrun.bat" "emrun" "emrun")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMMAKE" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emmake.bat" "emmake" "emmake")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMAR" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emar.bat" "emar" "emar")" -Verbose
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMCONFIGURE" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emconfigure.bat" "emconfigure" "emconfigure")" -Verbose

Write-Host
Write-InfoDarkGray "███ End setting environment variables - Finished"