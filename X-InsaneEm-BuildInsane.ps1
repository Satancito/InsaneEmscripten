[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Clean
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    Write-Host
    Write-InfoBlue "████ Building libInsane"
    Write-Host

    & "$X_INSANE_EM_TEST_REQUIRED_TOOLS_SCRIPT"
    & "$X_INSANE_EM_UPDATE_SUBMODULES_SCRIPT"

    $json = [System.IO.File]::ReadAllText($(Get-Item "$DOCS_PRODUCT_INFO_JSON"))
    $productInfo = ConvertFrom-Json $json
    $BotanVersion = $productInfo.BotanVersion
    $BotanMajorVersion = $productInfo.BotanMajorVersion

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
        & "$env:EMSCRIPTEN_EMMAKE" make All -j8 BOTAN_MAJOR_VERSION="$BotanMajorVersion" BOTAN_VERSION="$BotanVersion" BUILD_CONFIGURATION="Debug" BOTAN_LIB_DIR="$BOTAN_LIB_DIR" BOTAN_SUFFIX="$INSANE_EM_BOTAN_SUFFIX"
        & "$env:EMSCRIPTEN_EMMAKE" make All -j8 BOTAN_MAJOR_VERSION="$BotanMajorVersion" BOTAN_VERSION="$BotanVersion" BUILD_CONFIGURATION="Release" BOTAN_LIB_DIR="$BOTAN_LIB_DIR" BOTAN_SUFFIX="$INSANE_EM_BOTAN_SUFFIX"
    }
    finally {
        Pop-Location
    }

    Join-CompileCommandsJson -SourceDir "$OBJ_DIR_DEBUG" -DestinationDir "$PSScriptRoot"
    Join-CompileCommandsJson -SourceDir "$OBJ_DIR_RELEASE" -DestinationDir "$PSScriptRoot"

    Write-Host "Copying files..."
    Remove-Item "$INSANE_EM_INSTALL_DEBUG_DIR" -Force -Recurse -ErrorAction Ignore
    Remove-Item "$INSANE_EM_INSTALL_RELEASE_DIR" -Force -Recurse -ErrorAction Ignore
    $DEST_INSANE_DEBUG_INCLUDE_DIR = "$INSANE_EM_INCLUDE_DEBUG_DIR/Insane"
    $DEST_INSANE_RELEASE_INCLUDE_DIR = "$INSANE_EM_INCLUDE_RELEASE_DIR/Insane"
    $DEST_INSANE_DEBUG_LIB_DIR = "$INSANE_EM_LIB_DEBUG_DIR"
    $DEST_INSANE_RELEASE_LIB_DIR = "$INSANE_EM_LIB_RELEASE_DIR"
    

    New-Item "$DEST_INSANE_DEBUG_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_RELEASE_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_DEBUG_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_RELEASE_INCLUDE_DIR" -ItemType Container -Force | Out-Null

    $SOURCE_INSANE_INCLUDE_DIR = "$PSScriptRoot/Include/Insane" 
    $SOURCE_INSANECPP_INCLUDE_DIR = "$PSScriptRoot/submodules/InsaneCpp/Include/Insane"
    $SOURCE_INSANE_DEBUG_LIB = "$PSScriptRoot/Build/Obj/Debug/$INSANE_LIB_NAME"
    $SOURCE_INSANE_RELEASE_LIB = "$PSScriptRoot/Build/Obj/Release/$INSANE_LIB_NAME"

    #Copy
    Copy-Item -Path "$SOURCE_INSANE_DEBUG_LIB" -Destination "$DEST_INSANE_DEBUG_LIB_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANE_RELEASE_LIB" -Destination "$DEST_INSANE_RELEASE_LIB_DIR" -Force
    
    Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR/Insane*.h" -Destination "$DEST_INSANE_DEBUG_INCLUDE_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANECPP_INCLUDE_DIR/Insane*.h" -Destination "$DEST_INSANE_DEBUG_INCLUDE_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR/Insane*.h" -Destination "$DEST_INSANE_RELEASE_INCLUDE_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANECPP_INCLUDE_DIR/Insane*.h" -Destination "$DEST_INSANE_RELEASE_INCLUDE_DIR" -Force
}
finally {
    Write-InfoBlue "█ End - Building libInsane"
    Write-Host
}
