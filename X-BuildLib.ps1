[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Clean
)

$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Clear-Host
Write-Host
Write-InfoBlue "████ Building Insane LLVM Bitcode"
Write-Host

& "./X-CloneDependencies"
if($Clean.IsPresent)
{
    Write-InfoYellow "Removing obj and dist files. Please recompile again the library without the ""Clean"" parameter!!!."
    Remove-Item ./Build -Force -Recurse -ErrorAction Ignore
    Remove-Item ./Dist -Force -Recurse -ErrorAction Ignore
    Remove-Item ./build -Force -Recurse -ErrorAction Ignore
    Remove-Item ./dist -Force -Recurse -ErrorAction Ignore
    Write-Host "█ End Compiling Insane LLVM Bitcode - Finished" -ForegroundColor Blue
    Write-Host
    exit
}

$DIST_DIR= "Dist/Insane-Emscripten-llvm-BitCode"
$INCLUDE_DIR = "$DIST_DIR/Include/Insane"
$LIB_DIR = "$DIST_DIR/Lib"
$JS_DIR = "$DIST_DIR/Js"
$TOOLS_DIR = "$DIST_DIR/Tools"
$ASSETS_DIR = "$DIST_DIR/Assets"

$OBJ_DIR = "Build/Obj"

if(!(Test-Path "$OBJ_DIR" -PathType Container))
{ 
    New-Item "$OBJ_DIR" -Force -ItemType Container | Out-Null
}
& "./X-SetEnvVars.ps1"
Test-LastExitCode

$compiler = "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "em++.bat" "em++" "em++")"
$make = "$env:EMSCRIPTEN_ROOT/$(Select-ValueByPlatform "emmake.bat" "emmake" "emmake")"

Write-Host "Compiling..."
& "$make" make -j8 CXX="$compiler"

Test-LastExitCode

Write-Host "Copying files..."
Remove-Item "Dist" -Force -Recurse -ErrorAction Ignore

New-Item "$INCLUDE_DIR" -ItemType Container -Force | Out-Null
New-Item "$LIB_DIR" -ItemType Container -Force | Out-Null
New-Item "$JS_DIR" -ItemType Container -Force | Out-Null
New-Item "$TOOLS_DIR" -ItemType Container -Force | Out-Null
New-Item "$ASSETS_DIR" -ItemType Container -Force | Out-Null

Copy-Item -Path "Include/Insane/*" -Destination "$INCLUDE_DIR" -Force
Copy-Item -Path "../InsaneCpp/Include/Insane/*" -Destination "$INCLUDE_DIR" -Force
Copy-Item -Path "$OBJ_DIR/libInsane.bc" -Destination "$LIB_DIR" -Recurse -Force
Copy-Item -Path "Js/*" -Destination "$JS_DIR" -Force
Copy-Item -Path "Tools/*" -Destination "$TOOLS_DIR" -Force -Recurse
Copy-Item -Path "Docs/*" -Destination "$DIST_DIR" -Force -Recurse
Copy-Item -Path "./X-SetEnvVars.ps1" -Destination "$DIST_DIR" -Force -Recurse
Copy-Item -Path "./Z-CoreFxs*.ps1" -Destination "$DIST_DIR" -Force -Recurse
Copy-Item -Path "./Z-CoreValues*.ps1" -Destination "$DIST_DIR" -Force -Recurse
Copy-Item -Path "Assets/*" -Destination "$ASSETS_DIR" -Force -Recurse


Write-InfoBlue "█ End building Insane LLVM Bitcode - Finished"
Write-Host
