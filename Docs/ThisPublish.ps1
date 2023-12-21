[CmdletBinding()]
param (

)
    
$Error.Clear()  
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

$json = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name
$ModuleVersion = $productInfo.Version
$ModuleIsES6Module = $productInfo.IsES6Module
$ModuleExportExtension = $ModuleIsES6Module ? "mjs": "js"
$ModuleFileName = "$ModuleExportName-$ModuleVersion-Release-*.$ModuleExportExtension"

& "$PSScriptRoot/ThisBuild.ps1" -ReleaseMode

$COMPILED_MODULE_DIR = "$PSScriptRoot/Build/Module"
$COMPILED_MODULE_FILE = Get-Item "$COMPILED_MODULE_DIR/$ModuleFileName" -ErrorAction SilentlyContinue
$DIST_DIR = "$PSScriptRoot/Dist"

try {
    
    Write-Host
    Write-InfoBlue "████ Publishing module"
    Write-Host
    
    Write-Host "Compiled: $COMPILED_MODULE_FILE"
    Remove-Item "$DIST_DIR" -Force -Recurse -ErrorAction Ignore
    New-Item "$DIST_DIR" -ItemType Directory -Force | Out-Null
    Copy-Item "$COMPILED_MODULE_FILE" -Destination "$DIST_DIR" -Force -Recurse
    Write-PrettyKeyValue "Publish file" "$DIST_DIR/$(Split-Path "$COMPILED_MODULE_FILE" -Leaf)"
}
finally {
    Write-InfoBlue "█ End publishing module"
    Write-Host
}
