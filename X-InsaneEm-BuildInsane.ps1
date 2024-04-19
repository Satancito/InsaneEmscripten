[CmdletBinding()]
param (
    [Parameter(ParameterSetName = "Build_Lib")]
    [switch]
    $Clean,

    [Parameter(ParameterSetName = "Build_Lib")]
    [string]
    $DestinationDir = [string]::Empty,


    [Parameter(ParameterSetName = "Build_Lib")]
    [string]
    $DistDirSuffix = [string]::Empty
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
$DestinationDir = [string]::IsNullOrWhiteSpace($DestinationDir) ? "$(Get-CppLibsDir)" : $DestinationDir
$DistDirSuffix = [string]::IsNullOrWhiteSpace($DistDirSuffix) ? [string]::Empty : "-$($DistDirSuffix)"
Install-EmscriptenSDK 
Test-InsaneDependencies
    
if ($Clean.IsPresent) {
    Write-InfoBlue "Cleaning Insane build dirs"
    $__INSANEEM_INSANE_BUILD_CONFIGURATIONS.Keys | ForEach-Object {
        $configuration = $__INSANEEM_INSANE_BUILD_CONFIGURATIONS[$_]
        Remove-Item -Path "$($configuration.CurrentWorkingDir)" -Force -Recurse -ErrorAction Ignore
        Write-Host "Removed: `"$($configuration.CurrentWorkingDir)`""
    }
}

$__INSANEEM_INSANE_BUILD_CONFIGURATIONS.Keys | ForEach-Object {
    $configuration = $__INSANEEM_INSANE_BUILD_CONFIGURATIONS[$_]
    $prefix = "$($configuration.DistDirName)$DistDirSuffix"
    try {
        Write-OutputIntroOutroMessage "████ InsaneEm - Building `"$prefix`"" -ForegroundColor Blue
        Write-Host
        New-Item -Path "$($configuration.CurrentWorkingDir)" -ItemType Directory -Force | Out-Null
        Push-Location "$($configuration.CurrentWorkingDir)"
        $null = Test-ExternalCommand "`"$env:EMSCRIPTEN_EMMAKE`" make -f $PSScriptRoot/Makefile All -j8 SOURCES_DIR=$PSScriptRoot BOTAN_MAJOR_VERSION=$__PSBOTAN_BOTAN_MAJOR_VERSION BUILD_DIR=$($configuration.CurrentWorkingDir) BUILD_CONFIGURATION=$($configuration.Name) BOTAN_DIR=$($configuration.BotanLibDir) DIST_DIR=$DestinationDir/$prefix" -ThrowOnFailure
       
        Write-Host "Creating compiler database(`"$__PSCOREFXS_CLANGD_COMPILATION_DATABASE_JSON`")..."
        Join-CompileCommandsJson -SourceDir "$($configuration.CurrentWorkingDir)" -DestinationDir "$PSScriptRoot"
    }
    finally {
        Pop-Location
        Write-OutputIntroOutroMessage "████ End - InsaneEm - Building `"$prefix`"" -ForegroundColor Blue -IsOutro
    }    
}


