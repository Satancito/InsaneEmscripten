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
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoBlue "████ Building Insane LLVM Bitcode"
Write-Host

$DEST_DIR = "Dist/Insane-Emscripten-llvm-BitCode"
$DEST_INCLUDE_DIR = "$DEST_DIR/Include/Insane"
$DEST_LIB_DIR = "$DEST_DIR/Lib"
$DEST_JS_DIR = "$DEST_DIR/Js"
$DEST_TOOLS_DIR = "$DEST_DIR/Tools"
$DEST_ASSETS_DIR = "$DEST_DIR/Assets"
$OBJ_DIR = "Build/Obj"
$SET_ENV_VARS_SCRIPT = "./X-InsaneEmscripten-SetEmscriptenEnvVars.ps1"
$X_INSTALL_EMSCRIPTEN_SCRIPT = "./X-InsaneEmscripten-InstallEmscripten.ps1"

if ($Clean.IsPresent) {
    Write-InfoYellow "Removing obj and dist files."
    Remove-Item ./Build -Force -Recurse -ErrorAction Ignore
    Remove-Item ./Dist -Force -Recurse -ErrorAction Ignore
    Remove-Item ./build -Force -Recurse -ErrorAction Ignore
    Remove-Item ./dist -Force -Recurse -ErrorAction Ignore
}

if (!(Test-Path "$OBJ_DIR" -PathType Container)) { 
    New-Item "$OBJ_DIR" -Force -ItemType Container | Out-Null
}

Write-Host "Building..."

& "$($env:EMSCRIPTEN_EMMAKE)" make -j8 CXX="$($env:EMSCRIPTEN_COMPILER)"
Test-LastExitCode

if ($EnableClangd.IsPresent) {
    $jsonFiles = Get-ChildItem "Build/Obj/*.o.json"
    $compileCommandsJson = "compile_commands.json"
    $encoding = [System.Text.Encoding]::UTF8
    [System.IO.File]::WriteAllText($compileCommandsJson, "[", $encoding);
    $jsonFiles | ForEach-Object {
        [System.IO.File]::AppendAllText($compileCommandsJson, [System.IO.File]::ReadAllText($_.FullName), $encoding)
    }
    [System.IO.File]::AppendAllText($compileCommandsJson, "]", $encoding);
}

Write-Host "Copying files..."
Remove-Item "Dist" -Force -Recurse -ErrorAction Ignore

New-Item "$DEST_INCLUDE_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_LIB_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_JS_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_TOOLS_DIR" -ItemType Container -Force | Out-Null
New-Item "$DEST_ASSETS_DIR" -ItemType Container -Force | Out-Null

$LIB_NAME = "libInsane.a"
$SOURCE_INSANE_INCLUDE_DIR_CONTENT = "Include/Insane/*" 
$SOURCE_INSANECPP_INCLUDE_DIR_CONTENT = "modules/InsaneCpp/Include/Insane/Insane*.h"
$SOURCE_JS_DIR_CONTENT = "Js/*"
$SOURCE_TOOLS_DIR_CONTENT = "Tools/*"
$SOURCE_DOCS_DIR_CONTENT = "Docs/*"
$SOURCE_ASSETS_DIR_CONTENT = "Assets/*"
$Z_PSCORE_SCRIPT = "./Z-PsCoreFxs.ps1"
Copy-Item -Path $SOURCE_INSANE_INCLUDE_DIR_CONTENT -Destination "$DEST_INCLUDE_DIR" -Force
Copy-Item -Path $SOURCE_INSANECPP_INCLUDE_DIR_CONTENT -Destination "$DEST_INCLUDE_DIR" -Force
Copy-Item -Path "$OBJ_DIR/$LIB_NAME" -Destination "$DEST_LIB_DIR" -Recurse -Force
Copy-Item -Path "$SOURCE_JS_DIR_CONTENT" -Destination "$DEST_JS_DIR" -Force
Copy-Item -Path "$SOURCE_TOOLS_DIR_CONTENT" -Destination "$DEST_TOOLS_DIR" -Force -Recurse
Copy-Item -Path "$SOURCE_DOCS_DIR_CONTENT" -Destination "$DEST_DIR" -Force -Recurse
Copy-Item -Path "$SOURCE_ASSETS_DIR_CONTENT" -Destination "$DEST_ASSETS_DIR" -Force -Recurse
Copy-Item -Path "$SET_ENV_VARS_SCRIPT " -Destination "$DEST_DIR" -Force -Recurse
Copy-Item -Path "$Z_PSCORE_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
Copy-Item -Path "$X_INSTALL_EMSCRIPTEN_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse


Write-InfoBlue "█ End - Building Insane LLVM Bitcode"
Write-Host
