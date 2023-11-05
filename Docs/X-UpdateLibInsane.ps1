Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
$REPO_DIR = "$PSScriptRoot/InsaneEmscripten"
$isRepo = Test-GitRepository $REPO_DIR

if ($isRepo) {
    Push-Location "$REPO_DIR"
    Test-Command "git fetch origin" -WriteOutput
    Test-Command "git reset --hard origin/main" -WriteOutput
    Pop-Location 
}
else {
    git clone "https://github.com/Satancito/InsaneEmscripten.git" "$REPO_DIR"
}

try {
    Push-Location "$PSScriptRoot/InsaneEmscripten"
    & "$PSScriptRoot/InsaneEmscripten/X-BuildLib.ps1" -Clean
}
catch {    
}
finally {
    Pop-Location
}

Remove-Item "$PSScriptRoot/Lib" -Force -Recurse -ErrorAction Ignore
Remove-Item "$PSScriptRoot/Include/Insane" -Force -Recurse -ErrorAction Ignore

New-Item "$PSScriptRoot/Lib" -ItemType Container -Force | Out-Null
New-Item "$PSScriptRoot/Include/Insane" -ItemType Container -Force | Out-Null

$SOURCE_LIB_CONTENT = "$PSScriptRoot/InsaneEmscripten/Dist/Insane-Emscripten-llvm-BitCode/Lib/*"
$DEST_LIB_DIR = "$PSScriptRoot/Lib"
$SOURCE_INSANE_INCLUDE_DIR_CONTENT = "$PSScriptRoot/InsaneEmscripten/Dist/Insane-Emscripten-llvm-BitCode/Include/Insane/*.h"
$DEST_INSANE_INCLUDE_DIR = "$PSScriptRoot/Include/Insane"
Copy-Item -Path "$SOURCE_LIB_CONTENT" -Destination "$DEST_LIB_DIR" -Force
Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR_CONTENT" -Destination "$DEST_INSANE_INCLUDE_DIR" -Force