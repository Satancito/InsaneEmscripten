
function Test-Unused {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [System.Object]
        $Value
    )
    #Write-Host $Value
}

$Error.Clear()
$ErrorActionPreference = "Stop"
$X_INSANE_EM_Z_PS_CORE_FXS_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1"
Import-Module -Name "$(Get-Item "$X_INSANE_EM_Z_PS_CORE_FXS_INTERNAL_SCRIPT")" -Force -NoClobber

Update-GitSubmodules -Path $PSScriptRoot

$PRODUCT_INFO_JSON_FILE = "$PSScriptRoot/ProductInfo.json"; Test-Unused $PRODUCT_INFO_JSON_FILE;
$json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON_FILE"))
$productInfo = ConvertFrom-Json $json
$INSANE_EM_BOTAN_VERSION = $productInfo.BotanVersion
$INSANE_EM_INSANE_VERSION = $productInfo.InsaneVersion
$INSANE_EM_BOTAN_MAJOR_VERSION = "$("$INSANE_EM_BOTAN_VERSION".Split(".") | Select-Object -First 1)"; Test-Unused $INSANE_EM_BOTAN_MAJOR_VERSION
$INSANE_EM_BOTAN_SUFFIX = "Insane"; Test-Unused $INSANE_EM_BOTAN_SUFFIX;
$INSANE_EM_CLANG_FORMAT_FILE = "$PSScriptRoot/.clang-format"; Test-Unused $INSANE_EM_CLANG_FORMAT_FILE;
$Z_INSANE_EM_SCRIPT = "$PSScriptRoot/Z-InsaneEm.ps1";Test-Unused $Z_INSANE_EM_SCRIPT;

$INSANE_EM_DEFAULT_TEMP_DIR = "$(Get-UserHome)/.InsaneEmscripten"; Test-Unused $INSANE_EM_DEFAULT_TEMP_DIR;

$X_INSANE_EM_PS_BOTAN_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsBotan/X-PsBotan.ps1"; Test-Unused $X_INSANE_EM_PS_BOTAN_INTERNAL_SCRIPT;
$X_PS_EMSCRIPTEN_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsEmscripten/X-PsEmscripten-SDK.ps1"; Test-Unused $X_PS_EMSCRIPTEN_INTERNAL_SCRIPT;
$INSANE_EM_BUILD_CONFIGURATION_DEBUG = "Debug"
$INSANE_EM_BUILD_CONFIGURATION_RELEASE = "Release"
$INSANE_EM_INSANE_DIST_DEBUG_DIR =  "$(Get-UserHome)/.CppLibs/Insane-$INSANE_EM_INSANE_VERSION-$INSANE_EM_BUILD_CONFIGURATION_DEBUG-Emscripten"; Test-Unused $INSANE_EM_INSANE_DIST_DEBUG_DIR;
$INSANE_EM_INSANE_DIST_RELEASE_DIR =  "$(Get-UserHome)/.CppLibs/Insane-$INSANE_EM_INSANE_VERSION-$INSANE_EM_BUILD_CONFIGURATION_RELEASE-Emscripten"; Test-Unused $INSANE_EM_INSANE_DIST_RELEASE_DIR;
$INSANE_LIB_NAME = "libInsane.a"; Test-Unused $INSANE_LIB_NAME;


function Get-InsaneDir {
    param (
        [Parameter(Mandatory = $true, ParameterSetName = "Install")]
        [switch]
        $Install,

        [Parameter(Mandatory = $true, ParameterSetName = "Lib")]
        [switch]
        $Lib,

        [Parameter(Mandatory = $true, ParameterSetName = "Include")]
        [switch]
        $Include,

        [Parameter()]
        [switch]
        $ReleaseMode
    )
    $result = ($ReleaseMode.IsPresent ? $INSANE_EM_INSANE_DIST_RELEASE_DIR : $INSANE_EM_INSANE_DIST_DEBUG_DIR)
    if($Install.IsPresent)
    {
        return $result
    }

    if($Lib.IsPresent)
    {
        return "$result/Lib"
    }

    if($Include.IsPresent)
    {
        return "$result/Include"
    }
    
    throw "Get-InsaneDir. Not implemented DirType."
    
}


function Test-InsaneRequiredTools {
    Write-Host
    Write-InfoBlue "Test - InsaneEm - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Git"
    $command = Get-Command "git"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    Write-InfoMagenta "== Make"
    $command = Get-Command "make"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host
    
}

function Test-BotanRequiredTools {
    Write-Host
    Write-InfoBlue "Test - InsaneEm - Botan - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Python"
    $command = Get-Command "python"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    Write-InfoMagenta "== Git"
    $command = Get-Command "git"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    Write-InfoMagenta "== Make"
    $command = Get-Command "make"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host
    
}

