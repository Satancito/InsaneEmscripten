param (
    [Parameter()]
    [System.String]
    $InstallDir = [String]::Empty ,

    [Parameter()]
    [switch]
    $Clean   
)
    
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Write-Host
Write-InfoDarkGreen "████ Installing Emscripten"
Write-Host

$EMSDK = "https://github.com/emscripten-core/emsdk.git"
if([String]::IsNullOrWhiteSpace($InstallDir))
{
    $InstallDir = "$(Get-UserHome)"
}

try {
    New-Item $InstallDir -ItemType Container -Force | Out-Null
    Push-Location $InstallDir
    if ($Clean.IsPresent) {
        Remove-Item "./emsdk" -Force -Recurse -ErrorAction Ignore
        Write-InfoYellow "Removing SDK folder!. Try run this file again without `"Clean`" parameter."
        exit
    }
    if (!(Test-Path "./emsdk" -PathType Container)) {
        git clone "$EMSDK"; Test-LastExitCode
    }
    Push-Location "./emsdk"
    

    git pull; Test-LastExitCode

    $EMSDK_EXE = Select-ValueByPlatform "emsdk.bat" "emsdk" "emsdk"
    $EMSDK_ENV = Select-ValueByPlatform "emsdk_env.bat" "emsdk_env.sh" "emsdk_env.sh"

    & "./$EMSDK_EXE" install latest; Test-LastExitCode
    & "./$EMSDK_EXE" activate latest; Test-LastExitCode
    if($IsLinux -or $IsMacOS)
    {
        chmod +x "./$EMSDK_ENV"
        & sh "./$EMSDK_ENV"; Test-LastExitCode
    }
    else {
        & "./$EMSDK_ENV"; Test-LastExitCode    
    }
    

    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_SDK" -Value "$InstallDir/emsdk"
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_ROOT" -Value "$env:EMSCRIPTEN_SDK/upstream/emscripten"
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_COMPILER" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "em++.bat" "em++" "em++")"
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMRUN" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emrun.bat" "emrun" "emrun")"
    Set-PersistentEnvironmentVariable -Name "EMSCRIPTEN_EMMAKE" -Value "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emmake.bat" "emmake" "emmake")"

    Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_SDK)`" set to the value `"$env:EMSCRIPTEN_SDK`""
    Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_ROOT)`" set to the value `"$env:EMSCRIPTEN_ROOT`""
    Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_COMPILER)`" set to the value `"$env:EMSCRIPTEN_COMPILER`""
    Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_EMRUN)`" set to the value `"$env:EMSCRIPTEN_EMRUN`""
    Write-Host "Environment variable `"$(Get-VariableName -Variable $env:EMSCRIPTEN_EMMAKE)`" set to the value `"$env:EMSCRIPTEN_EMMAKE`""
}
finally {
    Pop-Location
    Pop-Location
}

Write-Host
Write-InfoDarkGreen "█ End install Emscripten"