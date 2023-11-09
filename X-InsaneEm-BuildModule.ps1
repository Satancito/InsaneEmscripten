[CmdletBinding()]
param (
    [switch]
    $NoMinifyJsFiles,

    [switch]
    $Clean,

    [switch]
    $EnableClangd
)

$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

$PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"
$json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name

& "$PSScriptRoot/X-InsaneEm-BuildLib.ps1" -Clean:$Clean -EnableClangd:$EnableClangd
& "$PSScriptRoot/Dist/$ModuleExportName/X-BuildModule.ps1" -NoMinifyJsFiles:$NoMinifyJsFiles -TestMode