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
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
try {
    & "./X-BuildLib.ps1" -Clean:$Clean -EnableClangd:$EnableClangd
    Push-Location "Dist/Insane*BitCode"
    & "./X-BuildModule.ps1" -NoMinifyJsFiles:$NoMinifyJsFiles -TestMode
}
finally {
    Pop-Location
}