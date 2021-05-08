
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Write-Host
Write-InfoDarkGray "███ Setting environment variables"

Write-Host
$path = $env:EMSCRIPTEN_SDK
while([String]::IsNullOrWhiteSpace($path) -or (!(Test-Path $path -PathType Container)))
{
    Write-InfoGreen "Enter the Emscripten SDK's path: " -NoNewLine
    $path = [System.Console]::ReadLine();
}

Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_SDK" -Value "$path"
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_ROOT" -Value "$env:EMSCRIPTEN_SDK/upstream/emscripten"

Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_SDK)`" set to the value `"$env:EMSCRIPTEN_SDK`""
Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_ROOT)`" set to the value `"$env:EMSCRIPTEN_ROOT`""

Write-Host
Write-InfoDarkGray "███ End setting environment variables - Finished"