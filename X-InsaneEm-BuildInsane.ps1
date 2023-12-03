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

function Join-CompileCommandsJson {
    param (
        [Parameter(Mandatory = $true)]
        [string]
        $SourceDir,
        
        [Parameter(Mandatory = $true)]
        [string]
        $DestinationDir
    )
    $jsonFiles = Get-ChildItem "$SourceDir/*.compile_commands.json"  
    $encoding = [System.Text.Encoding]::UTF8 
    $COMPILE_COMMANDS_JSON = "$DestinationDir/compile_commands.json"
    [System.IO.File]::WriteAllText($COMPILE_COMMANDS_JSON, "[", $encoding);
    $jsonFiles | ForEach-Object {
        [System.IO.File]::AppendAllText($COMPILE_COMMANDS_JSON, [System.IO.File]::ReadAllText($_.FullName), $encoding)
    }
    [System.IO.File]::AppendAllText($COMPILE_COMMANDS_JSON, "]", $encoding)
}

try {
    Write-Host
    Write-InfoBlue "████ Building libInsane"
    Write-Host

    & "$X_INSANE_EM_TEST_REQUIRED_TOOLS_SCRIPT"
    $PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"
    & "$X_INSANE_EM_UPDATE_SUBMODULES_SCRIPT"

    $json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON"))
    $productInfo = ConvertFrom-Json $json
    $ModuleExportName = $productInfo.Name
    $BotanVersion = $productInfo.BotanVersion
    $BotanMajorVersion = $productInfo.BotanMajorVersion

    $BOTAN_LIB_DIR = "$(Get-UserHome)/.InsaneEmscripten/PsBotan/Dist"

    $BUILD_DIR = "$PSScriptRoot/Build"
    $DIST_DIR = "$PSScriptRoot/Dist"
    $OBJ_DIR = "$BUILD_DIR/Obj"
    $OBJ_DIR_DEBUG = "$OBJ_DIR/Debug"
    $OBJ_DIR_RELEASE = "$OBJ_DIR/Release"


    if ($Clean.IsPresent) {
        Write-InfoYellow "Removing obj and dist files."
        Remove-Item "$BUILD_DIR" -Force -Recurse -ErrorAction Ignore
        Remove-Item "$DIST_DIR" -Force -Recurse -ErrorAction Ignore
    }

    New-Item "$OBJ_DIR_DEBUG" -Force -ItemType Container | Out-Null
    New-Item "$OBJ_DIR_RELEASE" -Force -ItemType Container | Out-Null

    Write-Host "Building..."
    try {
        Push-Location "$PSScriptRoot"
        Write-Host $env:EMSCRIPTEN_EMMAKE
        & "$env:EMSCRIPTEN_EMMAKE" make All -j8 BOTAN_MAJOR_VERSION="$BotanMajorVersion" BOTAN_VERSION="$BotanVersion" BUILD_CONFIGURATION="Debug" BOTAN_LIB_DIR="$BOTAN_LIB_DIR"
        & "$env:EMSCRIPTEN_EMMAKE" make All -j8 BOTAN_MAJOR_VERSION="$BotanMajorVersion" BOTAN_VERSION="$BotanVersion" BUILD_CONFIGURATION="Release" BOTAN_LIB_DIR="$BOTAN_LIB_DIR"
    }
    finally {
        Pop-Location
    }

    Join-CompileCommandsJson -SourceDir "$OBJ_DIR_DEBUG" -DestinationDir "$PSScriptRoot"
    Join-CompileCommandsJson -SourceDir "$OBJ_DIR_RELEASE" -DestinationDir "$PSScriptRoot"

    Write-Host "Copying files..."

    $DEST_DIR = "$DIST_DIR/$ModuleExportName"
    $DEST_INCLUDE_DIR = "$DEST_DIR/Include"
    $DEST_INSANE_INCLUDE_DIR = "$DEST_DIR/Include/Insane"
    $DEST_LIB_DIR = "$DEST_DIR/Lib"
    $DEST_INSANE_LIB_DIR = "$DEST_DIR/Lib/Insane"
    $DEST_INSANE_DEBUG_LIB_DIR = "$DEST_DIR/Lib/Insane/Debug"
    $DEST_INSANE_RELEASE_LIB_DIR = "$DEST_DIR/Lib/Insane/Release"
    $DEST_JS_DIR = "$DEST_DIR/Js"
    $DEST_TOOLS_DIR = "$DEST_DIR/Tools"
    $DEST_ASSETS_DIR = "$DEST_DIR/Assets"
    $DEST_SERVER_DIR = "$DEST_DIR/Server"

    Remove-Item "$DIST_DIR" -Force -Recurse -ErrorAction Ignore
    New-Item "$DEST_DIR" -ItemType Container -Force | Out-Null

    try {
        Push-Location "$DEST_DIR"
        $null = Test-Command "git init" -ThrowOnFailure
    }
    finally {
        Pop-Location
    }

    New-Item "$DEST_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_DEBUG_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_INSANE_RELEASE_LIB_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_JS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_TOOLS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_ASSETS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$DEST_SERVER_DIR" -ItemType Container -Force | Out-Null

    $INSANE_LIB_NAME = "libInsane.a"
    $SOURCE_INSANE_INCLUDE_DIR = "$PSScriptRoot/Include/Insane" 
    $SOURCE_INSANECPP_INCLUDE_DIR = "$PSScriptRoot/modules/InsaneCpp/Include/Insane"
    $SOURCE_JS_DIR = "$PSScriptRoot/Js"
    $SOURCE_TOOLS_DIR = "$PSScriptRoot/Tools"
    $SOURCE_DOCS_DIR = "$PSScriptRoot/Docs"
    $SOURCE_ASSETS_DIR = "$PSScriptRoot/Assets"
    $SOURCE_SERVER_DIR = "$PSScriptRoot/Server"
    $SOURCE_INSANE_DEBUG_LIB = "$PSScriptRoot/Build/Obj/Debug/$INSANE_LIB_NAME"
    $SOURCE_INSANE_RELEASE_LIB = "$PSScriptRoot/Build/Obj/Release/$INSANE_LIB_NAME"


    New-Item "$SOURCE_INSANE_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_INSANECPP_INCLUDE_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_JS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_TOOLS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_DOCS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_ASSETS_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_SERVER_DIR" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_INSANE_DEBUG_LIB" -ItemType Container -Force | Out-Null
    New-Item "$SOURCE_INSANE_RELEASE_LIB" -ItemType Container -Force | Out-Null

    Copy-Item -Path "$SOURCE_INSANE_DEBUG_LIB" -Destination "$DEST_INSANE_DEBUG_LIB_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANE_RELEASE_LIB" -Destination "$DEST_INSANE_RELEASE_LIB_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR/Insane*.h" -Destination "$DEST_INSANE_INCLUDE_DIR" -Force
    Copy-Item -Path "$SOURCE_INSANECPP_INCLUDE_DIR/Insane*.h" -Destination "$DEST_INSANE_INCLUDE_DIR" -Force
    Copy-Item -Path "$SOURCE_JS_DIR/*" -Destination "$DEST_JS_DIR" -Force
    Copy-Item -Path "$SOURCE_TOOLS_DIR/*" -Destination "$DEST_TOOLS_DIR" -Force -Recurse
    Copy-Item -Path "$SOURCE_DOCS_DIR/*" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$SOURCE_ASSETS_DIR/*" -Destination "$DEST_ASSETS_DIR" -Force -Recurse
    Copy-Item -Path "$SOURCE_SERVER_DIR/*" -Destination "$DEST_SERVER_DIR" -Force -Recurse

    Copy-Item -Path "$X_INSANE_EM_SET_EMSCRIPTEN_ENV_VARS_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$X_INSANE_EM_INSTALL_EMSCRIPTEN_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$Z_INSANE_EM_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse

    Copy-Item -Path "$X_PSCORE_FXS_UPDATE_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$Z_PSCORE_FXS_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
}
finally {
    Write-InfoBlue "█ End - Building libInsane"
    Write-Host
}


$X_INSANE_EM_TEST_REQUIRED_TOOLS_SCRIPT = "$PSScriptRoot/X-InsaneEm-TestRequiredTools.ps1"
$X_INSANE_EM_UPDATE_SUBMODULES_SCRIPT = "$PSScriptRoot/X-InsaneEm-UpdateSubmodules.ps1"
$X_INSANE_EM_INSTALL_EMSCRIPTEN_SCRIPT = "$PSScriptRoot/X-InsaneEm-InstallEmscripten.ps1"
$X_INSANE_EM_SET_EMSCRIPTEN_ENV_VARS_SCRIPT = "$PSScriptRoot/X-InsaneEm-SetEmscriptenEnvVars.ps1"
$X_PSCORE_FXS_UPDATE_SCRIPT = "$PSScriptRoot/X-PsCoreFxs-Update.ps1"
$Z_INSANE_EM_SCRIPT = "$PSScriptRoot/Z-InsaneEm.ps1"
$Z_PSCORE_FXS_SCRIPT = "$PSScriptRoot/Z-PsCoreFxs.ps1"