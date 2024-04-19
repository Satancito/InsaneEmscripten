[CmdletBinding()]
param (

)
    
$Error.Clear()  
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-Init.ps1")" -Force -NoClobber

$json = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name
$ModuleVersion = $productInfo.Version
$ModuleIsES6Module = $productInfo.IsES6Module
$ModuleExportExtension = $ModuleIsES6Module ? "mjs": "js"
$ModuleFileName = "$PSScriptRoot/Bin/Release/Module/$ModuleExportName-$ModuleVersion-Release.$ModuleExportExtension"

& "$PSScriptRoot/X-Build.ps1" -ReleaseMode

try {
    
    Write-Host
    Write-InfoBlue "████ Publishing module"
    Write-Host
    $distDir = "$PSScriptRoot/Dist"
    Write-Host "Compiled: $ModuleFileName"
    Remove-Item "$distDir" -Force -Recurse -ErrorAction Ignore
    New-Item "$distDir" -ItemType Directory -Force | Out-Null
    Copy-Item "$ModuleFileName" -Destination "$distDir" -Force -Recurse
    Write-PrettyKeyValue "Publish file" "$distDir/$(Split-Path "$ModuleFileName" -Leaf)"
}
finally {
    Write-InfoBlue "█ End publishing module"
    Write-Host
}
