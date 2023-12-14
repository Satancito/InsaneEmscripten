
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
Import-Module -Name "$(Get-Item "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1")" -Force -NoClobber

$X_PSCORE_FXS_UPDATE_SCRIPT = "$PSScriptRoot/X-PsCoreFxs-Update.ps1"; Test-Unused $X_PSCORE_FXS_UPDATE_SCRIPT
$Z_PSCORE_FXS_SCRIPT = "$PSScriptRoot/Z-PsCoreFxs.ps1"; Test-Unused $Z_PSCORE_FXS_SCRIPT;
$Z_PSCORE_FXS_CONFIG_JSON = "$PSScriptRoot/Z-PsCoreFxsConfig.json";
$Z_INSANE_EM_SCRIPT = "$PSScriptRoot/Z-InsaneEm.ps1";
$X_INSANE_EM_BUILD_BOTAN_SCRIPT = "$PSScriptRoot/X-InsaneEm-BuildBotan.ps1";
$X_INSANE_EM_BUILD_INSANE_SCRIPT = "$PSScriptRoot/X-InsaneEm-BuildInsane.ps1";
$X_INSANE_EM_CREATE_PROJECT_SCRIPT = "$PSScriptRoot/X-InsaneEm-CreateProject.ps1";
$X_INSANE_EM_BUILD_MODULE_SCRIPT = "$PSScriptRoot/X-InsaneEm-BuildProject.ps1";
$X_INSANE_EM_RUN_MODULE_SCRIPT = "$PSScriptRoot/X-InsaneEm-RunProject.ps1";
$X_INSANE_EM_PS_BOTAN_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsBotan/X-PsBotan.ps1";



$INSANE_EM_DEFAULT_TEMP_DIR = "$(Get-UserHome)/.InsaneEmscripten"
$INSANE_LIB_NAME = "libInsane.a"

$DOCS_PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"

$DENO_EXECUTABLE = "$(Get-UserHome)/.deno/bin/deno"


$INSANE_EM_INSTALL_DEBUG_DIR = "$(Get-UserHome)/.CppLibs/Insane-Debug-Emscripten"
$INSANE_EM_INSTALL_RELEASE_DIR = "$(Get-UserHome)/.CppLibs/Insane-Release-Emscripten"

$INSANE_EM_LIB_DEBUG_DIR = "$INSANE_EM_INSTALL_DEBUG_DIR/Lib"
$INSANE_EM_LIB_RELEASE_DIR = "$INSANE_EM_INSTALL_RELEASE_DIR/Lib"
$INSANE_EM_BOTAN_SUFFIX = "Insane"

$INSANE_EM_INCLUDE_DEBUG_DIR = "$INSANE_EM_INSTALL_DEBUG_DIR/Include"
$INSANE_EM_INCLUDE_RELEASE_DIR = "$INSANE_EM_INSTALL_RELEASE_DIR/Include"

function Get-InsaneLibraryDir {
    param (
        [Parameter(Mandatory = $true)]
        [ValidateSet("Lib", "Include", "Install")]
        [string]
        $DirType,

        [Parameter()]
        [switch]
        $ReleaseMode
    )
    $result = ($ReleaseMode.IsPresent ? $INSANE_EM_INSTALL_RELEASE_DIR : $INSANE_EM_INSTALL_DEBUG_DIR)
    switch ($DirType) {
        "Install" { 
            return $result
        }
        "Lib" { 
            return "$result/Lib"
        }
        "Include" {  
            return "$result/Include"
        }
        Default {
            throw "Not implemented DirType: $DirType"
        }
    }
}

function Test-RequiredTools {
    Write-Host
    Write-InfoBlue "Test - InsaneEm - Dependency tools"
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

    Write-InfoMagenta "== Java"
    $command = Get-Command "java"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    Write-InfoMagenta "== Node"
    $command = Get-Command "node"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    Write-InfoMagenta "== Deno"
    $command = Get-Command "deno"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    if ($IsWindows) {
        Write-InfoMagenta "== WSL"
        $command = Get-Command "wsl"
        Write-Host "$($command.Source)"
        & "$($command.Source)" --version
        Write-Host
    }
    
}

