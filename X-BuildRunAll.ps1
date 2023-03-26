$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    & "./X-BuildLib.ps1"
    Push-Location "Dist/Insane*BitCode"
    & "./X-BuildModule.ps1"
    & "./X-Run.ps1"
}
finally {
    Pop-Location
}