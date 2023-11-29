$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoBlue "████ Building libBotan"
Write-Host
$X_UPDATE_SUBMODULES_SCRIPT = "$PSScriptRoot/X-InsaneEm-UpdateSubmodules.ps1"
& "$X_UPDATE_SUBMODULES_SCRIPT"

$PS_BOTAN_REPO_URL = "https://github.com/Satancito/PsBotan.git"
$PS_BOTAN_REPO_DIR = "$(Get-UserHome)/.InsaneEmscripten/PsBotan"

Install-GitRepository -Url "$PS_BOTAN_REPO_URL" -Path "$PS_BOTAN_REPO_DIR" -Force

$PS_BOTAN_SCRIPT = "$PS_BOTAN_REPO_DIR/X-PsBotan.ps1"
$PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"

$json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON"))
$productInfo = ConvertFrom-Json $json
$BotanVersion = $productInfo.BotanVersion
& "$PS_BOTAN_SCRIPT" -Build -EmscriptenCompiler -Version $BotanVersion -BotanModules @()
& "$PS_BOTAN_SCRIPT" -Build -EmscriptenCompiler -Version $BotanVersion -ReleaseMode -BotanModules @()

Write-InfoBlue "████ End - Building libBotan"
Write-Host