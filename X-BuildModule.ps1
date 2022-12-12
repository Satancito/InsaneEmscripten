$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    Push-Location "Dist/Insane*BitCode"
    & "./X-BuildModule.ps1"
}
finally {
    Pop-Location
}