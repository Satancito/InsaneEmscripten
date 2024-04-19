$ErrorActionPreference = "Stop"
$null = $__INSANEEM_Z_PS_CORE_FXS_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1"
$null = $__INSANEEM_Z_PSBOTAN_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsBotan/Z-PsBotan.ps1"

Import-Module -Name "$__INSANEEM_Z_PS_CORE_FXS_INTERNAL_SCRIPT" -Force -NoClobber
Import-Module -Name "$__INSANEEM_Z_PSBOTAN_INTERNAL_SCRIPT" -Force -NoClobber

$__INSANEEM_GITHUB_URL = "https://github.com/Satancito/InsaneEmscripten.git"; $null = $__INSANEEM_GITHUB_URL
$__INSANEEM_X_PSBOTAN_EMSCRIPTEN_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsBotan/X-PsBotan-Emscripten.ps1"; $null = $__INSANEEM_X_PSBOTAN_EMSCRIPTEN_INTERNAL_SCRIPT
$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX = "Insane"; $null = $__INSANEEM_INSANE_BOTAN_DIR_SUFFIX
$__INSANEEM_Z_INSANEEM_SCRIPT = "$PSCommandPath"; $null = $__INSANEEM_Z_INSANEEM_SCRIPT
$__INSANEEM_CLANG_FORMAT_FILE = "$PSScriptRoot/.clang-format"; $null = $__INSANEEM_CLANG_FORMAT_FILE
$__INSANEEM_TEMP_DIR = "$(Get-UserHome)/.InsaneEm"; $null = $__INSANEEM_TEMP_DIR

$__INSANEEM_INSANE_VERSION = "1.0.0" # Update on next version
$__INSANEEM_INSANE_LIB_NAME = "libInsane.a"; $null = $__INSANEEM_INSANE_LIB_NAME
$__INSANEEM_INSANE_DEBUG_DIST_DIR_NAME = "Insane-$__INSANEEM_INSANE_VERSION-Emscripten-Wasm-Debug"; $null = $__INSANEEM_INSANE_DEBUG_DIST_DIR_NAME
$__INSANEEM_INSANE_RELEASE_DIST_DIR_NAME = "Insane-$__INSANEEM_INSANE_VERSION-Emscripten-Wasm-Release"; $null = $__INSANEEM_INSANE_RELEASE_DIST_DIR_NAME

$__INSANEEM_INSANE_BUILD_CONFIGURATIONS = @{
    Debug   = @{
        Name               = "Debug"
        DistDirName        = "$__INSANEEM_INSANE_DEBUG_DIST_DIR_NAME"
        BotanLibDir        = "$(Get-CppLibsDir)/$($__PSBOTAN_EMSCRIPTEN_CONFIGURATIONS.Debug.DistDirName)-$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX"
        CurrentWorkingDir  = "$__INSANEEM_TEMP_DIR/Bin/Debug"
    }
    Release = @{
        Name               = "Release"
        DistDirName        = "$__INSANEEM_INSANE_RELEASE_DIST_DIR_NAME"
        BotanLibDir        = "$(Get-CppLibsDir)/$($__PSBOTAN_EMSCRIPTEN_CONFIGURATIONS.Release.DistDirName)-$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX"
        CurrentWorkingDir  = "$__INSANEEM_TEMP_DIR/Bin/Release"
    }
}; $null = $__INSANEEM_INSANE_BUILD_CONFIGURATIONS

function Test-InsaneDependencies {
    Write-Host
    Write-InfoBlue "Test - Insane - Dependency tools"
    Write-Host

    Assert-GitExecutable
    Assert-MakeExecutable
    
}

