[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Clean
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$X_INSANE_EM_Z_PS_CORE_FXS_INTERNAL_SCRIPT")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    Write-Host
    Write-InfoBlue "████ Building libInsane"
    Write-Host
    Test-InsaneEmRequiredTools
    Update-GitSubmodules -Path $PSScriptRoot -Force

    $BOTAN_LIB_DIR = "$(Get-UserHome)/.CppLibs"

    $BUILD_DIR = "$PSScriptRoot/Build"
    $OBJ_DIR = "$BUILD_DIR/Obj"
    $OBJ_DIR_DEBUG = "$OBJ_DIR/Debug"
    $OBJ_DIR_RELEASE = "$OBJ_DIR/Release"


    if ($Clean.IsPresent) {
        Write-InfoYellow "Removing obj and dist files."
        Remove-Item "$BUILD_DIR" -Force -Recurse -ErrorAction Ignore
    }

    New-Item "$OBJ_DIR_DEBUG" -Force -ItemType Container | Out-Null
    New-Item "$OBJ_DIR_RELEASE" -Force -ItemType Container | Out-Null

    Write-Host "Building..."
    try {
        Push-Location "$PSScriptRoot"
        Write-Host $env:EMSCRIPTEN_EMMAKE
        & "$env:EMSCRIPTEN_EMMAKE" make All -j8 BOTAN_MAJOR_VERSION="$INSANE_EM_BOTAN_MAJOR_VERSION" BOTAN_VERSION="$INSANE_EM_BOTAN_VERSION" BUILD_CONFIGURATION="$INSANE_EM_BUILD_CONFIGURATION_DEBUG" BOTAN_LIB_DIR="$BOTAN_LIB_DIR" BOTAN_SUFFIX="$INSANE_EM_BOTAN_SUFFIX"
        & "$env:EMSCRIPTEN_EMMAKE" make All -j8 BOTAN_MAJOR_VERSION="$INSANE_EM_BOTAN_MAJOR_VERSION" BOTAN_VERSION="$INSANE_EM_BOTAN_VERSION" BUILD_CONFIGURATION="$INSANE_EM_BUILD_CONFIGURATION_RELEASE" BOTAN_LIB_DIR="$BOTAN_LIB_DIR" BOTAN_SUFFIX="$INSANE_EM_BOTAN_SUFFIX"
    }
    finally {
        Pop-Location
    }

    Write-Host "Creating compiler database(""compile_commands.json"")..."
    Join-CompileCommandsJson -SourceDir "$OBJ_DIR_DEBUG" -DestinationDir "$PSScriptRoot"
    Join-CompileCommandsJson -SourceDir "$OBJ_DIR_RELEASE" -DestinationDir "$PSScriptRoot"

    Write-Host "Removing old files..."
    Remove-Item "$INSANE_EM_INSANE_DIST_DEBUG_DIR" -Force -Recurse -ErrorAction Ignore
    Remove-Item "$INSANE_EM_INSANE_DIST_RELEASE_DIR" -Force -Recurse -ErrorAction Ignore
    Write-Host "Copying new files..."
    
    $DEST_INSANE_DEBUG_INCLUDE_DIR = "$(Get-InsaneDir -Include)"
    $DEST_INSANE_RELEASE_INCLUDE_DIR = "$(Get-InsaneDir -Include -ReleaseMode)"
    $DEST_INSANE_DEBUG_LIB_DIR = "$(Get-InsaneDir -Lib)"
    $DEST_INSANE_RELEASE_LIB_DIR = "$(Get-InsaneDir -Lib -ReleaseMode)"

    New-Item "$DEST_INSANE_DEBUG_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_RELEASE_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_DEBUG_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_RELEASE_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_DEBUG_INCLUDE_DIR/Insane" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_RELEASE_INCLUDE_DIR/Insane" -ItemType Container -Force | Out-Null

    $SOURCE_INSANE_INCLUDE_DIR = "$PSScriptRoot/Include" 
    $SOURCE_INSANECPP_INCLUDE_DIR = "$PSScriptRoot/submodules/InsaneCpp/Include"
    $SOURCE_INSANE_DEBUG_LIB = "$PSScriptRoot/Build/Obj/Debug/$INSANE_LIB_NAME"
    $SOURCE_INSANE_RELEASE_LIB = "$PSScriptRoot/Build/Obj/Release/$INSANE_LIB_NAME"

    #Copy
    Copy-Item -Path "$SOURCE_INSANE_DEBUG_LIB" -Destination "$DEST_INSANE_DEBUG_LIB_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANE_RELEASE_LIB" -Destination "$DEST_INSANE_RELEASE_LIB_DIR" -Force
    
    Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR/Insane/Insane*.h" -Destination "$DEST_INSANE_DEBUG_INCLUDE_DIR/Insane" -Force
    Copy-Item -Path "$SOURCE_INSANECPP_INCLUDE_DIR/Insane/Insane*.h" -Destination "$DEST_INSANE_DEBUG_INCLUDE_DIR/Insane" -Force 
    Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR/Insane/Insane*.h" -Destination "$DEST_INSANE_RELEASE_INCLUDE_DIR/Insane" -Force
    Copy-Item -Path "$SOURCE_INSANECPP_INCLUDE_DIR/Insane/Insane*.h" -Destination "$DEST_INSANE_RELEASE_INCLUDE_DIR/Insane" -Force
}
finally {
    Write-InfoBlue "█ End - Building libInsane"
    Write-Host
}
