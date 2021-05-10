$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

try {
    & "./X-BuildLib.ps1"
    & "./X-BuildModule.ps1"
    & "./X-RunModule.ps1"
}
finally {
    Pop-Location
}