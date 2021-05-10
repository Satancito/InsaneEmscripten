$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

try {
    Push-Location "Dist/Insane*BitCode"
    & "./X-BuildModule.ps1"
}
finally {
    Pop-Location
}