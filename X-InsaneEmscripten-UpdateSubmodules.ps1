Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
try {
    Push-Location "$PSScriptRoot"
    Test-Command "git submodule init" | Out-Null
    Test-Command "git submodule update --remote --recursive" -WriteOutput
}
finally {
    Pop-Location
}