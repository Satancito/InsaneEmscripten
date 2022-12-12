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
Write-InfoMagenta "████ Cloning Dependencies - InsaneEmscripten" 
Write-Host

$InsaneCpp = "https://github.com/Satancito/InsaneCpp.git"
$CommonCppLibs = "https://github.com/Satancito/CommonCppLibs.git"
$CommonCppIncludes = "https://github.com/Satancito/CommonCppIncludes.git"

$repos = @(
    @{ 
        Uri   = $InsaneCpp
        Name  = $(Get-VariableName $InsaneCpp)
        Clean = $CleanInsaneCpp.IsPresent 
    },
    @{ 
        Uri   = $CommonCppLibs
        Name  = $(Get-VariableName $CommonCppLibs) 
        Clean = $CleanCommonCppLibs.IsPresent
    },
    @{ 
        Uri   = $CommonCppIncludes
        Name  = $(Get-VariableName $CommonCppIncludes)
        Clean = $CleanCommonCppIncludes.IsPresent
    })

$LocationStackName = "CloneDeps"
try {
    Push-Location .. -StackName $LocationStackName
    $repos | ForEach-Object {
        $filesCount = (git -C "$($_.Name)" status --porcelain | Measure-Object | Select-Object -expand Count)
        if ($_.Clean -or ($filesCount -eq 0)) {
            Remove-Item -Path "./$($_.Name)" -Force -Recurse -ErrorAction Ignore
            Write-PrettyKeyValue "Cloning" "$($_.Name) - $($_.Uri)"
            git clone "$($_.Uri)"
        }
        else {
            Write-InfoYellow "Detected changes in repo `"$($_.Name)`", ignoring. If you need latest code, please run this script with `"-Clean$($_.Name)`" parameter.";
        }
    }
}
finally {
    Pop-LocationStack -StackName $LocationStackName
}

Write-InfoMagenta "█ End cloning dependencies - Finished"
Write-Host