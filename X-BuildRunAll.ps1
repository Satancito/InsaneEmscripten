$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    & "./X-TestDependencyTools.ps1"
    & "./X-InstallEmscripten.ps1"
    & "./X-BuildLib.ps1"
    & "./X-BuildModule.ps1"
    & "./X-RunModule.ps1"
}
finally {
    Pop-Location
}