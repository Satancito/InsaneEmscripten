param (
    [Parameter()]
    [switch]
    $Clean   
)

$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoMagenta "████ Cloning Dependencies - Insane" 
Write-Host

$InsaneCpp = "https://github.com/Satancito/InsaneCpp.git"
$CommonCppLibs = "https://github.com/Satancito/CommonCppLibs.git"
$CommonCppIncludes = "https://github.com/Satancito/CommonCppIncludes.git"

try {
    
    Push-Location ..
    if ($Clean.IsPresent) {
        Remove-Item "./$(Get-VariableName $InsaneCpp)" -Force -Recurse -ErrorAction Ignore
        Remove-Item "./$(Get-VariableName $CommonCppLibs)" -Force -Recurse -ErrorAction Ignore
        Remove-Item "./$(Get-VariableName $CommonCppIncludes)" -Force -Recurse -ErrorAction Ignore
    }
    if (!(Test-Path "./$(Get-VariableName $InsaneCpp)" -PathType Container))
    {
        Write-PrettyKeyValue "Cloning" "$InsaneCpp"; git clone "$InsaneCpp"; Test-LastExitCode
    }
    else {
        Write-InfoYellow "`"$InsaneCpp`" exists, ignoring. If you need latest code, please run this script with `"-Clean`" parameter.";
    }

    if (!(Test-Path "./$(Get-VariableName $CommonCppLibs)" -PathType Container))
    {
        Write-PrettyKeyValue "Cloning" "$CommonCppLibs"; git clone "$CommonCppLibs"; Test-LastExitCode
    }
    else {
        Write-InfoYellow "`"$CommonCppLibs`" exists, ignoring. If you need latest code, please run this script with `"-Clean`" parameter.";
    }

    if (!(Test-Path "./$(Get-VariableName $CommonCppIncludes)" -PathType Container))
    {        
        Write-PrettyKeyValue "Cloning" "$CommonCppIncludes"; git clone "$CommonCppIncludes"; Test-LastExitCode
    }
    else {
        Write-InfoYellow "`"$CommonCppIncludes`" exists, ignoring. If you need latest code, please run this script with `"-Clean`" parameter.";
    }
}
finally {
    Pop-Location
}


Write-InfoMagenta "█ End cloning dependencies - Finished"
Write-Host
