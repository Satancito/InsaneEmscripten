
param (
    [Parameter()]
    [switch]
    $Clean
    
    
)
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Write-Host
Write-InfoDarkGray "███ Setting environment variables"
Write-Host

$path = $env:EMSCRIPTEN_SDK
if ($Clean.IsPresent) {
    $path = [String]::Empty
}

$defaultPath = "$(Get-UserHome)/emsdk"   
while ([String]::IsNullOrWhiteSpace($path) -or (!(Test-Path $path -PathType Container))) {
    if (Test-Path $defaultPath -PathType Container) { 
        Write-InfoMagenta "Default Emscripten location detected: " -NoNewLine
        Write-InfoWhite "`"$defaultPath`". " -NoNewLine
        Write-InfoMagenta "Press enter to accept this value or type another valid location."
    }
    else {
        $defaultPath = [String]::Empty
    }
    do {
        Write-InfoGreen "Enter the Emscripten SDK's location: " -NoNewLine
        $path = [System.Console]::ReadLine();
        if ((![String]::IsNullOrWhiteSpace($defaultPath)) -and [String]::IsNullOrWhiteSpace($path)) {
            $path = $defaultPath
        }
        if (!(Test-Path "$path" -PathType Container))
        {
            Write-InfoYellow "Invalid location, directory `"$path`" not exists"
        }

    } until ((![String]::IsNullOrWhiteSpace($path)) -and (Test-Path $path -PathType Container) )
}

Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_SDK" -Value "$path"
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_ROOT" -Value "$env:EMSCRIPTEN_SDK/upstream/emscripten"
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_COMPILER" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "em++.bat" "em++" "em++")"
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMRUN" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emrun.bat" "emrun" "emrun")"
Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMMAKE" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emmake.bat" "emmake" "emmake")"

Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_SDK)`" set to the value `"$env:EMSCRIPTEN_SDK`""
Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_ROOT)`" set to the value `"$env:EMSCRIPTEN_ROOT`""
Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_COMPILER)`" set to the value `"$env:EMSCRIPTEN_COMPILER`""
Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_EMRUN)`" set to the value `"$env:EMSCRIPTEN_EMRUN`""
Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_EMMAKE)`" set to the value `"$env:EMSCRIPTEN_EMMAKE`""

Write-Host
Write-InfoDarkGray "███ End setting environment variables - Finished"