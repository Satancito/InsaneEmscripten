Import-Module -Name "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1" -Force -NoClobber

$productInfoFilename = "$PSScriptRoot/ProductInfo.json"
$json = [System.IO.File]::ReadAllText($productInfoFilename)
$productInfo = ConvertFrom-Json -InputObject $json
$__INSANEEM_PROJECT_PROJECTNAME_TEMP_DIR = "$(Get-UserHome)/.Temp_$($productInfo.Name)"; $null = $__INSANEEM_PROJECT_PROJECTNAME_TEMP_DIR
$__INSANEEM_PROJECT_PROJECTNAME_CLOSURE_COMPILER_URL = "https://repo1.maven.org/maven2/com/google/javascript/closure-compiler/v20230802/closure-compiler-v20230802.jar"; $null = $__INSANEEM_PROJECT_PROJECTNAME_CLOSURE_COMPILER_URL
$__INSANEEM_PROJECT_PROJECTNAME_CLOSURE_COMPILER_HASH = "D1A09A24CE1056943E01134A3D5A407D58B5E704"; $null = $__INSANEEM_PROJECT_PROJECTNAME_CLOSURE_COMPILER_HASH
$__INSANEEM_PROJECT_PROJECTNAME_INSANE_VERSION = "$($productInfo.InsaneVersion)"; $null = $__INSANEEM_PROJECT_PROJECTNAME_INSANE_VERSION

function Get-InsaneDistDir {
    [CmdletBinding()]
    param (
        [Parameter()]
        [switch]
        $ReleaseMode
    )
    return "$(Get-CppLibsDir)/Insane-$($productInfo.InsaneVersion)-Emscripten-Wasm-$($ReleaseMode.IsPresent ? "Release" : "Debug")"
}
