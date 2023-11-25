param (
    [Parameter()]
    [System.String]
    $InstallDir = [String]::Empty,

    [Parameter()]
    [switch]
    $Clean,

    [Parameter()]
    [switch]
    $Force
)
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoDarkGreen "████ Installing Emscripten SDK"
Write-Host
Write-Host

if ($Reset.IsPresent) {
    Write-InfoYellow "Setting new location ""$InstallDir"". Removing old sdk folder if exists ""$env:EMSCRIPTEN_SDK""."
    Remove-Item "$env:EMSCRIPTEN_SDK" -Force -Recurse -ErrorAction Ignore
}
& "$PSScriptRoot/X-InsaneEm-SetEmscriptenEnvVars.ps1" -InstallDir $InstallDir -Force:$Force -Clean:$Clean

$EMSDK = "https://github.com/emscripten-core/emsdk.git"
$EMSDK_EXE = Select-ValueByPlatform "emsdk.bat" "emsdk" "emsdk"

if ($Clean.IsPresent) {
    Write-InfoYellow "Removing SDK folder if exists! ""$env:EMSCRIPTEN_SDK""."
    Remove-Item "$env:EMSCRIPTEN_SDK" -Force -Recurse -ErrorAction Ignore
}

if(!(Test-GitRepository $env:EMSCRIPTEN_SDK))
{
    Remove-Item -Force -Recurse -Path "$env:EMSCRIPTEN_SDK" -ErrorAction Ignore
    New-Item "$env:EMSCRIPTEN_SDK" -ItemType Container -Force | Out-Null
    git clone "$EMSDK" "$env:EMSCRIPTEN_SDK"; 
}

git -C "$env:EMSCRIPTEN_SDK" pull

if ($IsLinux -or $IsMacOS) {
    chmod +x "$env:EMSCRIPTEN_SDK/$EMSDK_EXE"
}

Write-InfoMagenta "Installing SDK..."
& "$env:EMSCRIPTEN_SDK/$EMSDK_EXE" install latest 
Write-InfoMagenta "Activating SDK..."
& "$env:EMSCRIPTEN_SDK/$EMSDK_EXE" activate latest 

Write-Host
Write-InfoDarkGreen "█ End - Installing Emscripten SDK"