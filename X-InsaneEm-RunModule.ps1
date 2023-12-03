[CmdletBinding(DefaultParameterSetName = "None")]
param (
    [Parameter(ParameterSetName = "Emrun")]
    [switch]
    $Emrun,

    [Parameter(ParameterSetName = "BrowserNode")]
    [switch]
    $BrowserNodeServer,

    [Parameter(ParameterSetName = "BrowserDeno")]
    [switch]
    $BrowserDenoServer,

    [Parameter(ParameterSetName = "ConsoleNode")]
    [switch]
    $ConsoleNode,

    [Parameter(ParameterSetName = "ConsoleDeno")]
    [switch]
    $ConsoleDeno,

    [Parameter(ParameterSetName = "None")]
    [Parameter(ParameterSetName = "Emrun")]
    [Parameter(ParameterSetName = "BrowserNode")]
    [Parameter(ParameterSetName = "BrowserDeno")]
    [switch]
    $NoLaunchBrowser
)

$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
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
if($BrowserNodeServer.IsPresent)
{
    $params.Add($(Get-VariableName $BrowserNodeServer), $true)
}
if($BrowserDenoServer.IsPresent)
{
    $params.Add($(Get-VariableName $BrowserDenoServer), $true)
}
if($NoLaunchBrowser.IsPresent)
{
    $params.Add($(Get-VariableName $NoLaunchBrowser), $true)
}
if($ConsoleNode.IsPresent)
{
    $params.Add($(Get-VariableName $ConsoleNode), $true)
}
if($ConsoleDeno.IsPresent)
{
    $params.Add($(Get-VariableName $ConsoleDeno), $true)
}

& "$PSScriptRoot/Dist/$ModuleExportName/X-RunModule.ps1" @params