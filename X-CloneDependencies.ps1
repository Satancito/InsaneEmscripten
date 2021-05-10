$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

Clear-Host
Write-Host
Write-InfoMagenta "████ Cloning Dependencies - Insane" 
Write-Host

$InsaneCpp =  "https://github.com/Satancito/InsaneCpp.git"
$CommonCppLibs =  "https://github.com/Satancito/CommonCppLibs.git"
$CommonCppIncludes =  "https://github.com/Satancito/CommonCppIncludes.git"

try {
    Push-Location ..
    Remove-Item "./$(Get-VariableName $InsaneCpp)" -Force -Recurse -ErrorAction Ignore
    Remove-Item "./$(Get-VariableName $CommonCppLibs)" -Force -Recurse -ErrorAction Ignore
    Remove-Item "./$(Get-VariableName $CommonCppIncludes)" -Force -Recurse -ErrorAction Ignore
    Write-PrettyKeyValue "Cloning" "$InsaneCpp"; git clone "$InsaneCpp"; Test-LastExitCode
    Write-PrettyKeyValue "Cloning" "$CommonCppLibs"; git clone "$CommonCppLibs"; Test-LastExitCode
    Write-PrettyKeyValue "Cloning" "$CommonCppIncludes"; git clone "$CommonCppIncludes"; Test-LastExitCode
}
finally {
    Pop-Location
}


Write-InfoMagenta "█ End cloning dependencies - Finished"
Write-Host
