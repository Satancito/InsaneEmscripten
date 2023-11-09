[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Clean,

    [Parameter()]
    [switch]
    $EnableClangd
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoBlue "████ Building Insane LLVM Bitcode"
Write-Host
$X_UPDATE_SUBMODULES_SCRIPT = "$PSScriptRoot/X-InsaneEmscripten-UpdateSubmodules.ps1"
$PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"
& "$X_UPDATE_SUBMODULES_SCRIPT"

$json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name

$BUILD_DIR = "$PSScriptRoot/Build"
$DIST_DIR = "$PSScriptRoot/Dist"

$DEST_DIR = "$DIST_DIR/$ModuleExportName"
$DEST_INCLUDE_DIR = "$DEST_DIR/Include/Insane"
$DEST_LIB_DIR = "$DEST_DIR/Lib"
$DEST_JS_DIR = "$DEST_DIR/Js"
$DEST_TOOLS_DIR = "$DEST_DIR/Tools"
$DEST_ASSETS_DIR = "$DEST_DIR/Assets"
$OBJ_DIR = "$BUILD_DIR/Obj"
$SET_ENV_VARS_SCRIPT = "$PSScriptRoot/X-InsaneEmscripten-SetEmscriptenEnvVars.ps1"
$X_INSTALL_EMSCRIPTEN_SCRIPT = "$PSScriptRoot/X-InsaneEmscripten-InstallEmscripten.ps1"
$COMPILE_COMMANDS_JSON = "$PSScriptRoot/compile_commands.json"

if ($Clean.IsPresent) {
    Write-InfoYellow "Removing obj and dist files."
    Remove-Item "$BUILD_DIR" -Force -Recurse -ErrorAction Ignore
    Remove-Item "$DIST_DIR" -Force -Recurse -ErrorAction Ignore
}

if (!(Test-Path "$OBJ_DIR" -PathType Container)) { 
    New-Item "$OBJ_DIR" -Force -ItemType Container | Out-Null
}

Write-Host "Building..."
try {
    Push-Location "$PSScriptRoot"
    & "$($env:EMSCRIPTEN_EMMAKE)" make -j8 CXX="$($env:EMSCRIPTEN_COMPILER)"
}
finally {
    Pop-Location
}


if ($EnableClangd.IsPresent) {
    $jsonFiles = Get-ChildItem "$OBJ_DIR/*.o.json"
    $encoding = [System.Text.Encoding]::UTF8
    [System.IO.File]::WriteAllText($COMPILE_COMMANDS_JSON, "[", $encoding);
    $jsonFiles | ForEach-Object {
        [System.IO.File]::AppendAllText($COMPILE_COMMANDS_JSON, [System.IO.File]::ReadAllText($_.FullName), $encoding)
    }
    [System.IO.File]::AppendAllText($COMPILE_COMMANDS_JSON, "]", $encoding);
}

Write-Host "Copying files..."
Remove-Item "$DIST_DIR" -Force -Recurse -ErrorAction Ignore

New-Item "$DEST_DIR" -ItemType Container -Force | Out-Null
Push-Location "$DEST_DIR"
Test-Command "git init" -WriteOutput
Pop-Location
New-Item "$DEST_INCLUDE_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_LIB_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_JS_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_TOOLS_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_ASSETS_DIR" -ItemType Container -Force | Out-Null

$LIB_NAME = "libInsane.a"
$SOURCE_INSANE_INCLUDE_DIR = "$PSScriptRoot/Include/Insane" 
$SOURCE_INSANECPP_INCLUDE_DIR = "$PSScriptRoot/modules/InsaneCpp/Include/Insane"
$SOURCE_JS_DIR = "$PSScriptRoot/Js"
$SOURCE_TOOLS_DIR = "$PSScriptRoot/Tools"
$SOURCE_DOCS_DIR = "$PSScriptRoot/Docs"
$SOURCE_ASSETS_DIR = "$PSScriptRoot/Assets"
$Z_PSCORE_SCRIPT = "$PSScriptRoot/Z-PsCoreFxs.ps1"

New-Item "$SOURCE_JS_DIR" -ItemType Container -Force | Out-Null
New-Item "$SOURCE_TOOLS_DIR" -ItemType Container -Force | Out-Null
New-Item "$SOURCE_DOCS_DIR" -ItemType Container -Force | Out-Null
New-Item "$SOURCE_ASSETS_DIR" -ItemType Container -Force | Out-Null
New-Item "$SOURCE_INSANE_INCLUDE_DIR" -ItemType Container -Force | Out-Null
New-Item "$SOURCE_INSANECPP_INCLUDE_DIR" -ItemType Container -Force | Out-Null

Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR/*Insane*.h" -Destination "$DEST_INCLUDE_DIR" -Force
Copy-Item -Path "$SOURCE_INSANECPP_INCLUDE_DIR/*Insane*.h" -Destination "$DEST_INCLUDE_DIR" -Force
Copy-Item -Path "$OBJ_DIR/$LIB_NAME" -Destination "$DEST_LIB_DIR" -Recurse -Force
Copy-Item -Path "$SOURCE_JS_DIR/*" -Destination "$DEST_JS_DIR" -Force
Copy-Item -Path "$SOURCE_TOOLS_DIR/*" -Destination "$DEST_TOOLS_DIR" -Force -Recurse
Copy-Item -Path "$SOURCE_DOCS_DIR/*" -Destination "$DEST_DIR" -Force -Recurse
Copy-Item -Path "$SOURCE_ASSETS_DIR/*" -Destination "$DEST_ASSETS_DIR" -Force -Recurse
Copy-Item -Path "$SET_ENV_VARS_SCRIPT " -Destination "$DEST_DIR" -Force -Recurse
Copy-Item -Path "$Z_PSCORE_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
Copy-Item -Path "$X_INSTALL_EMSCRIPTEN_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse


Write-InfoBlue "█ End - Building Insane LLVM Bitcode"
Write-Host
