[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Force
)
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber

Install-EmscriptenSDK -Force:$Force