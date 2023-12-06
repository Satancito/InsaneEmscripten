[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $BuildBotan
)
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber

$INSANE_EMSCRIPTEN_REPO_DIR = "$INSANE_EM_DEFAULT_TEMP_DIR/InsaneEmscripten"

Install-GitRepository -Url "$INSANE_EM_REPO_URL" -Path "$INSANE_EMSCRIPTEN_REPO_DIR"

try {
    Push-Location "$INSANE_EMSCRIPTEN_REPO_DIR"
    if ($BuildBotan.IsPresent) {
        & "$INSANE_EMSCRIPTEN_REPO_DIR/X-InsaneEm-BuildBotan.ps1"
    }
    & "$INSANE_EMSCRIPTEN_REPO_DIR/X-InsaneEm-BuildInsane.ps1" -Clean
}
finally {
    Pop-Location
}
$jsonDocs = [System.IO.File]::ReadAllText($(Get-Item "$INSANE_EMSCRIPTEN_REPO_DIR/Docs/ProductInfo.json"))
$productInfo = ConvertFrom-Json $jsonDocs


$jsonLocal = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfoLocal = ConvertFrom-Json $jsonLocal
$productInfoLocal.BotanVersion = $productInfo.BotanVersion
$productInfoLocal.BotanMajorVersion = $productInfo.BotanMajorVersion
$jsonStr = (ConvertTo-Json $productInfoLocal)
[System.IO.File]::WriteAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"), $jsonStr)