[CmdletBinding()]
param (
    [Parameter(ParameterSetName = "Build_Lib")]
    [switch]
    $Build,

    [Parameter(ParameterSetName = "Build_Lib")]
    [Parameter(ParameterSetName = "Build_Botan")]
    [Parameter(ParameterSetName = "Build_All")]
    [string]
    $DestinationDir = [string]::Empty,

    [Parameter(ParameterSetName = "Build_Lib")]
    [Parameter(ParameterSetName = "Build_All")]
    [string]
    $DistDirSuffix = [string]::Empty,

    [Parameter(ParameterSetName = "Build_Lib")]
    [Parameter(ParameterSetName = "Build_All")]
    [string]
    $LibSuffix = [string]::Empty,

    [Parameter(ParameterSetName = "Build_Lib")]
    [Parameter(ParameterSetName = "Build_All")]
    [string]
    $LibsDir = [string]::Empty,

    [Parameter(Mandatory = $true, Position = 0, ParameterSetName = "Build_Botan")]
    [switch]
    $BuildBotan,

    [Parameter(Mandatory = $true, Position = 0, ParameterSetName = "Build_ICU")]
    [switch]
    $BuildICU,

    [Parameter(Mandatory = $true, Position = 0, ParameterSetName = "Build_All")]
    [switch]
    $BuildAll,

    [Parameter(ParameterSetName = "Build_All")]
    [switch]
    $SkipBotan,

    [Parameter(ParameterSetName = "Build_All")]
    [switch]
    $SkipICU,

    [Parameter(Mandatory = $true, ParameterSetName = "Install_EmscriptenSDK")]
    [switch]
    $InstallEmscriptenSDK,

    [Parameter(ParameterSetName = "Install_EmscriptenSDK")]
    [switch]
    $Force,

    [Parameter(Mandatory = $true, Position = 0, ParameterSetName = "Create_Project")]
    [switch]
    $CreateProject,

    [Parameter(Mandatory = $true, Position = 1, ParameterSetName = "Create_Project")]
    [string]
    $Name,

    [Parameter(Mandatory = $true, ParameterSetName = "Clean_TempDir")]
    [switch]
    $Clean
    
)

Import-Module -Name "$PSScriptRoot/Z-InsaneEm.ps1" -Force -NoClobber

if ($Clean.IsPresent) {
    Remove-InsaneEm
    exit
}

if ($BuildBotan.IsPresent) {
    Build-BotanLibraryForInsane -DestinationDir $DestinationDir
    exit
}

if ($BuildICU.IsPresent) {
    Build-IcuLibrary
    exit
}

if ($BuildAll.IsPresent) {
    if (!$SkipBotan.IsPresent) {
        Build-BotanLibraryForInsane -DestinationDir $DestinationDir
    }
    if (!$SkipICU.IsPresent) {
        Build-IcuLibrary
    }
}

if ($InstallEmscriptenSDK.IsPresent) {
    Install-EmscriptenSDK -Force:$Force
    exit
}

if ($CreateProject.IsPresent) {
    New-Project -ProjectName $Name
    exit
}

Build-InsaneLibrary -DestinationDir $DestinationDir -DistDirSuffix $DistDirSuffix -LibSuffix $LibSuffix -LibsDir $LibsDir
exit