[CmdletBinding(DefaultParameterSetName = "None")]
param (
    [Parameter(ParameterSetName = "Emrun", Mandatory = $false)]
    [switch]
    $Emrun,

    [Parameter(ParameterSetName = "Node", Mandatory = $false)]
    [switch]
    $NodeBrowser,

    [Parameter(ParameterSetName = "Deno", Mandatory = $false)]
    [switch]
    $DenoBrowser,

    [Parameter()]
    [switch]
    $NoLaunchBrowser
)

$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

$PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"
$json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name

[System.Collections.Hashtable]$params = new-object System.Collections.Hashtable
if($Emrun.IsPresent)
{
    $params.Add($(Get-VariableName $Emrun), $true)
}
if($NodeBrowser.IsPresent)
{
    $params.Add($(Get-VariableName $NodeBrowser), $true)
}
if($DenoBrowser.IsPresent)
{
    $params.Add($(Get-VariableName $DenoBrowser), $true)
}
if($NoLaunchBrowser.IsPresent)
{
    $params.Add($(Get-VariableName $NoLaunchBrowser), $true)
}
& "$PSScriptRoot/Dist/$ModuleExportName/X-RunModule.ps1" @params