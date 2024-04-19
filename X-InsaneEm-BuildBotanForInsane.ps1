$ErrorActionPreference = "Stop"
Import-Module -Name "$PSScriptRoot/Z-InsaneEm.ps1" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
try {
        Write-Host
        Write-OutputIntroOutroMessage -Value "Building libBotan" -ForegroundColor Green
        & "$__INSANEEM_X_PSBOTAN_EMSCRIPTEN_INTERNAL_SCRIPT" -DestinationDirSuffix "$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX"
}
finally {
        
        Write-OutputIntroOutroMessage -Value "End - Building libBotan" -ForegroundColor Green -IsOutro
        Write-Host
}




