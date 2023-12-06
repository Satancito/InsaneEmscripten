[CmdletBinding()]
param (
    [switch]
    $NoMinifyJsFiles,

    [switch]
    $ReleaseMode
)

$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

$PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"
$json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name

& "$PSScriptRoot/Dist/$ModuleExportName/ThisBuild.ps1" -NoMinifyJsFiles:$NoMinifyJsFiles -TestMode -ReleaseMode:$ReleaseMode