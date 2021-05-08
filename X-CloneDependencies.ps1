$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Clear-Host
Write-Host
Write-InfoMagenta "████ Cloning Dependencies - Insane" 
Write-Host

$InsaneCppRepo =  "https://github.com/Satancito/InsaneCpp.git"
$CommonCppLibsRepo =  "https://github.com/Satancito/CommonCppLibs.git"
$CommonCppIncludesRepo =  "https://github.com/Satancito/CommonCppIncludes.git"

try {
    Push-Location ..
    Write-PrettyKeyValue "Cloning" "$InsaneCppRepo"; git clone "$InsaneCppRepo"; Test-LastExitCode
    Write-PrettyKeyValue "Cloning" "$CommonCppLibsRepo"; git clone "$CommonCppLibsRepo"; Test-LastExitCode
    Write-PrettyKeyValue "Cloning" "$CommonCppIncludesRepo"; git clone "$CommonCppIncludesRepo"; Test-LastExitCode
}
finally {
    Pop-Location
}


Write-InfoMagenta "█ End cloning dependencies - Finished"
Write-Host
