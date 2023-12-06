
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
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
$X_INSANE_EM_TEST_REQUIRED_TOOLS_SCRIPT = "$PSScriptRoot/X-InsaneEm-TestRequiredTools.ps1"
$X_INSANE_EM_UPDATE_SUBMODULES_SCRIPT = "$PSScriptRoot/X-InsaneEm-UpdateSubmodules.ps1"
$X_INSANE_EM_INSTALL_EMSCRIPTEN_SCRIPT = "$PSScriptRoot/X-InsaneEm-InstallEmscripten.ps1"
$X_INSANE_EM_SET_EMSCRIPTEN_ENV_VARS_SCRIPT = "$PSScriptRoot/X-InsaneEm-SetEmscriptenEnvVars.ps1"
$X_PSCORE_FXS_UPDATE_SCRIPT = "$PSScriptRoot/X-PsCoreFxs-Update.ps1"
$Z_INSANE_EM_SCRIPT = "$PSScriptRoot/Z-InsaneEm.ps1"
$Z_PSCORE_FXS_SCRIPT = "$PSScriptRoot/Z-PsCoreFxs.ps1"
$Z_PSCORE_FXS_CONFIG_JSON = "$PSScriptRoot/Z-PsCoreFxsConfig.json"


$DOCS_PRODUCT_INFO_JSON = "$PSScriptRoot/Docs/ProductInfo.json"

$PS_BOTAN_REPO_URL = "https://github.com/Satancito/PsBotan.git"
$EMSCRIPTEN_SDK_REPO_URL = "https://github.com/emscripten-core/emsdk.git"
$INSANE_EM_REPO_URL = "https://github.com/Satancito/InsaneEmscripten.git"

$INSANE_EM_DEFAULT_TEMP_DIR = "$(Get-UserHome)/.InsaneEmscripten"

$DENO_EXECUTABLE = "$(Get-UserHome)/.deno/bin/deno"

$INSANE_LIB_NAME = "libInsane.a"

$INSANE_EM_INSTALL_DEBUG_DIR = "$(Get-UserHome)/.CppLibs/Insane-Debug-Emscripten"
$INSANE_EM_INSTALL_RELEASE_DIR = "$(Get-UserHome)/.CppLibs/Insane-Release-Emscripten"

$INSANE_EM_LIB_DEBUG_DIR = "$INSANE_EM_INSTALL_DEBUG_DIR/Lib"
$INSANE_EM_LIB_RELEASE_DIR = "$INSANE_EM_INSTALL_RELEASE_DIR/Lib"

$INSANE_EM_INCLUDE_DEBUG_DIR = "$INSANE_EM_INSTALL_DEBUG_DIR/Include"
$INSANE_EM_INCLUDE_RELEASE_DIR = "$INSANE_EM_INSTALL_RELEASE_DIR/Include"

function Get-InsaneLibraryDir {
    param (
        [Parameter(Mandatory=$true)]
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

$X_INSANE_EM_BUILD_BOTAN_SCRIPT ="$PSScriptRoot/X-InsaneEm-BuildBotan.ps1"
$X_INSANE_EM_BUILD_INSANE_SCRIPT ="$PSScriptRoot/X-InsaneEm-BuildInsane.ps1"
$X_INSANE_EM_CREATE_PROJECT_SCRIPT ="$PSScriptRoot/X-InsaneEm-CreateProject.ps1"
$X_INSANE_EM_BUILD_MODULE_SCRIPT ="$PSScriptRoot/X-InsaneEm-BuildProject.ps1"
$X_INSANE_EM_RUN_MODULE_SCRIPT ="$PSScriptRoot/X-InsaneEm-RunProject.ps1"

