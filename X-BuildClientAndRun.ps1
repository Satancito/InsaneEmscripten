$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Clear-Host
Write-Host
Write-InfoBlue "████ Building Insane and Run Test client"
Write-Host

try {
    & "./X-BuildLib.ps1"
    Push-Location "Dist/Insane*BitCode"
    & "./X-BuildModule.ps1"
    & "./X-Run.ps1"
}
finally {
    Pop-Location
}


Write-InfoBlue "█ End building Insane and run Test client - Finished"
Write-Host
