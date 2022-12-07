param (
    [Parameter()]
    [switch]
    $CleanInsaneCpp,   

    [Parameter()]
    [switch]
    $CleanCommonCppLibs,

    [Parameter()]
    [switch]
    $CleanCommonCppIncludes
)

$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PSCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoMagenta "████ Cloning Dependencies - Insane" 
Write-Host

$InsaneCpp = "https://github.com/Satancito/InsaneCpp.git"
$CommonCppLibs = "https://github.com/Satancito/CommonCppLibs.git"
$CommonCppIncludes = "https://github.com/Satancito/CommonCppIncludes.git"
$LocationStackName = "CloneDeps"
try {
    
    Push-Location .. -StackName $LocationStackName
    if ($CleanInsaneCpp.IsPresent) {
        Write-PrettyKeyValue "Removing" "$(Get-Location)/$(Get-VariableName $InsaneCpp)" -LabelForegroudColor Yellow
        Remove-Item "./$(Get-VariableName $InsaneCpp)" -Force -Recurse -ErrorAction Ignore
    }

    if ($CleanCommonCppLibs.IsPresent) {
        Write-PrettyKeyValue "Removing" "$(Get-Location)/$(Get-VariableName $CommonCppLibs)" -LabelForegroudColor Yellow
        Remove-Item "./$(Get-VariableName $CommonCppLibs)" -Force -Recurse -ErrorAction Ignore
    }

    if ($CleanCommonCppIncludes.IsPresent) {
        Write-PrettyKeyValue "Removing" "$(Get-Location)/$(Get-VariableName $CommonCppIncludes)" -LabelForegroudColor Yellow
        Remove-Item "./$(Get-VariableName $CommonCppIncludes)" -Force -Recurse -ErrorAction Ignore
    }

    if (!(Test-Path "./$(Get-VariableName $InsaneCpp)" -PathType Container))
    {
        Write-PrettyKeyValue "Cloning" "$InsaneCpp"; git clone "$InsaneCpp"; Test-LastExitCode
    }
    else {
        Write-InfoYellow "`"$InsaneCpp`" local directory exists, ignoring. If you need latest code, please run this script with `"-CleanInsaneCpp`" parameter.";
    }

    if (!(Test-Path "./$(Get-VariableName $CommonCppLibs)" -PathType Container))
    {
        Write-PrettyKeyValue "Cloning" "$CommonCppLibs"; git clone "$CommonCppLibs"; Test-LastExitCode
    }
    else {
        Write-InfoYellow "`"$CommonCppLibs`" local directory exists, ignoring. If you need latest code, please run this script with `"-CleanCommonCppLibs`" parameter.";
    }

    if (!(Test-Path "./$(Get-VariableName $CommonCppIncludes)" -PathType Container))
    {        
        Write-PrettyKeyValue "Cloning" "$CommonCppIncludes"; git clone "$CommonCppIncludes"; Test-LastExitCode
    }
    else {
        Write-InfoYellow "`"$CommonCppIncludes`" local directory exists, ignoring. If you need latest code, please run this script with `"-CleanCommonCppIncludes`" parameter.";
    }
}
finally {
    Pop-LocationStack -StackName $LocationStackName
}

Write-InfoMagenta "█ End cloning dependencies - Finished"
Write-Host
