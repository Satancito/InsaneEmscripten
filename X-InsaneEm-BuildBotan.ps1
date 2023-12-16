$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$X_INSANE_EM_Z_PS_CORE_FXS_INTERNAL_SCRIPT")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
        Write-Host
        Write-InfoBlue "████ Building libBotan"
        Write-Host
        Test-InsaneEmRequiredTools
        Update-GitSubmodules -Path $PSScriptRoot -Force

        & "$X_INSANE_EM_PS_BOTAN_INTERNAL_SCRIPT" -Build -EmscriptenCompiler -DestinationSuffix "$INSANE_EM_BOTAN_SUFFIX" -Version $INSANE_EM_BOTAN_VERSION -BotanModules @() -BotanOptions @()
        & "$X_INSANE_EM_PS_BOTAN_INTERNAL_SCRIPT" -Build -EmscriptenCompiler -DestinationSuffix "$INSANE_EM_BOTAN_SUFFIX" -Version $INSANE_EM_BOTAN_VERSION -BotanModules @() -BotanOptions @() -ReleaseMode
}
finally {
        Write-InfoBlue "████ End - Building libBotan"
        Write-Host
}




