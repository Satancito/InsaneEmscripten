Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
$INSANE_EMSCRIPTEN_REPO_URL = "https://github.com/Satancito/InsaneEmscripten.git"
$INSANE_EMSCRIPTEN_REPO_DIR = "$(Get-UserHome)/.InsaneEmscripten/InsaneEmscripten"

$PS_BOTAN_REPO_URL = "https://github.com/Satancito/PsBotan.git"
$PS_BOTAN_REPO_DIR = "$(Get-UserHome)/.InsaneEmscripten/PsBotan"

Install-GitRepository -Url "$INSANE_EMSCRIPTEN_REPO_URL" -Path "$INSANE_EMSCRIPTEN_REPO_DIR"
Install-GitRepository -Url "$PS_BOTAN_REPO_URL" -Path "$PS_BOTAN_REPO_DIR"

try {
    Push-Location "$INSANE_EMSCRIPTEN_REPO_DIR"
    & "$INSANE_EMSCRIPTEN_REPO_DIR/X-InsaneEm-BuildBotan.ps1"
    & "$INSANE_EMSCRIPTEN_REPO_DIR/X-InsaneEm-BuildInsane.ps1" -Clean
}
finally {
    Pop-Location
}
$json = [System.IO.File]::ReadAllText($(Get-Item "$INSANE_EMSCRIPTEN_REPO_DIR/Docs/ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name

$SOURCE_LIB_CONTENT = "$INSANE_EMSCRIPTEN_REPO_DIR/Dist/$ModuleExportName/Lib/*"
$DEST_LIB_DIR = "$PSScriptRoot/Lib"
$SOURCE_INSANE_INCLUDE_DIR_CONTENT = "$INSANE_EMSCRIPTEN_REPO_DIR/Dist/$ModuleExportName/Include/Insane/*.h"
$DEST_INSANE_INCLUDE_DIR = "$PSScriptRoot/Include/Insane"

Remove-Item "$DEST_LIB_DIR/libInsane.a" -Force -Recurse -ErrorAction Ignore
Remove-Item "$DEST_INSANE_INCLUDE_DIR" -Force -Recurse -ErrorAction Ignore

New-Item "$PSScriptRoot/Lib" -ItemType Container -Force | Out-Null
New-Item "$PSScriptRoot/Include/Insane" -ItemType Container -Force | Out-Null

Copy-Item -Path "$SOURCE_LIB_CONTENT" -Destination "$DEST_LIB_DIR" -Force
Copy-Item -Path "$SOURCE_INSANE_INCLUDE_DIR_CONTENT" -Destination "$DEST_INSANE_INCLUDE_DIR" -Force

$jsonLocal = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfoLocal = ConvertFrom-Json $jsonLocal
$productInfoLocal.BotanVersion = $BotanVersion
$jsonStr = (ConvertTo-Json $productInfoLocal)
[System.IO.File]::WriteAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"), $jsonStr)